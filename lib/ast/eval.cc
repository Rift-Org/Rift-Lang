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

#include <ast/grmr.hh>
#include <ast/eval.hh>
#include <error/error.hh>
#include <utils/macros.hh>
#include <ast/env.hh>

namespace rift
{
    namespace ast
    {
        
        #pragma mark - Static Variables

        static Token return_token = Token(TokenType::NIL, "", "", -1);
        static Environment* curr_env = &rift::ast::Environment::getInstance(false);
        static std::unordered_map<Expr<Token>*, int> locals = {};


        #pragma mark - Eval

        Token Eval::lookup(Expr<Token>* expr, std::string key)
        {
            if (locals.find(expr) != locals.end()) {
                auto depth = locals[expr];
                return curr_env->at(depth)->getEnv<Token>(key);
            } else {
                // return curr_env->getEnv<Token>(key);
                return Environment::getInstance(false).getEnv<Token>(key);
            }   
        }

        void Eval::resolve(Expr<Token>* expr, int depth)
        {
            locals[expr] = depth;
        }

        std::vector<string> Eval::evaluate(std::unique_ptr<Program<Tokens>>& prgm, bool interactive)
        {
            std::vector<std::string> res;

            try {
                auto toks = prgm->accept(*this);
                for (const auto& tok : toks) {
                    any val = tok.getLiteral();
                    if (isNumber(tok)) {
                        res.push_back(castNumberString(tok));
                    } else if (isString(tok)) {
                        res.push_back(castString(tok));
                    } else if (val.type() == typeid(bool)) {
                        res.push_back(std::any_cast<bool>(val) ? "true" : "false");
                    } else if (val.type() == typeid(std::nullptr_t)) {
                        res.push_back("null");
                    } else {
                        res.push_back("undefined");
                    }
                }
            } catch (const std::runtime_error& e) {
                error::runTimeError(e.what());
            }

            return res;
        }

        #pragma mark - Eval Visitor

        Token Eval::visit_literal(const Literal<Token>& expr) const
        {
            Token val = expr.value;
            any literal = val.getLiteral();

            if (literal.type() == typeid(std::string)) 
                return Token(TokenType::STRINGLITERAL, std::any_cast<std::string>(literal), 0, expr.value.line);

            /* Numeric Literals */
            else if (literal.type() == typeid(double))
                return Token(TokenType::NUMERICLITERAL, std::to_string(std::any_cast<double>(literal)), std::any_cast<double>(literal), expr.value.line);
            else if (literal.type() == typeid(int))
                return Token(TokenType::NUMERICLITERAL, std::to_string(std::any_cast<int>(literal)), std::any_cast<int>(literal), expr.value.line);
            else if (literal.type() == typeid(unsigned))
                return Token(TokenType::NUMERICLITERAL, std::to_string(std::any_cast<unsigned>(literal)), std::any_cast<unsigned>(literal), expr.value.line);
            else if (literal.type() == typeid(short))
                return Token(TokenType::NUMERICLITERAL, std::to_string(std::any_cast<short>(literal)), std::any_cast<short>(literal), expr.value.line);
            else if (literal.type() == typeid(unsigned long))
                return Token(TokenType::NUMERICLITERAL, std::to_string(std::any_cast<unsigned long>(literal)), std::any_cast<unsigned long>(literal), expr.value.line);
            else if (literal.type() == typeid(unsigned short))
                return Token(TokenType::NUMERICLITERAL, std::to_string(std::any_cast<unsigned short>(literal)), std::any_cast<unsigned short>(literal), expr.value.line);
            else if (literal.type() == typeid(unsigned long long))
                return Token(TokenType::NUMERICLITERAL, std::to_string(std::any_cast<unsigned long long>(literal)), std::any_cast<unsigned long long>(literal), expr.value.line);
            else if (literal.type() == typeid(long long))
                return Token(TokenType::NUMERICLITERAL, std::to_string(std::any_cast<long long>(literal)), std::any_cast<long long>(literal), expr.value.line);
            
            /* Other Literals */
            else if (literal.type() == typeid(std::nullptr_t))
                return Token(TokenType::NIL, "nil", nullptr, expr.value.line);
            else if (literal.type() == typeid(bool))
                return Token(std::any_cast<bool>(literal)?TokenType::TRUE:TokenType::FALSE, std::to_string(std::any_cast<bool>(literal)), std::any_cast<bool>(literal), expr.value.line);
            else if (literal.type() == typeid(std::nullptr_t))
                return Token(TokenType::NIL, "nil", nullptr, expr.value.line);

            /* Function */
            // else if (res.type == TokenType::FUN)
            //     return Token(TokenType::FUN, val.lexeme, literal, expr.value.line);
            else
                rift::error::runTimeError("Unknown literal type");
            return Token();
        }

