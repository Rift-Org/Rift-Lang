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

namespace rift
{
    namespace ast
    {

        static Environment* curr_env = &rift::ast::Environment::getInstance(true);

        #pragma mark - Public API

        std::unique_ptr<Program> Parser::parse()
        {
            try {
                return program();
            } catch (const ParserException &e) {
                return nullptr;
            }
        }

        ////////////////////////////////////////////////////////////////////////
        #pragma mark - Expressions Parsing
        ////////////////////////////////////////////////////////////////////////

        std::unique_ptr<Expr<Token>> Parser::primary()
        {
            if (match({Token(TokenType::FALSE, "false", "", line)}))
                return std::unique_ptr<Expr<Token>>(new Literal<Token>(Token(TokenType::FALSE, "false", "", line)));
            if (match({Token(TokenType::TRUE, "true", "", line)}))
                return std::unique_ptr<Expr<Token>>(new Literal<Token>(Token(TokenType::TRUE, "true", "", line)));
            if (match({Token(TokenType::NIL, "nil", "", line)}))
                return std::unique_ptr<Expr<Token>>(new Literal<Token>(Token(TokenType::NIL, "nil", "", line)));

            if (match({Token(TokenType::NUMERICLITERAL, "", "", line)}))
                return std::unique_ptr<Expr<Token>>(new Literal<Token>(peekPrev(1)));
            if (match({Token(TokenType::STRINGLITERAL, "", "", line)}))
                return std::unique_ptr<Expr<Token>>(new Literal<Token>(Token(peekPrev(1))));
            return nullptr;
        }

        std::unique_ptr<Expr<Token>> Parser::var_expr()
        {
            if (match({Token(TokenType::IDENTIFIER, "", "", line)}))
                return std::unique_ptr<Expr<Token>>(new VarExpr<Token>(Token(peekPrev(1))));
            if (match({Token(TokenType::C_IDENTIFIER, "", "", line)}))
                return std::unique_ptr<Expr<Token>>(new VarExpr<Token>(Token(peekPrev(1))));
            else
                return primary();
        }

