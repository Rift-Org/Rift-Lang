#include <ast/parser.hh>
#include <memory>

using namespace rift::scanner;
using namespace rift::ast::Expr;
using GenExpr = rift::ast::Expr::Expr<rift::scanner::Token>;

namespace rift
{
    namespace ast
    {
        #pragma mark - Grammar Evaluators

        std::unique_ptr<GenExpr> Parser::primary()
        {
            if (match({Token(TokenType::FALSE, "false", "", line)}))
                return std::unique_ptr<Literal<Token>>(new Literal<Token>(false));
            if (match({Token(TokenType::TRUE, "true", "", line)}))
                return std::unique_ptr<Literal<Token>>(new Literal<Token>(true));
            if (match({Token(TokenType::NIL, "nil", "", line)}))
                return std::unique_ptr<Literal<Token>>(new Literal<Token>(nullptr));

            if (match({Token(TokenType::NUMERICLITERAL, "", "", line)}))
                return std::unique_ptr<Literal<Token>>(new Literal<Token>(peekPrev().literal));
            if (match({Token(TokenType::STRINGLITERAL, "", "", line)}))
                return std::unique_ptr<Literal<Token>>(new Literal<Token>(peekPrev().literal));

            if (match({Token(TokenType::LEFT_PAREN, "(", "", line)})) { 
                auto expr = expression();
                consume(Token(TokenType::RIGHT_PAREN, ")", "", line), std::unique_ptr<ParserException>(new ParserException("Expected ')' after expression")));
                return expr;
            }

            throw ParserException("Expected expression");
        }

        std::unique_ptr<GenExpr> Parser::unary()
        {
            if (match({Token(TokenType::BANG, "!", "", line)})) {
                auto op = peekPrev();
                auto right = unary();
                return std::unique_ptr<ast::Expr::Unary<Token>>(new ast::Expr::Unary<Token>(op, std::move(right)));
            }

            if (match({Token(TokenType::MINUS, "-", "", line)})) {
                auto op = peekPrev();
                auto right = unary();
                return std::unique_ptr<ast::Expr::Unary<Token>>(new ast::Expr::Unary<Token>(op, std::move(right)));
            }

            return primary();
        }

        std::unique_ptr<GenExpr> Parser::factor()
        {
            auto expr = unary();

            while (match({Token(TokenType::STAR, "*", "", line)}) || match({Token(TokenType::SLASH, "/", "", line)})) {
                auto op = peekPrev();
                auto right = unary();
                expr = std::unique_ptr<ast::Expr::Binary<Token>>(new ast::Expr::Binary<Token>(std::move(expr), op, std::move(right)));
            }

            return expr;
        }

        std::unique_ptr<GenExpr> Parser::term()
        {
            auto expr = factor();

            while (match({Token(TokenType::MINUS, "-", "", line)}) || match({Token(TokenType::PLUS, "+", "", line)})) {
                auto op = peekPrev();
                auto right = factor();
                expr = std::unique_ptr<ast::Expr::Binary<Token>>(new ast::Expr::Binary<Token>(std::move(expr), op, std::move(right)));
            }

            return expr;
        }

        std::unique_ptr<GenExpr> Parser::comparison()
        {
            auto expr = term();

            while (match({Token(TokenType::GREATER, ">", "", line)}) || match({Token(TokenType::GREATER_EQUAL, ">=", "", line)}) || match({Token(TokenType::LESS, "<", "", line)}) || match({Token(TokenType::LESS_EQUAL, "<=", "", line)}) ) {
                auto op = peekPrev();
                auto right = term();
                expr = std::unique_ptr<ast::Expr::Binary<Token>>(new ast::Expr::Binary<Token>(std::move(expr), op, std::move(right)));
            }

            return expr;
        }

        std::unique_ptr<GenExpr> Parser::equality()
        {
            auto expr = comparison();

            while (match({Token(TokenType::BANG_EQUAL, "!=", "", line)}) || match({Token(TokenType::EQUAL_EQUAL, "==", "", line)})) {
                auto op = peekPrev();
                auto right = comparison();
                expr = std::unique_ptr<ast::Expr::Binary<Token>>(new ast::Expr::Binary<Token>(std::move(expr), op, std::move(right)));
            }

            return expr;
        }

        std::unique_ptr<GenExpr> Parser::expression()
        {
            return equality();
        }

        std::unique_ptr<GenExpr> Parser::parse()
        {
            try {
                return expression();
            } catch (const ParserException &e) {
                return nullptr;
            }
        }

        # pragma mark - Utilities

        void Parser::synchronize()
        {
            advance();

            while (!atEnd()) {
                if (peekPrev().type == TokenType::SEMICOLON) return;

                switch (peek().type) {
                    case TokenType::CLASS:
                    case TokenType::FUN:
                    case TokenType::VAR:
                    case TokenType::FOR:
                    case TokenType::IF:
                    case TokenType::WHILE:
                    case TokenType::PRINT:
                    case TokenType::RETURN:
                        return;
                    default:
                        break;
                }

                advance();
            }
        }
    }
}