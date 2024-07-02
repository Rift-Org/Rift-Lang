/////////////////////////////////////////////////////////////
///                                                       ///
///     ██████╗ ██╗███████╗████████╗                      ///
///     ██╔══██╗██║██╔════╝╚══██╔══╝                      ///
///     ██████╔╝██║█████╗     ██║                         ///
///     ██╔══██╗██║██╔══╝     ██║                         ///
///     ██║  ██║██║██║        ██║                         ///
///     ╚═╝  ╚═╝╚═╝╚═╝        ╚═╝                         ///
///     * RIFT CORE - The official compiler for Rift.     ///
///     * Copyright (c) 2024, Rift-Org                    ///
///     * License terms may be found in the LICENSE file. ///
///                                                       ///
/////////////////////////////////////////////////////////////

#include <ast/parser.hh>
#include <ast/stmt.hh>
#include <error/error.hh>
#include <memory>
#include <ast/prgm.hh>
#include <ast/env.hh>
#include <utils/literals.hh>

using namespace rift::scanner;

using env = rift::ast::Environment;

namespace rift
{
    namespace ast
    {

        #pragma mark - Public API

        std::unique_ptr<Program> Parser::parse()
        {
            try {
                return program();
            } catch (const ParserException &e) {
                return nullptr;
            }
        }

        #pragma mark - Expressions Parsing

        std::unique_ptr<Expr> Parser::primary()
        {
            if (match({Token(TokenType::FALSE, "false", "", line)}))
                return std::unique_ptr<Literal>(new Literal(Token(TokenType::FALSE, "false", "", line)));
            if (match({Token(TokenType::TRUE, "true", "", line)}))
                return std::unique_ptr<Literal>(new Literal(Token(TokenType::TRUE, "true", "", line)));
            if (match({Token(TokenType::NIL, "nil", "", line)}))
                return std::unique_ptr<Literal>(new Literal(Token(TokenType::NIL, "nil", "", line)));

            if (match({Token(TokenType::NUMERICLITERAL, "", "", line)}))
                return std::unique_ptr<Literal>(new Literal(peekPrev(1)));
            if (match({Token(TokenType::STRINGLITERAL, "", "", line)}))
                return std::unique_ptr<Literal>(new Literal(Token(peekPrev(1))));
            if (match({Token(TokenType::IDENTIFIER, "", "", line)}))
                return std::unique_ptr<Literal>(new Literal(Token(peekPrev(1))));
            if (match({Token(TokenType::C_IDENTIFIER, "", "", line)}))
                return std::unique_ptr<Literal>(new Literal(Token(peekPrev(1))));
            return nullptr;
        }

        std::unique_ptr<Expr> Parser::call()
        {
            auto expr = primary();

            // TODO: i have to somehow get the right paren, and then check if there is a semicolon
            // since that's the only way to verify between func test() {} and test(); 
            // note the "test()""
            if (peekPrev().type == TokenType::IDENTIFIER && peek() == Token(TokenType::LEFT_PAREN)) {
                // get function from token, and grab its paramaters so I can plug them in with args
                auto idt = peekPrev();
                auto func = env::getInstance(true).getEnv(idt.lexeme);
                if (func.type != TokenType::FUN) rift::error::report(line, "call", "Expected function", idt, ParserException("Expected function"));
                Tokens params = std::any_cast<Tokens>(func.literal);

                consume(Token(TokenType::LEFT_PAREN));
                auto arg = args(params);
                consume(Token(TokenType::RIGHT_PAREN));
                // another dillema, how do i handle return 3;
                // do I handle it here or in the return stmt, I choose later
                // consume(Token(TokenType::SEMICOLON));
                return std::unique_ptr<Call>(new Call(std::move(expr), std::move(arg)));
            }

            return expr;
        }

        std::unique_ptr<Expr> Parser::unary()
        {
            if (match({Token(TokenType::BANG, "!", "", line)})) {
                auto op = peekPrev();
                auto right = unary();
                if (right == nullptr) rift::error::report(line, "unary", "Expected expression after unary operator", op, ParserException("Expected expression after unary operator"));
                return std::unique_ptr<Unary>(new Unary(op, std::move(right)));
            }

            if (match({Token(TokenType::MINUS, "-", "", line)})) {
                auto op = peekPrev();
                auto right = unary();
                if (right == nullptr) rift::error::report(line, "unary", "Expected expression after unary operator", op, ParserException("Expected expression after unary operator"));
                return std::unique_ptr<Unary>(new Unary(op, std::move(right)));
            }

            return call();
        }