        Token Eval::visit_var_expr(const VarExpr<Token>& expr) const
        {
            Token val = expr.value;
            any literal;

            // Token res = curr_env->getEnv<Token>(val.lexeme);
            VarExpr<Token>& expr_ref = const_cast<VarExpr<Token>&>(expr);
            return Eval::lookup(&expr_ref, val.lexeme);
        }

        Token Eval::visit_binary(const Binary<Token>& expr) const
        {
            Token left;
            Token right;

            string l_s, r_s;
            any resAny;
            bool resBool;

            // Operators that can't evaulate yet
            switch (expr.op.type) {
                case NULLISH_COAL:
                    left = expr.left.get()->accept(*this);
                    if (left.type == TokenType::NIL) {
                        right = expr.right.get()->accept(*this);
                        return right;
                    }
                    return left;
                case LOG_AND:
                    left = expr.left.get()->accept(*this);
                    if(truthy(left)) {
                        if(truthy(expr.right.get()->accept(*this)))
                            return Token(TokenType::TRUE, "true", "true", expr.op.line);
                        return Token(TokenType::FALSE, "false", "false", expr.op.line);
                    }
                    return Token();
                case LOG_OR:
                    left = expr.left.get()->accept(*this);
                    if (truthy(left)) {
                        right = expr.right.get()->accept(*this);
                        return Token(TokenType::TRUE, "true", "true", expr.op.line);
                    } else {
                        right = expr.left.get()->accept(*this);
                        if (truthy(right)) return Token(TokenType::TRUE, "true", "true", expr.op.line);
                        return Token(TokenType::FALSE, "false", "false", expr.op.line);
                    }
                default:
                    break;
            }

            left = expr.left.get()->accept(*this);
            right = expr.right.get()->accept(*this);

            // Operators which depend on evaluation of both
            switch (expr.op.type) {
                /* arthimetic ops */
                case TokenType::MINUS:
                    if (!isNumber(left) && !isNumber(right))
                        rift::error::runTimeError("Expected a number for '-' operator");
                    resAny = any_arithmetic(left, right, expr.op);
                    return Token(TokenType::NUMERICLITERAL, castNumberString(resAny), resAny, expr.op.line);
                case TokenType::PLUS:
                    if ((!isNumber(left) && !isNumber(right)) && (!isString(left) && !isString(right)))
                        rift::error::runTimeError("Expected a number or string for '+' operator");
                    if (isNumber(left) && isNumber(right)) {
                        resAny = any_arithmetic(left, right, expr.op);
                        return Token(TokenType::NUMERICLITERAL, castNumberString(resAny), resAny, expr.op.line);
                    } else if (isString(left) && isString(right)) {
                        l_s = castString(left), r_s = castString(right);
                        if (l_s[0] == '"' && l_s[l_s.size()-1] == '"') l_s = l_s.substr(1, l_s.size()-2);
                        if (r_s[0] == '"' && r_s[r_s.size()-1] == '"') r_s = r_s.substr(1, r_s.size()-2);
                        return Token(TokenType::STRINGLITERAL, l_s + r_s, 0, expr.op.line);
                    } else if (isString(left) && isNumber(right)) {
                        return Token(TokenType::STRINGLITERAL, castString(left) + castNumberString(right), 0, expr.op.line);
                    } else if (isNumber(left) && isString(right)) {
                        return Token(TokenType::STRINGLITERAL, castNumberString(left) + castString(right), 0, expr.op.line);
                    }
                    rift::error::runTimeError("Expected a number or string for '+' operator");
                case TokenType::SLASH:
                    if (!isNumber(left) && !isNumber(right))
                        rift::error::runTimeError("Expected a number for '-' operator");
                    resAny = any_arithmetic(left, right, expr.op);
                    return Token(TokenType::NUMERICLITERAL, castNumberString(resAny), resAny, expr.op.line);
                case TokenType::STAR:
                    if (!isNumber(left) && !isNumber(right))
                        rift::error::runTimeError("Expected a number for '*' operator");
                    resAny = any_arithmetic(left, right, expr.op);
                    return Token(TokenType::NUMERICLITERAL, castNumberString(resAny), resAny, expr.op.line);
                /* comparison ops */
                case TokenType::GREATER:
                    if(strcmp(castString(left).c_str(),castString(right).c_str())>0)
                        return Token(TokenType::TRUE, "true", "true", expr.op.line);
                    return Token(TokenType::FALSE, "false", "false", expr.op.line);
                    rift::error::runTimeError("Expected a number or string for '>' operator");
                case TokenType::GREATER_EQUAL:
                    _BOOL_LOGIC(expr.op);
                    rift::error::runTimeError("Expected a number or string for '>=' operator");
                case TokenType::LESS:
                    _BOOL_LOGIC(expr.op);
                    rift::error::runTimeError("Expected a number or string for '<' operator");
                case TokenType::LESS_EQUAL:
                    _BOOL_LOGIC(expr.op);
                    rift::error::runTimeError("Expected a number or string for '<=' operator");
                case TokenType::BANG_EQUAL:
                    _BOOL_LOGIC(expr.op);
                    rift::error::runTimeError("Expected a number or string for '!=' operator");
                case TokenType::EQUAL_EQUAL:
                    _BOOL_LOGIC(expr.op);
                    rift::error::runTimeError("Expected a number or string for '==' operator");
                default:
                    rift::error::runTimeError("Unknown operator for a binary expression");
            }

            return Token();
        }

