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

#include <ast/eval.hh>
#include <error/error.hh>

namespace rift
{
    namespace ast
    {
        #pragma mark - Eval

        Eval::Eval()
        {
            visitor = new EvalVisitor(*this);
        }

        Eval::~Eval()
        {
            delete visitor;
        }

        EvalVisitor::EvalVisitor(Eval& eval)
        {
            this->eval = &eval;
        }

        EvalVisitor::~EvalVisitor()
        {
            this->eval = nullptr;
        }

        void Eval::evaluate(const rift::ast::Expr::Expr<Token>& expr)
        {
            try {
                Token tok = expr.accept(*visitor);
                any val = tok.getLiteral();
                if (this->visitor->isNumber(tok)) {
                    std::cout << this->visitor->castNumber(tok) << std::endl;
                } else if (this->visitor->isString(tok)) {
                    std::cout << this->visitor->castString(tok) << std::endl;
                } else if (val.type() == typeid(bool)) {
                    std::cout << std::any_cast<bool>(val) << std::endl;
                } else if (val.type() == typeid(std::nullptr_t)) {
                    std::cout << "null" << std::endl;
                } else {
                    std::cout << "undefined" << std::endl;
                }
            } catch (const std::runtime_error& e) {
                error::runTimeError(e.what());
            }
        }

        #pragma mark - Eval Visitor

        Token EvalVisitor::visit_binary(const BinaryExpr& expr) const
        {
            Token left = expr.left.get()->accept(*this);
            Token right = expr.right.get()->accept(*this);
            double resDouble;
            bool resBool;

            switch (expr.op.type) {
                /* arthimetic ops */
                case TokenType::MINUS:
                    if (!isNumber(left) && !isNumber(right))
                        throw std::runtime_error("Expected a number for '-' operator");
                    resDouble = castNumber(left) - castNumber(right);
                    return Token(TokenType::NUMERICLITERAL, std::to_string(resDouble), resDouble, expr.op.line);
                case TokenType::PLUS:
                    if ((!isNumber(left) && !isNumber(right)) && (!isString(left) && !isString(right)))
                        throw std::runtime_error("Expected a number or string for '+' operator");
                    if (isNumber(left) && isNumber(right)) {
                        resDouble = castNumber(left) + castNumber(right);
                        return Token(TokenType::NUMERICLITERAL, std::to_string(resDouble), resDouble, expr.op.line);
                    } else if (isString(left) && isString(right)) {
                        return Token(TokenType::STRINGLITERAL, castString(left) + castString(right), 0, expr.op.line);
                    } else if (isString(left) && isNumber(right)) {
                        return Token(TokenType::STRINGLITERAL, castString(left) + std::to_string(castNumber(right)), 0, expr.op.line);
                    } else if (isNumber(left) && isString(right)) {
                        return Token(TokenType::STRINGLITERAL, std::to_string(castNumber(left)) + castString(right), 0, expr.op.line);
                    }
                    throw std::runtime_error("Expected a number or string for '+' operator");
                case TokenType::SLASH:
                    if (!isNumber(left) && !isNumber(right))
                        throw std::runtime_error("Expected a number for '-' operator");
                    resDouble = castNumber(left) / castNumber(right);
                    return Token(TokenType::NUMERICLITERAL, std::to_string(resDouble), resDouble, expr.op.line);
                case TokenType::STAR:
                    if (!isNumber(left) && !isNumber(right))
                        throw std::runtime_error("Expected a number for '*' operator");
                    resDouble = castNumber(left) * castNumber(right);
                    return Token(TokenType::NUMERICLITERAL, std::to_string(resDouble), resDouble, expr.op.line);

                /* comparison ops */
                case TokenType::GREATER:
                    if (!isNumber(left) && !isNumber(right))
                        throw std::runtime_error("Expected a number for '>' operator");
                    resBool = castNumber(left) > castNumber(right);
                    return Token(resBool?TokenType::TRUE:TokenType::FALSE, std::to_string(resBool), resBool, expr.op.line);
                case TokenType::GREATER_EQUAL:
                    if (!isNumber(left) && !isNumber(right))
                        throw std::runtime_error("Expected a number for '>=' operator");
                    resBool = castNumber(left) >= castNumber(right);
                    return Token(resBool?TokenType::TRUE:TokenType::FALSE, std::to_string(resBool), resBool, expr.op.line);
                case TokenType::LESS:
                    if (!isNumber(left) && !isNumber(right))
                        throw std::runtime_error("Expected a number for '<' operator");
                    resBool = castNumber(left) < castNumber(right);
                    return Token(resBool?TokenType::TRUE:TokenType::FALSE, std::to_string(resBool), resBool, expr.op.line);
                case TokenType::LESS_EQUAL:
                    if (!isNumber(left) && !isNumber(right))
                        throw std::runtime_error("Expected a number for '<=' operator");
                    resBool = castNumber(left) <= castNumber(right);
                    return Token(resBool?TokenType::TRUE:TokenType::FALSE, std::to_string(resBool), resBool, expr.op.line);
                case TokenType::BANG_EQUAL:
                    resBool = !equal(left, right);
                    return Token(resBool?TokenType::TRUE:TokenType::FALSE, std::to_string(resBool), resBool, expr.op.line);
                case TokenType::EQUAL_EQUAL:
                    resBool = equal(left, right);
                    return Token(resBool?TokenType::TRUE:TokenType::FALSE, std::to_string(resBool), resBool, expr.op.line);
                default:
                    throw std::runtime_error("Unknown operator for a binary expression");
            }
        }

