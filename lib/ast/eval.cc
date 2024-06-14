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

        void Eval::evaluate(const rift::ast::Expr::Expr<Token>& expr)
        {
            try {
                T val = expr.accept(*visitor);
                if (this->visitor->isNumber(val)) {
                    std::cout << this->visitor->castNumber(val) << std::endl;
                } else if (this->visitor->isString(val)) {
                    std::cout << this->visitor->castString(val) << std::endl;
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
            Token left = expr.left.get();
            Token right = expr.right.get();

            switch (expr.op.type) {
                /* arthimetic ops */
                case TokenType::MINUS:
                    if (!isNumber(left.literal) && !isNumber(right.literal))
                        throw std::runtime_error("Expected a number for '-' operator");
                    return castNumber(left.literal) - castNumber(right.literal);
                case TokenType::PLUS:
                    if ((!isNumber(left.literal) && !isNumber(right.literal)) && (!isString(left) && !isString(right)))
                        throw std::runtime_error("Expected a number or string for '+' operator");
                    if (isNumber(left.literal) && isNumber(right.literal)) return castNumber(left.literal) + castNumber(right.literal);
                    if (isString(left) && isString(right)) return castString(left) + castString(right);
                case TokenType::SLASH:
                    if (!isNumber(left.literal) && !isNumber(right.literal))
                        throw std::runtime_error("Expected a number for '-' operator");
                    return castNumber(left.literal) / castNumber(right.literal);
                case TokenType::STAR:
                    if (!isNumber(left.literal) && !isNumber(right.literal))
                        throw std::runtime_error("Expected a number for '*' operator");
                    return castNumber(left.literal) * castNumber(right.literal);

                /* comparison ops */
                case TokenType::GREATER:
                    if (!isNumber(left.literal) && !isNumber(right.literal))
                        throw std::runtime_error("Expected a number for '>' operator");
                    return castNumber(left.literal) > castNumber(right.literal);
                case TokenType::GREATER_EQUAL:
                    if (!isNumber(left.literal) && !isNumber(right.literal))
                        throw std::runtime_error("Expected a number for '>=' operator");
                    return castNumber(left.literal) >= castNumber(right.literal);
                case TokenType::LESS:
                    if (!isNumber(left.literal) && !isNumber(right.literal))
                        throw std::runtime_error("Expected a number for '<' operator");
                    return castNumber(left.literal) < castNumber(right.literal);
                case TokenType::LESS_EQUAL:
                    if (!isNumber(left.literal) && !isNumber(right.literal))
                        throw std::runtime_error("Expected a number for '<=' operator");
                    return castNumber(left.literal) <= castNumber(right.literal);
                case TokenType::BANG_EQUAL:
                    return !equal(left, right);
                case TokenType::EQUAL_EQUAL:
                    return equal(left, right);
                
                default:
                    throw std::runtime_error("Unknown operator for a binary expression");
            }
        }

        Token EvalVisitor::visit_grouping(const GroupingExpr& expr) const
        {
            return expr.accept(*this);
        }

        Token EvalVisitor::visit_literal(const LiteralExpr& expr) const
        {
            return expr.value;
        }

        Token EvalVisitor::visit_unary(const UnaryExpr& expr) const
        {
            T right = expr.expr.get();

            switch (expr.op.type) {
                case TokenType::MINUS:
                    if (!isNumber(right))
                        throw std::runtime_error("Expected a number after '-' operator");
                    return -castNumber(right);
                case TokenType::BANG:
                    return !truthy(right);
                default:
                    throw std::runtime_error("Unknown operator for a unary expression");
            }
        }

        #pragma mark - Helpers

        bool EvalVisitor::truthy(any val) const {
            if (val.type() == typeid(bool)) {
                return std::any_cast<bool>(val);
            }
            return true;
        }

        bool EvalVisitor::equal(any left, any right) const {
            if (left.type() != right.type()) return false;
            else if (left.type() == typeid(bool)) return std::any_cast<bool>(left) == std::any_cast<bool>(right);
            else if (left.type() == typeid(std::string)) return std::any_cast<std::string>(left) == std::any_cast<std::string>(right);
            else if (left.type() == typeid(double)) return std::any_cast<double>(left) == std::any_cast<double>(right);
            return false;
        }

        bool EvalVisitor::isNumber(any val) const {
            return val.type() == typeid(double) || val.type() == typeid(float) || 
                    val.type() == typeid(int) || val.type() == typeid(long) ||
                    val.type() == typeid(unsigned) || val.type() == typeid(short) || 
                    val.type() == typeid(unsigned long) || val.type() == typeid(unsigned short) ||
                    val.type() == typeid(unsigned long long) || val.type() == typeid(long long);
        }

        bool EvalVisitor::isString(any val) const {
            return val.type() == typeid(std::string) || val.type() == typeid(char*) || val.type() == typeid(const char*) ||
                    val.type() == typeid(char) || val.type() == typeid(unsigned char) || val.type() == typeid(signed char);
        }

        double EvalVisitor::castNumber(any val) const {
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

        std::string EvalVisitor::castString(any val) const {
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