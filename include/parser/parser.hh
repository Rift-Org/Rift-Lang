
#pragma once

#include <memory>
#include <vector>
#include <exception>
#include <scanner/tokens.hh>
#include <reader/reader.hh>
#include <ast/expr.hh>

using namespace rift::scanner;
using namespace rift::reader;

using GenExpr = rift::ast::Expr::Expr<rift::scanner::Token>;

namespace rift
{
    namespace parser
    {
        class Parser : public Reader<Token>
        {
            public:
                Parser(std::shared_ptr<std::vector<Token>> &tokens) : Reader<Token>(tokens), tokens(tokens)  {};
                ~Parser() = default;
            protected:
                std::shared_ptr<std::vector<Token>> tokens;
                std::exception exception;

            private:
                #pragma mark - Grammar Evaluators
                
                /// @note rules in order of precedence

                /// @example 1 + 2 * 3
                std::unique_ptr<GenExpr> expression();
                /// @example 1 == 1, 1 != 2
                std::unique_ptr<GenExpr> equality();
                /// @example 1 > 2, 1 <= 2
                std::unique_ptr<GenExpr> comparison();
                /// @example 1 + 2, 1 - 2
                std::unique_ptr<GenExpr> term();
                /// @example 1 * 2, 1 / 2
                std::unique_ptr<GenExpr> factor();
                /// @example -1, !1
                std::unique_ptr<GenExpr> unary();
                /// @example 1, "string", true, false, nil
                std::unique_ptr<GenExpr> primary();

                void synchronize();

                std::unique_ptr<GenExpr> parse();
        };

        class ParserException : public ReaderException
        {
            public:
                ParserException(const std::string &message) : message(message) {}
                ~ParserException() = default;

                const char *what() const noexcept override { return message.c_str(); }

            private:
                std::string message;
        };
    }
}