        std::unique_ptr<Expr> Parser::factor()
        {
            auto expr = unary();

            while (match({Token(TokenType::STAR, "*", "", line)}) || match({Token(TokenType::SLASH, "/", "", line)})) {
                auto op = peekPrev();
                auto right = unary();
                if (expr == nullptr) rift::error::report(line, "factor", "Expected number before factor operator", op, ParserException("Expected number before factor operator"));
                if (right == nullptr) rift::error::report(line, "factor", "Expected number after factor operator", op, ParserException("Expected number after factor operator"));
                expr = std::unique_ptr<Binary>(new Binary(std::move(expr), op, std::move(right)));
            }

            return expr;
        }

        std::unique_ptr<Expr> Parser::term()
        {
            auto expr = factor();

            while (match({Token(TokenType::MINUS, "-", "", line)}) || match({Token(TokenType::PLUS, "+", "", line)})) {
                auto op = peekPrev();
                auto right = factor();
                if (expr == nullptr) rift::error::report(line, "term", "Expected number before term operator", op, ParserException("Expected number before term operator"));
                if (right == nullptr) rift::error::report(line, "term", "Expected number after term operator", op, ParserException("Expected number after term operator"));
                expr = std::unique_ptr<Binary>(new Binary(std::move(expr), op, std::move(right)));
            }

            return expr;
        }

        std::unique_ptr<Expr> Parser::comparison()
        {
            auto expr = term();

            while (match({Token(TokenType::GREATER, ">", "", line)}) || match({Token(TokenType::GREATER_EQUAL, ">=", "", line)}) || match({Token(TokenType::LESS, "<", "", line)}) || match({Token(TokenType::LESS_EQUAL, "<=", "", line)}) ) {
                auto op = peekPrev();
                auto right = term();
                if (expr == nullptr) rift::error::report(line, "comparison", "Expected expression before comparison operator", op, ParserException("Expected expression before comparison operator"));
                if (right == nullptr) rift::error::report(line, "comparison", "Expected expression after comparison operator", op, ParserException("Expected expression after comparison operator"));
                expr = std::unique_ptr<Binary>(new Binary(std::move(expr), op, std::move(right)));
            }

            return expr;
        }

        std::unique_ptr<Expr> Parser::equality()
        {
            auto expr = comparison();

            if (match({Token(TokenType::BANG_EQUAL, "!=", "", line)}) || match({Token(TokenType::EQUAL_EQUAL, "==", "", line)}) || 
                match({Token(TokenType::NULLISH_COAL, "??", "", line)}) || match({Token(TokenType::LOG_AND, "&&", "", line)}) ) {
                auto op = peekPrev();
                auto right = comparison();
                if (expr == nullptr) rift::error::report(line, "equality", "Expected expression before equality operator", op, ParserException("Expected expression before equality operator"));
                if (right == nullptr) rift::error::report(line, "equality", "Expected expression after equality operator", op, ParserException("Expected expression after equality operator"));
                expr = std::unique_ptr<Binary>(new Binary(std::move(expr), op, std::move(right)));
            }

            return expr;
        };

        std::unique_ptr<Expr> Parser::ternary()
        {
            auto expr = equality();

            if (match({Token(TokenType::QUESTION, "?", "", line)})) {
                auto left = equality();
                consume(Token(TokenType::COLON, ":", "", line), std::unique_ptr<ParserException>(new ParserException("Expected a colon while expecting a ternary operator")));
                auto right = equality();
                return std::unique_ptr<Ternary>(new Ternary(std::move(expr),std::move(left),std::move(right)));
            }

            return expr;
        }

        std::unique_ptr<Expr> Parser::assignment()
        {
            if(match({Token(TokenType::EQUAL)})) {
                auto idt = peekPrev(2);
                auto expr = ternary();
                if (expr == nullptr) 
                    rift::error::report(line, "assignment", "Expected expression after assignment operator", peekPrev(), ParserException("Expected expression after assignment operator"));

                return std::unique_ptr<Assign>(new Assign(idt, std::move(expr)));
            }

            return ternary();
        }

        std::unique_ptr<Expr> Parser::expression()
        {
            auto ret = assignment();
            return ret;
        }

        #pragma mark - Statements Parsing