        Call<Token>::Exprs Parser::args(Tokens params)
        {
            Call<Token>::Exprs exprs = {};
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

        std::unique_ptr<Expr<Token>> Parser::call()
        {
            auto expr = primary();

            // TODO: i have to somehow get the right paren, and then check if there is a semicolon
            // since that's the only way to verify between func test() {} and test(); 
            // note the "test()""
            if (peekPrev().type == TokenType::IDENTIFIER && peek() == Token(TokenType::LEFT_PAREN)) {
                // get function from token, and grab its paramaters so I can plug them in with args
                auto idt = peekPrev();
                auto func = curr_env->getEnv<Token>(idt.lexeme);
                Tokens params;
                // std::cout << idt.lexeme << ":" << func.lexeme << std::endl;

                params = std::any_cast<Tokens>(func.literal);
                consume(Token(TokenType::LEFT_PAREN));
                auto arg = args(params);
                consume(Token(TokenType::RIGHT_PAREN));
                // another dillema, how do i handle return 3;
                // do I handle it here or in the return stmt, I choose later
                // consume(Token(TokenType::SEMICOLON));
                return std::unique_ptr<Expr<Token>>(new Call<Token>(idt, std::move(arg)));
            }

            return expr;
        }

        std::unique_ptr<Expr<Token>> Parser::unary()
        {
            if (match({Token(TokenType::BANG, "!", "", line)})) {
                auto op = peekPrev();
                auto right = unary();
                if (right == nullptr) rift::error::report(line, "unary", "Expected expression after unary operator", op, ParserException("Expected expression after unary operator"));
                return std::unique_ptr<Expr<Token>>(new Unary<Token>(op, std::move(right)));
            }

            if (match({Token(TokenType::MINUS, "-", "", line)})) {
                auto op = peekPrev();
                auto right = unary();
                if (right == nullptr) rift::error::report(line, "unary", "Expected expression after unary operator", op, ParserException("Expected expression after unary operator"));
                return std::unique_ptr<Expr<Token>>(new Unary<Token>(op, std::move(right)));
            }

            return call();
        }

        std::unique_ptr<Expr<Token>> Parser::factor()
        {
            auto expr = unary();

            while (match({Token(TokenType::STAR, "*", "", line)}) || match({Token(TokenType::SLASH, "/", "", line)})) {
                auto op = peekPrev();
                auto right = unary();
                if (expr == nullptr) rift::error::report(line, "factor", "Expected number before factor operator", op, ParserException("Expected number before factor operator"));
                if (right == nullptr) rift::error::report(line, "factor", "Expected number after factor operator", op, ParserException("Expected number after factor operator"));
                expr = std::unique_ptr<Expr<Token>>(new Binary<Token>(std::move(expr), op, std::move(right)));
            }

            return expr;
        }

        std::unique_ptr<Expr<Token>> Parser::term()
        {
            auto expr = factor();

            while (match({Token(TokenType::MINUS, "-", "", line)}) || match({Token(TokenType::PLUS, "+", "", line)})) {
                auto op = peekPrev();
                auto right = factor();
                if (expr == nullptr) rift::error::report(line, "term", "Expected number before term operator", op, ParserException("Expected number before term operator"));
                if (right == nullptr) rift::error::report(line, "term", "Expected number after term operator", op, ParserException("Expected number after term operator"));
                expr = std::unique_ptr<Expr<Token>>(new Binary<Token>(std::move(expr), op, std::move(right)));
            }

            return expr;
        }

        std::unique_ptr<Expr<Token>> Parser::comparison()
        {
            auto expr = term();

            while (match({Token(TokenType::GREATER, ">", "", line)}) || match({Token(TokenType::GREATER_EQUAL, ">=", "", line)}) || match({Token(TokenType::LESS, "<", "", line)}) || match({Token(TokenType::LESS_EQUAL, "<=", "", line)}) ) {
                auto op = peekPrev();
                auto right = term();
                if (expr == nullptr) rift::error::report(line, "comparison", "Expected expression before comparison operator", op, ParserException("Expected expression before comparison operator"));
                if (right == nullptr) rift::error::report(line, "comparison", "Expected expression after comparison operator", op, ParserException("Expected expression after comparison operator"));
                expr = std::unique_ptr<Expr<Token>>(new Binary<Token>(std::move(expr), op, std::move(right)));
            }

            return expr;
        }

        std::unique_ptr<Expr<Token>> Parser::equality()
        {
            auto expr = comparison();

            if (match({Token(TokenType::BANG_EQUAL, "!=", "", line)}) || match({Token(TokenType::EQUAL_EQUAL, "==", "", line)}) || 
                match({Token(TokenType::NULLISH_COAL, "??", "", line)}) || match({Token(TokenType::LOG_AND, "&&", "", line)}) ) {
                auto op = peekPrev();
                auto right = comparison();
                if (expr == nullptr) rift::error::report(line, "equality", "Expected expression before equality operator", op, ParserException("Expected expression before equality operator"));
                if (right == nullptr) rift::error::report(line, "equality", "Expected expression after equality operator", op, ParserException("Expected expression after equality operator"));
                expr = std::unique_ptr<Expr<Token>>(new Binary<Token>(std::move(expr), op, std::move(right)));
            }

            return expr;
        };

        std::unique_ptr<Expr<Token>> Parser::ternary()
        {
            auto expr = equality();

            if (match({Token(TokenType::QUESTION, "?", "", line)})) {
                auto left = equality();
                consume(Token(TokenType::COLON, ":", "", line), std::unique_ptr<ParserException>(new ParserException("Expected a colon while expecting a ternary operator")));
                auto right = equality();
                return std::unique_ptr<Expr<Token>>(new Ternary<Token>(std::move(expr),std::move(left),std::move(right)));
            }

            return expr;
        }

        std::unique_ptr<Expr<Token>> Parser::assignment()
        {
            if(match({Token(TokenType::EQUAL)})) {
                auto idt = peekPrev(2);
                auto expr = ternary();
                if (expr == nullptr) 
                    rift::error::report(line, "assignment", "Expected expression after assignment operator", peekPrev(), ParserException("Expected expression after assignment operator"));

                return std::unique_ptr<Expr<Token>>(new Assign<Token>(idt, std::move(expr)));
            }

            return ternary();
        }

        std::unique_ptr<Expr<Token>> Parser::expression()
        {
            auto ret = assignment();
            return ret;
        }

        ////////////////////////////////////////////////////////////////////////
        #pragma mark - Statements Parsing
        ////////////////////////////////////////////////////////////////////////

        std::unique_ptr<Stmt<void>> Parser::ret_stmt()
        {
            std::unique_ptr<Stmt<void>> stmt;
            if (consume (Token(TokenType::PRINT, "", "", line))) {
                stmt = statement_print();
            } else if (consume(Token(TokenType::IF, "if", "if", line))) {
                stmt = statement_if();
            } else if (consume(Token(TokenType::RETURN))) {
                stmt = statement_return();
            } else if (consume (Token(TokenType::FOR, "", "", line)))  {
                stmt = statement_for();
            } else if(match({Token(TokenType::LEFT_BRACE, "{", "", line)})) {
                stmt = statement_block();
            } else {
                stmt = statement_expression();
            }
            return stmt;
        }

        std::unique_ptr<Stmt<void>> Parser::statement_expression()
        {
            auto expr = expression();
            return std::unique_ptr<Stmt<void>>(new StmtExpr<void>(std::move(expr)));
        }

        std::unique_ptr<Stmt<void>> Parser::statement_print()
        {
            consume(Token(TokenType::LEFT_PAREN, "(", "", line), std::unique_ptr<ParserException>(new ParserException("Expected '(' after print")));
            auto expr = expression();
            consume(Token(TokenType::RIGHT_PAREN, ")", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ')' after print")));
            consume(Token(TokenType::SEMICOLON, ";", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ';' after print statement")));
            return std::unique_ptr<Stmt<void>>(new StmtPrint<void>(expr));
        }

        std::unique_ptr<Stmt<void>> Parser::statement_if()
        {
            std::unique_ptr<StmtIf<void>> ret = std::make_unique<StmtIf<void>>();
            consume(Token(TokenType::LEFT_PAREN, "(", "", line), std::unique_ptr<ParserException>(new ParserException("Expected '(' after if")));
            auto expr = expression();
            consume(Token(TokenType::RIGHT_PAREN, ")", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ')' after if")));
            
            /// if stmt
            StmtIf<void>::Stmt* if_stmt= new StmtIf<void>::Stmt(std::move(expr));

            // block vs stmt
            if (peek() == Token(TokenType::LEFT_BRACE, "{", "", line)) {
                consume(Token(TokenType::LEFT_BRACE, "{", "", line), std::unique_ptr<ParserException>(new ParserException("Expected '{' after if block")));
                auto blk = statement_block();
                if_stmt->blk = std::unique_ptr<Block<void>>(dynamic_cast<Block<void>*>(blk.get()));
                if (!if_stmt->blk)
                    rift::error::report(line, "statement_if", "Expected block", peek(), ParserException("Expected block"));
            } else {
                auto stmt = ret_stmt();
                if_stmt->stmt = std::move(stmt);
            }
            ret->if_stmt = if_stmt;

            /// elif stmts
            if (peek() == Token(TokenType::ELIF, "elif", "elif", line)) {
                std::vector<StmtIf<void>::Stmt*> elif_stmts = {};

                while (consume(Token(TokenType::ELIF, "elif", "elif", line))) {
                    auto expr = expression();
                    StmtIf<void>::Stmt* curr = new StmtIf<void>::Stmt(std::move(expr));
                     // block vs stmt
                    if (peek() == Token(TokenType::LEFT_BRACE, "{", "", line)) {
                        consume(Token(TokenType::LEFT_BRACE, "{", "", line), std::unique_ptr<ParserException>(new ParserException("Expected '{' after elif block")));
                        auto blk = statement_block();
                        curr->blk = std::unique_ptr<Block<void>>(dynamic_cast<Block<void>*>(blk.get()));
                        if (!curr->blk)
                            rift::error::report(line, "statement_if", "Expected block", peek(), ParserException("Expected block"));
                    } else {
                        auto stmt = ret_stmt();
                        curr->stmt = std::move(stmt);
                    }
                    elif_stmts.push_back(curr);
                }

                ret->elif_stmts = elif_stmts;
            }


            /// else stmt
            if (consume(Token(TokenType::ELSE, "else", "else", line))) {
                StmtIf<void>::Stmt* else_stmt = new StmtIf<void>::Stmt();
                // block vs stmt
                if (peek() == Token(TokenType::LEFT_BRACE, "{", "", line)) {
                    consume(Token(TokenType::LEFT_BRACE, "{", "", line), std::unique_ptr<ParserException>(new ParserException("Expected '{' after else block")));
                    auto blk = statement_block();
                    else_stmt->blk = std::unique_ptr<Block<void>>(dynamic_cast<Block<void>*>(blk.get()));
                    if (!else_stmt->blk)
                        rift::error::report(line, "statement_if", "Expected block", peek(), ParserException("Expected block"));
                } else {
                    std::unique_ptr<Stmt<void>> stmt = ret_stmt();
                    else_stmt->stmt = std::move(stmt);
                }
                ret->else_stmt = else_stmt;
            }

            return std::make_unique<Stmt<void>>(std::move(ret));
        }

        std::unique_ptr<Stmt<void>> Parser::statement_block()
        {
            std::vector<std::unique_ptr<Decl<Token>>> decls = {};

            curr_env->addChild();
            while (!atEnd() && !peek(Token(TokenType::RIGHT_BRACE, "}", "", line))) {
                std::vector<std::unique_ptr<Decl<Token>>> inner = ret_decl();
                decls.insert(decls.end(), std::make_move_iterator(inner.begin()), std::make_move_iterator(inner.end()));
            }
            curr_env->removeChild();

            if (!match({Token(TokenType::RIGHT_BRACE, "}", "", line)})) 
                rift::error::report(line, "statement_block", "Expected '}' after block", peek(), ParserException("Expected '}' after block"));
            
            auto blk = std::make_unique<Block<void>>(decls);
            return std::make_unique<Stmt<void>>(blk.get());
        }

        std::unique_ptr<Stmt<void>> Parser::statement_return()
        {
            auto expr = expression();
            consume(Token(TokenType::SEMICOLON, ";", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ';' after return statement")));
            auto ret_stmt = std::make_unique<StmtReturn<void>>(std::move(expr));
            return std::make_unique<Stmt<void>>(std::move(ret_stmt));
        }

        std::unique_ptr<Stmt<void>> Parser::statement_for()
        {
            std::unique_ptr<For<void>> _for = std::make_unique<For<void>>();
            consume(Token(TokenType::LEFT_PAREN, "(", "", line), std::unique_ptr<ParserException>(new ParserException("Expected '(' after for")));

            // first ;
            if (match({Token(TokenType::VAR, "", "", line)})) {
                _for->decl = std::make_unique<Decl<Token>>();
                // _for->decl = declaration_variable(true);
            } else if (match({Token(TokenType::CONST, "", "", line)}))  {
                _for->decl = declaration_variable(false);
            } else if(peek(Token(TokenType::IDENTIFIER, "", "", line))) {
                _for->stmt_l = std::move(ret_stmt());
            }

            // second ;
            auto expr = expression();
            _for->expr = std::move(expr);
            consume(Token(TokenType::SEMICOLON, ";", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ';' after for second statement")));

            // third ;
            if(match({Token(TokenType::IDENTIFIER, "", "", line)}))
                _for->stmt_r = std::move(ret_stmt());
            consume(Token(TokenType::RIGHT_PAREN, ")", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ')' after for")));

            if (match({Token(TokenType::LEFT_BRACE, "{", "", line)})) {
                auto blk = statement_block();
                _for->blk = std::unique_ptr<Block<void>>(dynamic_cast<Block<void>*>(blk.get()));
                if (!_for->blk)
                    rift::error::report(line, "statement_for", "Expected block", peek(), ParserException("Expected block"));
            } else {
                _for->stmt_o = std::move(ret_stmt());
            }

            return std::make_unique<Stmt<void>>(_for.get());
        }

        ////////////////////////////////////////////////////////////////////////
        #pragma mark - Declarations Parsing
        ////////////////////////////////////////////////////////////////////////

        std::unique_ptr<Decl<Token>> Parser::declaration_statement()
        {
            auto stmt = ret_stmt();
            auto decl_stmt = std::make_unique<DeclStmt<Token>>(std::move(stmt));
            return std::make_unique<Decl<Token>>(stmt.get());
        }

        std::unique_ptr<Decl<Token>> Parser::declaration_variable(bool mut)
        {
            // make sure there is an identifier
            auto tok_t = mut ? TokenType::IDENTIFIER : TokenType::C_IDENTIFIER;
            if (!consume(Token(TokenType::IDENTIFIER, "", "", line)) && !consume(Token(TokenType::C_IDENTIFIER, "", "", line)))
                rift::error::report(line, "declaration_variable", "Expected variable name", peek(), ParserException("Expected variable name"));
            auto idt = peekPrev();

            // make sure the identifier is not already declared
            /// @note this is just a check, the actual declaration is done in the evaluator
            ///       this also checks if any outer block has already declared this variable
            if (curr_env->getEnv<Token>(castString(idt)).type != TokenType::NIL)
                rift::error::report(line, "declaration_variable", "🛑 Variable '" + castString(idt) + "' already declared at line: " + castNumberString(idt.line), idt, ParserException("Variable '" + castString(idt) + "' already declared"));

            if(peek() == Token(TokenType::EQUAL, "=", "", line)) {
                auto expr = assignment();
                consume(Token(TokenType::SEMICOLON, ";", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ';' after variable assignment")));
                idt.type = tok_t;

                auto asgn = dynamic_cast<Assign<Token>*>(expr.get());
                auto val = asgn->value.get();

                // lets do some RTTI checks
                // was the assignment a function? mut y = test();
                auto func = dynamic_cast<Call<Token>*>(val);
                if (func != NULL) {
                    auto val = curr_env->getEnv<Token>(func->name.lexeme); // value of "test" identifier
                    std::cout << "TEST[" << idt.lexeme << "," << val << "]" << std::endl;
                    curr_env->setEnv(idt.lexeme, val, false);
                } else {
                    // more checks and setEnv's...
                }
                // env::getInstance(true).setEnv(idt.lexeme, Token(tok_t, idt.lexeme, val, idt.line), mut);
                // tmp->value = std::unique_ptr<Expr>(val);
                // expr = std::unique_ptr<Expr>(tmp);
                auto decl_var = std::make_unique<DeclVar<Token>>(idt, std::move(expr));
                return std::make_unique<Decl<Token>>(decl_var.get());
            } else if (!mut) {
                rift::error::report(line, "declaration_variable", "🛑 Constants must be defined", idt, ParserException("Constants must be defined"));
            }

            if (!consume(Token(TokenType::IDENTIFIER, "", "", line)) && !consume(Token(TokenType::C_IDENTIFIER, "", "", line)))
                rift::error::report(line, "declaration_variable", "Expected variable name", peek(), ParserException("Expected variable name"));
            idt = peekPrev();

            consume(Token(TokenType::SEMICOLON, ";", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ';' after variable declaration")));

            std::unique_ptr<rift::ast::DeclVar<Token>> decl_var = std::make_unique<DeclVar<Token>>(new DeclVar<Token>(Token(tok_t, idt.lexeme, idt.literal, idt.line)));
            return std::make_unique<Decl<Token>>(decl_var.get());
        }

        std::unique_ptr<Decl<Token>> Parser::declaration_func() 
        {
            std::unique_ptr<DeclFunc<Token>> _func = std::make_unique<DeclFunc<Token>>();
            _func->func = function();
            if (_func->func->blk == nullptr) {
                consume(Token(TokenType::SEMICOLON, ";", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ';' after function declaration")));
            }
            return std::make_unique<Decl<Token>>(_func.get());
        }

        Program::vec_t Parser::ret_decl()
        {
            Program::vec_t decls = {};
            if (consume_va({Token(TokenType::VAR), Token(TokenType::CONST)}, nullptr) != Token()) {
                auto test = declaration_variable(peekPrev().type == TokenType::VAR);
                decls.emplace_back(std::move(test));
            } else if (match({Token(TokenType::FUN)})) {
                decls.emplace_back(declaration_func());
            } else {
                decls.emplace_back(declaration_statement());
            }

            // edge-case (maybe due to mis-design)
            if (peek() == TokenType::SEMICOLON) consume(TokenType::SEMICOLON);

            return decls;
        }

        ////////////////////////////////////////////////////////////////////////
        #pragma mark - Application
        ////////////////////////////////////////////////////////////////////////

        std::unique_ptr<Program> Parser::program()
        {
            Program::vec_t decls = {};

            while (!atEnd()) {
                auto inner = ret_decl();
                decls.insert(decls.end(), std::make_move_iterator(inner.begin()), std::make_move_iterator(inner.end()));
            }

            return std::unique_ptr<Program>(new Program(decls));
        }

        ////////////////////////////////////////////////////////////////////////
        # pragma mark - Utilities
        ////////////////////////////////////////////////////////////////////////

        std::unique_ptr<DeclFunc<Token>::Func> Parser::function()
        {
            std::unique_ptr<DeclFunc<Token>::Func> ret = std::make_unique<DeclFunc<Token>::Func>();
            auto idt = consume_va({Token(TokenType::IDENTIFIER), Token(TokenType::C_IDENTIFIER)}, std::unique_ptr<ParserException>(new ParserException("Expected function name")));
            ret->name = idt;
            ret->closure = new Environment(Environment::getInstance(true)); // might be useless since alloc done at runtime too

            consume(Token(TokenType::LEFT_PAREN, "(", "", line), std::unique_ptr<ParserException>(new ParserException("Expected '(' after function name")));
            ret->params = params();
            consume(Token(TokenType::RIGHT_PAREN, ")", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ')' after function params")));
            // give the params (usefull for the call operator)
            curr_env->setEnv(idt.lexeme, Token(TokenType::FUN, idt.lexeme, ret->params, idt.line), false);

            if(match({Token(TokenType::LEFT_BRACE, "{", "", line)})) {
                auto blk = dynamic_cast<Block<void>*>(statement_block().get());
                if (!blk)
                    rift::error::report(line, "function", "Expected block", peek(), ParserException("Expected block"));
                ret->blk = std::make_unique<Block<void>>(blk);
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