        Token EvalVisitor::visit_grouping(const GroupingExpr& expr) const
        {
            return expr.expr.get()->accept(*this);
        }

        Token EvalVisitor::visit_literal(const LiteralExpr& expr) const
        {
            Token val = expr.value;
            any literal = val.getLiteral();

            if (literal.type() == typeid(std::string)) 
                return Token(TokenType::STRINGLITERAL, std::any_cast<std::string>(literal), 0, expr.value.line);
            else if (literal.type() == typeid(double))

            /* Numeric Literals */
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
                throw std::runtime_error("Unknown literal type");
        }

        Token EvalVisitor::visit_unary(const UnaryExpr& expr) const
        {
            Token right = expr.expr.get()->accept(*this);
            bool res;
 
            switch (expr.op.type) {
                case TokenType::MINUS:
                    if (!isNumber(right))
                        throw std::runtime_error("Expected a number after '-' operator");
                    res = castNumber(right);
                    return Token(TokenType::NUMERICLITERAL, std::to_string(-res), res, expr.op.line);
                case TokenType::BANG:
                res = truthy(right);
                return Token(res?TokenType::TRUE:TokenType::FALSE, std::to_string(!res), !res, expr.op.line);
                default:
                    throw std::runtime_error("Unknown operator for a unary expression");
            }
        }

        #pragma mark - Helpers

        bool EvalVisitor::truthy(Token tok) const {
            any val = tok.getLiteral();
            if (val.type() == typeid(bool)) {
                return std::any_cast<bool>(val);
            }
            return true;
        }

        bool EvalVisitor::equal(Token tl, Token tr) const {
            any left = tl.getLiteral();
            any right = tr.getLiteral();
            if (left.type() != right.type()) return false;
            else if (left.type() == typeid(bool)) return std::any_cast<bool>(left) == std::any_cast<bool>(right);
            else if (left.type() == typeid(std::string)) return std::any_cast<std::string>(left) == std::any_cast<std::string>(right);
            else if (left.type() == typeid(double)) return std::any_cast<double>(left) == std::any_cast<double>(right);
            return false;
        }

        bool EvalVisitor::isNumber(Token tok) const {
            any val = tok.getLiteral();
            return val.type() == typeid(double) || val.type() == typeid(float) || 
                    val.type() == typeid(int) || val.type() == typeid(long) ||
                    val.type() == typeid(unsigned) || val.type() == typeid(short) || 
                    val.type() == typeid(unsigned long) || val.type() == typeid(unsigned short) ||
                    val.type() == typeid(unsigned long long) || val.type() == typeid(long long);
        }

        bool EvalVisitor::isString(Token tok) const {
            any val = tok.getLiteral();
            return val.type() == typeid(std::string) || val.type() == typeid(char*) || val.type() == typeid(const char*) ||
                    val.type() == typeid(char) || val.type() == typeid(unsigned char) || val.type() == typeid(signed char);
        }

        double EvalVisitor::castNumber(Token tok) const {
            any val = tok.getLiteral();
            if (val.type() == typeid(double)) {
                return std::any_cast<double>(val);
            } else if (val.type() == typeid(float)) {
                return static_cast<double>(std::any_cast<float>(val));
            } else if (val.type() == typeid(int)) {
                return static_cast<double>(std::any_cast<int>(val));
            } else if (val.type() == typeid(long)) {
                return static_cast<double>(std::any_cast<long>(val));
            } else if (val.type() == typeid(unsigned)) {
                return static_cast<double>(std::any_cast<unsigned>(val));
            } else if (val.type() == typeid(short)) {
                return static_cast<double>(std::any_cast<short>(val));
            } else if (val.type() == typeid(unsigned long)) {
                return static_cast<double>(std::any_cast<unsigned long>(val));
            } else if (val.type() == typeid(unsigned short)) {
                return static_cast<double>(std::any_cast<unsigned short>(val));
            } else if (val.type() == typeid(unsigned long long)) {
                return static_cast<double>(std::any_cast<unsigned long long>(val));
            } else if (val.type() == typeid(long long)) {
                return static_cast<double>(std::any_cast<long long>(val));
            } 

            throw std::runtime_error("Expected a number");
        }

        std::string EvalVisitor::castString(Token tok) const {
            any val = tok.getLiteral();
            if (val.type() == typeid(std::string)) {
                return std::any_cast<std::string>(val);
            } else if (val.type() == typeid(char*)) {
                return std::string(std::any_cast<char*>(val));
            } else if (val.type() == typeid(const char*)) {
                return std::string(std::any_cast<const char*>(val));
            } else if (val.type() == typeid(char)) {
                return std::string(1, std::any_cast<char>(val));
            } else if (val.type() == typeid(unsigned char)) {
                return std::string(1, std::any_cast<unsigned char>(val));
            } else if (val.type() == typeid(signed char)) {
                return std::string(1, std::any_cast<signed char>(val));
            }

            throw std::runtime_error("Expected a string");
        }
    }
}