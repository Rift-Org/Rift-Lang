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

#include <exception>
#include <ast/grmr.hh>
#include <ast/expr.hh>
#include <ast/stmt.hh>
#include <ast/decl.hh>
#include <ast/prgm.hh>
#include <utils/arithmetic.hh>
#include <utils/literals.hh>

using any = std::any;
using string = std::string;

namespace rift
{
    namespace ast
    {
        class Eval : public ExprVisitor<Token>, StmtVisitor<void>, 
                            DeclVisitor<Token>, ProgramVisitor<Tokens>
        {
            public:
                Eval() = default;
                ~Eval() = default;

                // expressions
                Token visit_assign(const Assign<Token>& expr) const override;
                Token visit_binary(const Binary<Token>& expr) const override;
                Token visit_grouping(const Grouping<Token>& expr) const override;
                Token visit_literal(const Literal<Token>& expr) const override;
                Token visit_var_expr(const VarExpr<Token>& expr) const override;
                Token visit_unary(const Unary<Token>& expr) const override;
                Token visit_ternary(const Ternary<Token>& expr) const override;
                Token visit_call(const Call<Token>& expr) const override;

                // statements
                void visit_expr_stmt(const StmtExpr<void>& stmt) const override;
                void visit_print_stmt(const StmtPrint<void>& stmt) const override;
                void visit_if_stmt(const StmtIf<void>& stmt) const override;
                void visit_return_stmt(const StmtReturn<void>& stmt) const override;
                void visit_block_stmt(const Block<void>& block) const override;
                void visit_for_stmt(const For<void>& decl) const override;

                // declarations
                Token visit_decl_stmt(const DeclStmt<Token>& decl) const override;
                Token visit_decl_var(const DeclVar<Token>& decl) const override;
                Token visit_decl_func(const DeclFunc<Token>& decl) const override;
                Token visit_decl_class(const DeclClass<Token>& decl) const override;

                // program
                Tokens visit_program(const Program<Tokens>& prgm) const override;

                /// @brief Evaluates the given *expr/stmt/decl*
                std::vector<string> evaluate(std::unique_ptr<Program<Tokens>>& prgm, bool interactive);

                /// @note Resolver API
                static Token lookup(Expr<Token>* expr, std::string key);
                void resolve(Expr<Token>* expr, int depth);

            private:
                const std::unique_ptr<ProgramVisitor<Tokens>> visitor;
        };

        /// @class EvaluatorException
        /// @brief The base exception for the evaluator
        class EvaluatorException: public std::exception
        {
            public:
                EvaluatorException(const std::string &message) : message(message) {}
                ~EvaluatorException() = default;

                const char *what() const noexcept override { return message.c_str(); }

            private:
                std::string message;
        };

        class StmtReturnException: public std::exception
        {
            public:
                StmtReturnException(Token tok): tok(tok) {};
                ~StmtReturnException() = default;
                Token tok;
                const char* what() const noexcept override {
                    return "Statement Return Exception";
                }
        };
    }
}