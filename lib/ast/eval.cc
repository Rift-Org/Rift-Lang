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

using env = rift::ast::Environment;

namespace rift
{
    namespace ast
    {
        #pragma mark - Eval

        Eval::Eval()
        {
            this->visitor = std::unique_ptr<Visitor>(new Visitor());
        }

        std::vector<std::string> Eval::evaluate(const Program& expr)
        {
            std::vector<std::string> res;

            try {
                auto toks = expr.accept(*visitor.get());
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

        Token Visitor::visit_assign(const Assign& expr) const
        {
            auto name = castString(expr.name);
            env::getInstance().setEnv(name, expr.value->accept(*this));
            return env::getInstance().getEnv(name);;
        }

        Token Visitor::visit_binary(const Binary& expr) const
        {
            Token left = expr.left.get()->accept(*this);
            Token right = expr.right.get()->accept(*this);
            string l_s, r_s;
            any resAny;
            bool resBool;

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
                    _BOOL_LOGIC(expr.op);
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

        Token Visitor::visit_grouping(const Grouping& expr) const
        {
            return expr.expr.get()->accept(*this);
        }

        Token Visitor::visit_literal(const Literal& expr) const
        {
            Token val = expr.value;
            any literal;

            if (val.type == TokenType::IDENTIFIER) {
                Token res = env::getInstance().getEnv(castString(val));
                if (res.type == TokenType::NIL) rift::error::runTimeError("Undefined variable '" + castString(val) + "'");
                literal = res.getLiteral();
            } else {
                literal = val.getLiteral();
            }

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
                return Token(TokenType::NIL, "null", nullptr, expr.value.line);
            else if (literal.type() == typeid(bool))
                return Token(std::any_cast<bool>(literal)?TokenType::TRUE:TokenType::FALSE, std::to_string(std::any_cast<bool>(literal)), std::any_cast<bool>(literal), expr.value.line);
            else if (literal.type() == typeid(std::nullptr_t))
                return Token(TokenType::NIL, "null", nullptr, expr.value.line);
            else
                rift::error::runTimeError("Unknown literal type");
            return Token();
        }

        Token Visitor::visit_unary(const Unary& expr) const
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

        #pragma mark - Stmt Visitors

        Token Visitor::visit_expr_stmt(const StmtExpr& stmt) const
        {
            return stmt.expr->accept(*this);
        }

        Token Visitor::visit_print_stmt(const StmtPrint& stmt) const
        {
            Token val = stmt.expr->accept(*this);
            std::string res = castAnyString(val);
            if (res.at(0) == '"' && res.at(res.size()-1) == '"') 
                res = res.substr(1, res.size()-2);
            std::cout << res << std::endl;
            return val;
        }

        #pragma mark - Program / Block Visitor

        Tokens Visitor::visit_block_stmt(const Block& block) const
        {
            Tokens toks = {};
            env::getInstance().addChild(); // add scope
            for (auto it=block.decls->begin(); it!=block.decls->end(); it++) {
                auto its = (*it)->accept(*this);
                toks.insert(toks.end(), its.begin(), its.end());
            }
            env::getInstance().removeChild(); // remove scope

            return toks;
        }

        Tokens Visitor::visit_program(const Program& prgm) const
        {
            Tokens toks = {};
            for (auto it=prgm.decls->begin(); it!=prgm.decls->end(); it++) {
                auto its = (*it)->accept(*this);
                toks.insert(toks.end(), its.begin(), its.end());
            }
            return toks;
        }

        #pragma mark - Decl Visitors

        Tokens Visitor::visit_decl_stmt(const DeclStmt& decl) const
        {
            return {decl.stmt->accept(*this)};
        }

        Tokens Visitor::visit_decl_var(const DeclVar& decl) const
        {
            // check performed in parser, undefined variables are CT errors
            return {decl.expr->accept(*this)};
        }
    }
}