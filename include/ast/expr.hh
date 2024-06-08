
#pragma once
#include "../scanner/tokens.hh"

#include <iostream>
#include <stdlib.h>

typedef rift::scanner::tokens::Token Token;

namespace rift
{
    namespace ast
    {
        namespace Expr
        {
            /// @class Expr
            /// @brief Base class for all expressions 
            /// @details Types of expression include
            ///          - Binary: An expression with two operands and an operator
            ///            - Example: 1 + 2
            ///          - Grouping: An expression with a single subexpression
            ///            - Example: (1 + 2)
            ///          - Literal: An expression with a single value
            ///            - Example: 1
            ///          - Unary: An expression with a single operator and a single operand
            ///            - Example: -1
            template <typename T = void>
            class Expr
            {
                public:
                    virtual T accept(Visitor& visitor) = 0;
                    virtual ~Expr() = default;
            };


            /// @class Visitor
            /// @brief This class is used to visit each type of expression
            template <typename T = void>
            class Visitor
            {
                public:
                    virtual T visitBinary(Binary *expr) = 0;
                    virtual T visitGrouping(Grouping *expr) = 0;
                    virtual T visitLiteral(Literal *expr) = 0;
                    virtual T visitUnary(Unary *expr) = 0;
            };

            # pragma mark - Concrete Expressions

            /// @class Binary
            /// @param left The left operand
            /// @param op The operator
            /// @param right The right operand
            template <typename T = void>
            class Binary : public Expr
            {
                public:
                    Binary(Expr *left, Token op, Expr *right): left(std::move(left)), op(op), right(std::move(right)) {}
                    Token op;
                    std::unique_ptr<Expr> left;
                    std::unique_ptr<Expr> right;

                    inline void accept(ExprVisitor& visitor) override {visitor.visitBinary(this);}
            };

            /// @class Grouping
            /// @param expr The subexpression
            template <typename T = void>
            class Grouping : public Expr
            {
                public:
                    Grouping(Expr *expr): expr(std::move(expr)) {};
                    std::unique_ptr<Expr> expr;

                    inline void accept(ExprVisitor& visitor) override {visitor.visitGrouping(this);}
            };

            /// @class Literal
            /// @param value The value of the literal
            template <typename T = void>
            class Literal: public Expr
            {
                public:
                    Literal(char* value): value(value) {};
                    char* value;

                    inline void accept(ExprVisitor &visitor) override {visitor.visitLiteral(this);}
            };

            /// @class Unary
            /// @param op The operator
            /// @param expr The operand
            template <typename T = void>
            class Unary : public Expr
            {
                public:
                    Unary(Token op, Expr *expr): op(op), expr(std::move(expr)) {};
                    Token op;
                    std::unique_ptr<Expr> expr;

                    inline void accept(ExprVisitor& visitor) override {visitor.visitUnary(this);}
            };
        }
    }
};