        Token Eval::visit_assign(const Assign<Token>& expr) const
        {
            auto name = expr.name.lexeme;
            auto val = expr.value->accept(*this);

            const Expr<Token>* const_expr = &expr;
            Expr<Token>* expr_ptr = const_cast<Expr<Token>*>(const_expr);
            
            if (locals.find(expr_ptr) != locals.end()) {
                auto depth = locals[expr_ptr];
                curr_env->at(depth)->setEnv<Token>(expr.name.lexeme, val, false);
            } else {
                // curr_env->setEnv<Token>(expr.name.lexeme, val, false);
                // couldnt find it in locals so must be in globals
                Environment::getInstance(false).setEnv<Token>(expr.name.lexeme, val, false);
            }   

            return val;
        }

        Token Eval::visit_grouping(const Grouping<Token>& expr) const
        {
            return expr.expr.get()->accept(*this);
        }

        Token Eval::visit_unary(const Unary<Token>& expr) const
        {
            Token right = expr.expr.get()->accept(*this);
            bool res = false;
            any resAny;
 
            switch (expr.op.type) {
                case TokenType::MINUS:
                    if (!isNumber(right))
                        rift::error::runTimeError("Expected a number after '-' operator");

                resAny = any_arithmetic(right, Token(TokenType::NUMERICLITERAL, "-1", -1, expr.op.line), Token(TokenType::STAR, "-", "", expr.op.line));
                return Token(TokenType::NUMERICLITERAL, castNumberString(resAny), resAny, expr.op.line);

                case TokenType::BANG:
                    if (right.type == TokenType::TRUE || right.type == TokenType::FALSE)
                        res = truthy(right);
                    else if (isNumber(right))
                        res = std::any_cast<bool>(any_arithmetic(right, Token(TokenType::NUMERICLITERAL, "0", 0, expr.op.line), Token(TokenType::EQUAL_EQUAL, "==", "", expr.op.line)));
                    else if (isString(right))
                        res = castString(right).empty();
                    else
                        rift::error::runTimeError("Expected a number or string after '!' operator");
                    return Token(res?TokenType::TRUE:TokenType::FALSE, std::to_string(!res), !res, expr.op.line);
                default:
                    rift::error::runTimeError("Unknown operator for a unary expression");
            }
            return Token();
        }

        Token Eval::visit_ternary(const Ternary<Token>& expr) const
        {
            Token cond = expr.condition->accept(*this);

            if(truthy(cond)) 
                return Token(expr.left->accept(*this));
            return Token(expr.right->accept(*this));
        }

        Token Eval::visit_call(const Call<Token>& expr) const
        {
            auto name = curr_env->getEnv<Token>(expr.name.lexeme);

            if (name.type == TokenType::NIL)
                rift::error::runTimeError("Undefined function '" + name.lexeme + "'");

            // set new env with closure
            auto func = std::any_cast<DeclFunc<Token>::Func*>(name.literal);
            // curr_env = new Environment(func->closure);


            // map arguments to parameters
            for (const auto& arg : expr.args) {
                curr_env->setEnv<Token>(arg.first, arg.second->accept(*this), false);
            }

            func->blk->accept(*this);

            // cleanup
            auto tmp = return_token;
            return_token = Token(TokenType::NIL, "", "", -1);
            // delete curr_env;
            // curr_env = &rift::ast::Environment::getInstance(false);

            return tmp;
        }

        #pragma mark - Stmt Visitors

        void Eval::visit_expr_stmt(const StmtExpr<void>& stmt) const
        {
            stmt.expr->accept(*this);
        }

        void Eval::visit_print_stmt(const StmtPrint<void>& stmt) const
        {
            Token val = stmt.expr->accept(*this);
            std::string res = castAnyString(val);
            if (res.at(0) == '"' && res.at(res.size()-1) == '"') 
                res = res.substr(1, res.size()-2);
            std::cout << res << std::endl;
            // return val;
        }

