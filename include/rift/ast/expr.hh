
#pragma once
#include "../scanner/tokens.hh"

#include <iostream>
#include <stdlib.h>

typedef rift::scanner::tokens::Token Token;

namespace rift
{
    namespace ast
    {
        class Expr
        {
        public:
            class Visitor;
            virtual ~Expr() = default;

            class Binary; class Grouping; class Literal; class Unary;

            /* visitor pattern */
            class ExprVisitor
            {
                public:
                    virtual void visitBinary(Expr::Binary *expr) = 0;
                    virtual void visitGrouping(Expr::Grouping *expr) = 0;
                    virtual void visitLiteral(Expr::Literal *expr) = 0;
                    virtual void visitUnary(Expr::Unary *expr) = 0;
            };

            class ExprAcceptor
            {
                public:
                    virtual void accept(ExprVisitor& visitor) = 0;
            };

            class Binary : public ExprAcceptor
            {
                public:
                    Binary(Expr *left, Token op, Expr *right): left(std::move(left)), op(op), right(std::move(right)) {}
                    Token op;
                    std::unique_ptr<Expr> left;
                    std::unique_ptr<Expr> right;

                    void accept(ExprVisitor& visitor) override {visitor.visitBinary(this);}
            };
            class Grouping : public ExprAcceptor
            {
                public:
                    Grouping(Expr *expr): expr(std::move(expr)) {};
                    std::unique_ptr<Expr> expr;
                    void accept(ExprVisitor& visitor) override {visitor.visitGrouping(this);}
            };
            class Literal : public ExprAcceptor
            {
                public:
                    Literal(char* value): value(value) {};
                    char* value;
                    void accept(ExprVisitor &visitor) override {visitor.visitLiteral(this);}
            };
            class Unary : public ExprAcceptor
            {
                public:
                    Unary(Token op, Expr *expr): op(op), expr(std::move(expr)) {};
                    Token op;
                    std::unique_ptr<Expr> expr;
                    void accept(ExprVisitor& visitor) override {visitor.visitUnary(this);}
            };
        };
    }
}