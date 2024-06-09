
#pragma once
#include <string>

namespace rift
{
    namespace scanner
    {
        /// @enum TokenType
        /// @brief Represents the type of token.
        enum TokenType
        {
            // Single-character tokens
            LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
            COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

            // One or two character tokens
            BANG, BANG_EQUAL,
            EQUAL, EQUAL_EQUAL,
            GREATER, GREATER_EQUAL,
            LESS, LESS_EQUAL, 
            SEPARATOR, WHITESPACE,
            OPERATOR,

            // Literals
            IDENTIFIER, STRINGLITERAL, NUMERICLITERAL,

            // Keywords
            AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
            PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

            EOFF
        };

        /// @struct Token
        /// @brief Represents a token in the source code (a lexeme with a type and a literal value)
        struct Token
        {
            TokenType type;
            std::string text, lexeme, literal;

            int line;

            Token(TokenType type, std::string lexeme, std::string literal, int line)
            {
                this->type = type; this->lexeme = lexeme;
                this->literal = literal; this->line = line;
            }
            private:
                /// @brief Converts a TokenType to a string
                std::string convertTypeString(TokenType type);
                /// @brief Converts a Token to a string
                std::string to_string();
        };
    }
}