        std::unique_ptr<Stmt> Parser::ret_stmt()
        {
            std::unique_ptr<Stmt> stmt;
            if (consume (Token(TokenType::PRINT, "", "", line))) {
                stmt = statement_print();
            } else if (consume(Token(TokenType::IF, "if", "if", line))) {
                stmt = statement_if();
            } else if (consume(Token(TokenType::RETURN))) {
                stmt = statement_return();
            } else {
                stmt = statement_expression();
            }
            return stmt;
        }

        std::unique_ptr<StmtExpr> Parser::statement_expression()
        {
            auto expr = expression();
            return std::unique_ptr<StmtExpr>(new StmtExpr(std::move(expr)));
        }

        std::unique_ptr<StmtPrint> Parser::statement_print()
        {
            consume(Token(TokenType::LEFT_PAREN, "(", "", line), std::unique_ptr<ParserException>(new ParserException("Expected '(' after print")));
            auto expr = expression();
            consume(Token(TokenType::RIGHT_PAREN, ")", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ')' after print")));
            consume(Token(TokenType::SEMICOLON, ";", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ';' after print statement")));
            return std::unique_ptr<StmtPrint>(new StmtPrint(expr));
        }

        std::unique_ptr<StmtIf> Parser::statement_if()
        {
            std::unique_ptr<StmtIf> ret = std::make_unique<StmtIf>();
            consume(Token(TokenType::LEFT_PAREN, "(", "", line), std::unique_ptr<ParserException>(new ParserException("Expected '(' after if")));
            auto expr = expression();
            consume(Token(TokenType::RIGHT_PAREN, ")", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ')' after if")));
            
            /// if stmt
            StmtIf::Stmt* if_stmt= new StmtIf::Stmt(std::move(expr));

            // block vs stmt
            if (peek() == Token(TokenType::LEFT_BRACE, "{", "", line)) {
                consume(Token(TokenType::LEFT_BRACE, "{", "", line), std::unique_ptr<ParserException>(new ParserException("Expected '{' after if block")));
                auto blk = block();
                if_stmt->blk = std::move(blk);
            } else {
                auto stmt = ret_stmt();
                if_stmt->stmt = std::move(stmt);
            }
            ret->if_stmt = if_stmt;

            /// elif stmts
            if (peek() == Token(TokenType::ELIF, "elif", "elif", line)) {
                std::vector<StmtIf::Stmt*> elif_stmts = {};

                while (consume(Token(TokenType::ELIF, "elif", "elif", line))) {
                    auto expr = expression();
                    StmtIf::Stmt* curr = new StmtIf::Stmt(std::move(expr));
                     // block vs stmt
                    if (peek() == Token(TokenType::LEFT_BRACE, "{", "", line)) {
                        consume(Token(TokenType::LEFT_BRACE, "{", "", line), std::unique_ptr<ParserException>(new ParserException("Expected '{' after elif block")));
                        auto blk = block();
                        curr->blk = std::move(blk);
                    } else {
                        std::unique_ptr<Stmt> stmt = ret_stmt();
                        curr->stmt = std::move(stmt);
                    }
                    elif_stmts.push_back(curr);
                }
                ret->elif_stmts = elif_stmts;
            }


            /// else stmt
            if (consume(Token(TokenType::ELSE, "else", "else", line))) {
                StmtIf::Stmt* else_stmt = new StmtIf::Stmt();
                // block vs stmt
                if (peek() == Token(TokenType::LEFT_BRACE, "{", "", line)) {
                    consume(Token(TokenType::LEFT_BRACE, "{", "", line), std::unique_ptr<ParserException>(new ParserException("Expected '{' after else block")));
                    auto blk = block();
                    else_stmt->blk = std::move(blk);
                } else {
                    std::unique_ptr<Stmt> stmt = ret_stmt();
                    else_stmt->stmt = std::move(stmt);
                }
                ret->else_stmt = else_stmt;
            }

            return ret;
        }

        std::unique_ptr<StmtReturn> Parser::statement_return()
        {
            auto expr = expression();
            consume(Token(TokenType::SEMICOLON, ";", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ';' after return statement")));
            return std::unique_ptr<StmtReturn>(new StmtReturn(std::move(expr)));
        }

        #pragma mark - Declarations Parsing

        std::unique_ptr<DeclStmt> Parser::declaration_statement()
        {
            std::unique_ptr<Stmt> stmt = ret_stmt();
            return std::make_unique<DeclStmt>(std::move(stmt));
        }

        std::unique_ptr<DeclVar> Parser::declaration_variable(bool mut)
        {
            // make sure there is an identifier
            auto tok_t = mut ? TokenType::IDENTIFIER : TokenType::C_IDENTIFIER;
            if (!consume(Token(TokenType::IDENTIFIER, "", "", line)) && !consume(Token(TokenType::C_IDENTIFIER, "", "", line)))
                rift::error::report(line, "declaration_variable", "Expected variable name", peek(), ParserException("Expected variable name"));
            auto idt = peekPrev();

            // make sure the identifier is not already declared
            /// @note this is just a check, the actual declaration is done in the evaluator
            ///       this also checks if any outer block has already declared this variable
            if (env::getInstance(true).getEnv(castString(idt)).type != TokenType::NIL)
                rift::error::report(line, "declaration_variable", "🛑 Variable '" + castString(idt) + "' already declared at line: " + castNumberString(idt.line), idt, ParserException("Variable '" + castString(idt) + "' already declared"));
            env::getInstance(true).setEnv(castString(idt), idt, idt.type == TokenType::C_IDENTIFIER);

            if(peek() == Token(TokenType::EQUAL, "=", "", line)) {
                auto expr = assignment();
                consume(Token(TokenType::SEMICOLON, ";", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ';' after variable assignment")));
                idt.type = tok_t;
                return std::make_unique<DeclVar>(idt, std::move(expr));
            } else if (!mut) {
                rift::error::report(line, "declaration_variable", "🛑 Constants must be defined", idt, ParserException("Constants must be defined"));
            }

            if (!consume(Token(TokenType::IDENTIFIER, "", "", line)) && !consume(Token(TokenType::C_IDENTIFIER, "", "", line)))
                rift::error::report(line, "declaration_variable", "Expected variable name", peek(), ParserException("Expected variable name"));
            idt = peekPrev();

            consume(Token(TokenType::SEMICOLON, ";", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ';' after variable declaration")));
            return std::unique_ptr<DeclVar>(new DeclVar(Token(tok_t, idt.lexeme, idt.literal, idt.line)));
        }

        std::unique_ptr<For> Parser::for_()
        {
            std::unique_ptr<For> _for = std::make_unique<For>();
            consume(Token(TokenType::LEFT_PAREN, "(", "", line), std::unique_ptr<ParserException>(new ParserException("Expected '(' after for")));

            // first ;
            if (match({Token(TokenType::VAR, "", "", line)})) {
                _for->decl = std::move(declaration_variable(true));
            } else if (match({Token(TokenType::CONST, "", "", line)}))  {
                _for->decl = std::move(declaration_variable(false));
            } else if(peek(Token(TokenType::IDENTIFIER, "", "", line))) {
                _for->stmt_l = std::move(ret_stmt());
            }
            // taken by var_decl();
            // consume(Token(TokenType::SEMICOLON, ";", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ';' after for first statement")));

            // second ;
            auto expr = expression();
            _for->expr = std::move(expr);
            consume(Token(TokenType::SEMICOLON, ";", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ';' after for second statement")));

            // third ;
            if(match({Token(TokenType::IDENTIFIER, "", "", line)}))
                _for->stmt_r = std::move(ret_stmt());
            consume(Token(TokenType::RIGHT_PAREN, ")", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ')' after for")));

            if (match({Token(TokenType::LEFT_BRACE, "{", "", line)})) {
                _for->blk = std::move(block());
            } else {
                _for->stmt_o = std::move(ret_stmt());
            }

            return _for;
        }

        std::unique_ptr<DeclFunc> Parser::declaration_func() 
        {
            std::unique_ptr<DeclFunc> _func = std::make_unique<DeclFunc>();
            _func->func = function();
            if (_func->func->blk == nullptr) {
                consume(Token(TokenType::SEMICOLON, ";", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ';' after function declaration")));
            }
            return _func;
        }

        #pragma mark - Program / Block Parsing

        vec_prog Parser::ret_decl()
        {
            vec_prog decls = std::make_unique<std::vector<std::unique_ptr<Decl>>>();
            if (consume (Token(TokenType::VAR, "", "", line))) {
                auto test = declaration_variable(true);
                decls->emplace_back(std::move(test));
            } else if (consume (Token(TokenType::CONST, "", "", line))) {
                auto test = declaration_variable(false);
                decls->emplace_back(std::move(test));
            } else if (consume (Token(TokenType::FOR, "", "", line)))  {
                decls->emplace_back(for_());
            } else if(match({Token(TokenType::LEFT_BRACE, "{", "", line)})) {
                auto inner_decls = std::move(block()->decls);
                decls->insert(decls->end(), std::make_move_iterator(inner_decls->begin()), std::make_move_iterator(inner_decls->end()));
            } else if (match({Token(TokenType::FUN)})) {
                decls->emplace_back(declaration_func());
            } else {
                decls->emplace_back(declaration_statement());
            }
            return decls;
        }

        std::unique_ptr<Block> Parser::block()
        {
            vec_prog decls = std::make_unique<std::vector<std::unique_ptr<Decl>>>();

            env::addChild(true);
            while (!atEnd() && !peek(Token(TokenType::RIGHT_BRACE, "}", "", line))) {
                auto inner = ret_decl();
                decls->insert(decls->end(), std::make_move_iterator(inner->begin()), std::make_move_iterator(inner->end()));
            }
            env::removeChild(true);

            if (!match({Token(TokenType::RIGHT_BRACE, "}", "", line)})) 
                rift::error::report(line, "statement_block", "Expected '}' after block", peek(), ParserException("Expected '}' after block"));

            return std::unique_ptr<Block>(new Block(std::move(decls)));
        }

        #pragma mark - Functions / Methods

        std::unique_ptr<DeclFunc::Func> Parser::function()
        {
            std::unique_ptr<DeclFunc::Func> ret = std::make_unique<DeclFunc::Func>();
            auto idt = consume_va({Token(TokenType::IDENTIFIER), Token(TokenType::C_IDENTIFIER)}, std::unique_ptr<ParserException>(new ParserException("Expected function name")));
            ret->name = idt;

            consume(Token(TokenType::LEFT_PAREN, "(", "", line), std::unique_ptr<ParserException>(new ParserException("Expected '(' after function name")));
            ret->params = params();
            consume(Token(TokenType::RIGHT_PAREN, ")", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ')' after function params")));
            // give the params (usefull for the call operator)
            env::getInstance(true).setEnv(idt.lexeme, Token(TokenType::FUN, idt.lexeme, ret->params, idt.line), false);

            if(match({Token(TokenType::LEFT_BRACE, "{", "", line)})) {
                ret->blk = std::move(block());
            } else if(match({Token(TokenType::FAT_ARROW, "=>", "", line)})) {
                // TODO: allow stmt to emulate lambdas
                rift::error::report(line, "function", "Lambdas not implemented yet", peek(), ParserException("Lambdas not implemented yet"));
            } else {
                consume(Token(TokenType::SEMICOLON, ";", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ';' after function declaration")));
            }

            return ret;
        }

        // should be a comma operator instead... sigh aman
        Tokens Parser::params()
        {
            Tokens toks = {};
            while(peek().type != TokenType::RIGHT_PAREN) {
                toks.push_back(consume_va({Token(TokenType::IDENTIFIER), Token(TokenType::C_IDENTIFIER)}, std::unique_ptr<ParserException>(new ParserException("Expected parameter name"))));
                if (!consume(Token(TokenType::COMMA, ",", "", line))) break;
            }
            return toks;
        }

        Exprs Parser::args(Tokens params)
        {
            Exprs exprs = {};
            int idx = 0;
            while(peek().type != TokenType::RIGHT_PAREN) {
                auto exp = expression();
                if (idx >= params.size()) 
                    rift::error::report(line, "args", "Too many arguments", peek(), ParserException("Too many arguments"));
                if (exp == nullptr) 
                    rift::error::report(line, "args", "Expected expression", peek(), ParserException("Expected expression"));
                
                exprs.insert({params[idx].lexeme, std::move(exp)});
                idx++;
            }
            return exprs;
        }


        #pragma mark - Application

        std::unique_ptr<Program> Parser::program()
        {
            vec_prog decls = std::make_unique<std::vector<std::unique_ptr<Decl>>>();

            while (!atEnd()) {
                auto inner = ret_decl();
                decls->insert(decls->end(), std::make_move_iterator(inner->begin()), std::make_move_iterator(inner->end()));
            }

            env::getInstance(true).clear(true);
            return std::unique_ptr<Program>(new Program(std::move(decls)));
        }

        # pragma mark - Utilities

        void Parser::synchronize()
        {
            advance();

            while (!atEnd()) {
                if (peekPrev().type == TokenType::SEMICOLON) return;

                switch (peek().type) {
                    case TokenType::CLASS:
                    case TokenType::FUN:
                    case TokenType::VAR:
                    case TokenType::FOR:
                    case TokenType::IF:
                    case TokenType::WHILE:
                    case TokenType::PRINT:
                    case TokenType::RETURN:
                        return;
                    default:
                        break;
                }

                advance();
            }
        }
    }
}