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


#pragma once
#include <scanner/tokens.hh>
#include <utils/macros.hh>

#include <any>
#include <iostream>
#include <stdlib.h>
#include <memory>

using namespace rift::scanner;

namespace rift
{
    namespace ast
    {
        namespace Expr
        {
            /// @note forward declrations
            __DEFAULT_FORWARD_VA(
                Visitor,
                Binary,
                Grouping,
                Literal,
                Unary
            );

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
                    virtual T accept(const Visitor<T>& visitor) const = 0;
                    virtual ~Expr() = default;
            };


            /// @class Visitor
            /// @brief This class is used to visit each type of expression
            template <typename T>
            class Visitor
            {
                public:
                    virtual T visit_binary(const Binary<T>& expr) const = 0;
                    virtual T visit_grouping(const Grouping<T>& expr) const = 0;
                    virtual T visit_literal(const Literal<T>& expr) const = 0;
                    virtual T visit_unary(const Unary<T>& expr) const = 0;
            };

            #pragma mark - Concrete Expressions

            /// @class Binary
            /// @param left The left operand
            /// @param op The operator
            /// @param right The right operand
            template <typename T = void>
            class Binary : public Expr<T>
            {
                public:
                    Binary(std::unique_ptr<Expr<T>> left, Token op, std::unique_ptr<Expr<T>> right): op(op), left(std::move(left)), right(std::move(right)) {};
                    Token op;
                    std::unique_ptr<Expr<T>> left;
                    std::unique_ptr<Expr<T>> right;

                    inline T accept(const Visitor<T>& visitor) const override { return visitor.visit_binary(*this); }
            };

            /// @class Grouping
            /// @param expr The subexpression
            template <typename T = void>
            class Grouping : public Expr<T>
            {
                public:
                    Grouping(std::unique_ptr<Expr<T>> expr): expr(std::move(expr)) {};
                    std::unique_ptr<Expr<T>> expr;

                    inline T accept(const Visitor<T>& visitor) const override {return visitor.visit_grouping(*this);}
            };

            /// @class Literal
            /// @param value The value of the literal
            template <typename T = void>
            class Literal: public Expr<T>
            {
                public:
                    Literal(Token value): value(value) {};
                    Token value;

                    inline T accept(const Visitor<T> &visitor) const override {return visitor.visit_literal(*this);}
            };

            /// @class Unary
            /// @param op The operator
            /// @param expr The operand
            template <typename T = void>
            class Unary : public Expr<T>
            {
                public:
                    Unary(Token op, std::unique_ptr<Expr<T>> expr): op(op), expr(std::move(expr)) {};
                    Token op;
                    std::unique_ptr<Expr<T>> expr;

                    inline T accept(const Visitor<T>& visitor) const override {return visitor.visit_unary(*this);}
            };
        }
    }
};