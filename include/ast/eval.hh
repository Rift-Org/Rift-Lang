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
        static Token return_token = Token(TokenType::NIL, "", "", -1);
        class Eval
        {
            public:
                Eval();
                ~Eval() = default;

                /// @brief Evaluates the given *expr/stmt/decl*
                std::vector<string> evaluate(const Program& expr, bool interactive);

            private:
                std::unique_ptr<Visitor> visitor;
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