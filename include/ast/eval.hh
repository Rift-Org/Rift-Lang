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
#include <utils/arithmetic.hh>
#include <utils/literals.hh>

using any = std::any;
using string = std::string;

namespace rift
{
    namespace ast
    {
        class Eval
        {
            public:
                Eval();
                ~Eval() = default;

                /// @brief Evaluates the given *expression*
                std::vector<string> evaluate(const Program& expr);
                /// @brief Evaluates the given *statement*
                void evaluate(const rift::ast::Stmt& expr);

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
    }
}