        void Eval::visit_if_stmt(const StmtIf<void>& stmt) const
        {
            auto if_stmt = stmt.if_stmt;
            // if stmt
            // auto expr = std::move(if_stmt->expr);
            // if (expr == nullptr) rift::error::runTimeError("If statement expression should not be null");
            auto expr_tok = if_stmt->expr->accept(*this);

            if (truthy(expr_tok)) {
                if (if_stmt->blk != nullptr) if_stmt->blk->accept(*this);
                else if (if_stmt->stmt != nullptr) if_stmt->stmt->accept(*this);
                else rift::error::runTimeError("If statement should have a statement or block");
            }

            // elif stmt
            auto elif_stmts = stmt.elif_stmts;
            for (const auto& elif_stmt : elif_stmts) {
                if(elif_stmt->expr == nullptr) rift::error::runTimeError("Elif statement expression should not be null");
                if(truthy(elif_stmt->expr->accept(*this))) {
                    if (elif_stmt->blk != nullptr) elif_stmt->blk->accept(*this);
                    else if (elif_stmt->stmt != nullptr) elif_stmt->stmt->accept(*this);
                    else rift::error::runTimeError("Elif statement should have a statement or block");
                }
            }

            // else stmt
            auto else_stmt = stmt.else_stmt;
            if(else_stmt != nullptr) {
                if (else_stmt->blk != nullptr) else_stmt->blk->accept(*this);
                else if (else_stmt->stmt != nullptr) else_stmt->stmt->accept(*this);
                else rift::error::runTimeError("Else statement should have a statement or block");
            }
        }

        void Eval::visit_return_stmt(const StmtReturn<void>& stmt) const
        {
            return_token = stmt.expr->accept(*this);
            // return return_token;
        }

        #pragma mark - Program / Block Visitor

        void Eval::visit_block_stmt(const Block<void>& block) const
        {
            Tokens toks = {};

            curr_env->addChild(); // add scope
            for (auto it=block.decls.begin(); it!=block.decls.end(); it++) {
                    if (return_token.line != -1) break; // -1 = no return
                    toks.push_back((*it)->accept(*this));
            }
            curr_env->removeChild(); // remove scope

            // return return_token;
        }

        void Eval::visit_for_stmt(const For<void>& decl) const
        {
            Tokens toks = {};
            if (decl.decl != nullptr) decl.decl->accept(*this);
            else if (decl.stmt_l != nullptr) decl.stmt_l->accept(*this);

            while(truthy(decl.expr->accept(*this))) {
                if(decl.stmt_o != nullptr) decl.stmt_o->accept(*this);
                else if (decl.blk != nullptr) decl.blk->accept(*this);
                else rift::error::runTimeError("For statement should have a statement or block");

                if (decl.stmt_r != nullptr) decl.stmt_r->accept(*this);
            }
            // return return_token;
        }

        #pragma mark - Decl Visitors

        Token rift::ast::Eval::visit_decl_stmt(const rift::ast::DeclStmt<Token> &decl) const
        {
            decl.stmt->accept(*this);
            return Token();
        }

        Token Eval::visit_decl_var(const DeclVar<Token>& decl) const
        {
            // check performed in parser, undefined variables are CT errors
            if (decl.expr != nullptr) {
                return decl.expr->accept(*this);
            } else {
                // declaration just set it to a nil token
                curr_env->setEnv<Token>(decl.identifier.lexeme, Token(), decl.identifier.type == TokenType::C_IDENTIFIER);
                return Token();
            }
        }

        Token Eval::visit_decl_func(const DeclFunc<Token>& decl) const
        {
            auto name = decl.func->name;
            auto params = decl.func->params;
            decl.func->closure = new Environment(Environment::getInstance(false)); // grab a copy of global env

            // quick check
            if (curr_env->getEnv<Token>(name.lexeme).type != TokenType::NIL)
                rift::error::runTimeError("Function '" + name.lexeme + "' already defined");

            if (decl.func->blk) {
                curr_env->setEnv<Token>(name.lexeme, Token(TokenType::FUN, name.lexeme, decl.func.get(), name.line), false);
            } else {
                // rift::error::runTimeError("Function '" + name.lexeme + "' should have a block (no support for lambdas yet)");
                // this is just a declaration for now, will add stmt when support fat arrow lambdas
                curr_env->setEnv<Token>(name.lexeme, Token(TokenType::NIL, "null", nullptr, name.line), false);
            }
            return {name};
        }

        #pragma mark - Program

        Tokens Eval::visit_program(const Program<Tokens>& prgm) const
        {
            Tokens toks = {};
            for (auto it=prgm.decls.begin(); it!=prgm.decls.end(); it++) {
                toks.push_back((*it)->accept(*this));
            }
            return toks;
        }
    }
}