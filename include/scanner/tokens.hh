
#pragma once
#include <string>
#include <any>

namespace rift
{
    namespace scanner
    {
        /// @enum TokenType
        /// @brief Represents the type of token.
        enum TokenType
        {
            // Single-character tokens
            LEFT_PAREN,
            RIGHT_PAREN,
            LEFT_BRACE,
            RIGHT_BRACE,
            COMMA,
            DOT,
            MINUS,
            PLUS,
            SEMICOLON,
            SLASH,
            STAR,

            // One or two character tokens
            BANG,
            BANG_EQUAL,
            EQUAL,
            EQUAL_EQUAL,
            GREATER,
            GREATER_EQUAL,
            LESS,
            LESS_EQUAL,
            SEPARATOR,
            WHITESPACE,
            OPERATOR,

            // Literals
            IDENTIFIER,
            STRINGLITERAL,
            NUMERICLITERAL,

            // Keywords
            AND,
            CLASS,
            ELSE,
            FALSE,
            FUN,
            FOR,
            IF,
            NIL,
            OR,
            PRINT,
            RETURN,
            SUPER,
            THIS,
            TRUE,
            VAR,
            WHILE,

            EOFF
        };

        /// @struct Token
        /// @brief Represents a token in the source code (a lexeme with a type and a literal value)
        struct Token
        {
            TokenType type;
            std::string text, lexeme;
            std::any literal;

            int line;

            Token(TokenType type, std::string lexeme, std::any literal, int line)
            {
                this->type = type;
                this->lexeme = lexeme;
                this->literal = literal;
                this->line = line;
            }

            Token() : type(TokenType::EOFF), lexeme(""), literal(0), line(0) {}

            /// @brief Converts a TokenType to a string
            static std::string convertTypeString(TokenType type);
            /// @brief Converts a Token to a string
            std::string to_string() const;

            // friend std::ostream &operator<<(std::ostream &os, const Token &token);
            bool operator==(const Token &token);
        };

        // std::ostream& operator<<(std::ostream& os, const Token& token) {
        //     os << std::string("Token(Type=") << token.convertTypeString(token.type)
        //     /*<< ", Lexeme=\"" << token.lexeme
        //     << "\", Literal=" << token.literal.type().name()
        //     << ", Line=" << token.line << ")"*/;
        //     return os;
        // }

    }