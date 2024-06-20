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
#include <utils/macros.hh>

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

        std::string Eval::evaluate(const rift::ast::Expr::Expr<Token>& expr)
        {
            std::string res;

            try {
                Token tok = expr.accept(*visitor);
                any val = tok.getLiteral();
                if (this->visitor->isNumber(tok)) {
                    res = this->visitor->castNumberString(tok);
                } else if (this->visitor->isString(tok)) {
                    res = this->visitor->castString(tok);
                } else if (val.type() == typeid(bool)) {
                    res = std::any_cast<bool>(val) ? "true" : "false";
                } else if (val.type() == typeid(std::nullptr_t)) {
                    res = "null";
                } else {
                    res = "undefined";
                }
            } catch (const std::runtime_error& e) {
                error::runTimeError(e.what());
            }

            std::cout << res << std::endl;
            return res;
        }

        #pragma mark - Eval Visitor

        Token EvalVisitor::visit_binary(const BinaryExpr& expr) const
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

        Token EvalVisitor::visit_unary(const UnaryExpr& expr) const
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
            if(val.type() == typeid(Token)) return isString(std::any_cast<Token>(val));
            return val.type() == typeid(std::string) || val.type() == typeid(char*) || val.type() == typeid(const char*) ||
                    val.type() == typeid(char) || val.type() == typeid(unsigned char) || val.type() == typeid(signed char);
        }

        any EvalVisitor::castNumber(Token tok) const {
            any val = tok.getLiteral();
            if (val.type() == typeid(double)) {
                return std::any_cast<double>(val);
            } else if (val.type() == typeid(float)) {
                return std::any_cast<float>(val);
            } else if (val.type() == typeid(int)) {
                return std::any_cast<int>(val);
            } else if (val.type() == typeid(long)) {
                return std::any_cast<long>(val);
            } else if (val.type() == typeid(unsigned)) {
                return std::any_cast<unsigned>(val);
            } else if (val.type() == typeid(short)) {
                return std::any_cast<short>(val);
            } else if (val.type() == typeid(unsigned long)) {
                return std::any_cast<unsigned long>(val);
            } else if (val.type() == typeid(unsigned short)) {
                return std::any_cast<unsigned short>(val);
            } else if (val.type() == typeid(unsigned long long)) {
                return std::any_cast<unsigned long long>(val);
            } else if (val.type() == typeid(long long)) {
                return std::any_cast<long long>(val);
            }

            rift::error::runTimeError("Expected a number");
            return "";
        }

        std::string EvalVisitor::castNumberString(any val) const {
            if (val.type() == typeid(double)) {
                return std::to_string(std::any_cast<double>(val));
            } else if (val.type() == typeid(float)) {
                return std::to_string(std::any_cast<float>(val));
            } else if (val.type() == typeid(int)) {
                return std::to_string(std::any_cast<int>(val));
            } else if (val.type() == typeid(long)) {
                return std::to_string(std::any_cast<long>(val));
            } else if (val.type() == typeid(unsigned)) {
                return std::to_string(std::any_cast<unsigned>(val));
            } else if (val.type() == typeid(short)) {
                return std::to_string(std::any_cast<short>(val));
            } else if (val.type() == typeid(unsigned long)) {
                return std::to_string(std::any_cast<unsigned long>(val));
            } else if (val.type() == typeid(unsigned short)) {
                return std::to_string(std::any_cast<unsigned short>(val));
            } else if (val.type() == typeid(unsigned long long)) {
                return std::to_string(std::any_cast<unsigned long long>(val));
            } else if (val.type() == typeid(long long)) {
                return std::to_string(std::any_cast<long long>(val));
            } else if (val.type() == typeid(Token)) {
                return castNumberString(std::any_cast<Token>(val).getLiteral());
            }

            rift::error::runTimeError("Expected a number or token");
            return "";
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
            
            rift::error::runTimeError("Expected a string");
            return "";
        }

        #pragma mark - Arithmetic Ops

        // for now don't allow different types (although we should be able to add diff types for example: int and double)
        std::any EvalVisitor::any_arithmetic(any left, any right, Token op) const
        {
            if (left.type() != right.type())
                rift::error::report(op.line, "any_arithmetic", "not able to do arithmetic ops on different types (future work)", Token(), EvaluatorException("unable to do arithmetic ops on different types (future work)"));

            if (op.type == TokenType::PLUS) {
                _ANY_ARITHMETIC(left,right,+, op);
            } else if (op.type == TokenType::MINUS) {    
                _ANY_ARITHMETIC(left,right,-, op);
            } else if (op.type == TokenType::STAR) {
                _ANY_ARITHMETIC(left,right,*, op);
            } else if (op.type == TokenType::SLASH) {
                _ANY_ARITHMETIC(left,right,/, op); 
            } else if (op.type == TokenType::LESS) {
                _ANY_ARITHMETIC(left,right,<, op);
            } else if (op.type == TokenType::LESS_EQUAL) {
                _ANY_ARITHMETIC(left,right,<=, op);
            } else if (op.type == TokenType::GREATER) {
                _ANY_ARITHMETIC(left,right,>, op);
            } else if (op.type == TokenType::GREATER_EQUAL) {
                _ANY_ARITHMETIC(left,right,>=, op);
            } else if (op.type == TokenType::BANG_EQUAL) {
                _ANY_ARITHMETIC(left,right,!=, op);
            } else if (op.type == TokenType::EQUAL_EQUAL) {
                _ANY_ARITHMETIC(left,right,==, op);
            } else {
                rift::error::report(op.line, "any_arithmetic", "unsupported operand (future work)", Token(), EvaluatorException("unsupported operand (future work)"));
            }
            return any();
        }
    }
}