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
            QUESTION,
            COLON,

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
            FAT_ARROW,

            // Operators
            OPERATOR,
            NULLISH_COAL,
            LOG_AND,
            LOG_OR,
            BIT_AND,
            BIT_OR,

            // Literals
            IDENTIFIER,
            C_IDENTIFIER,
            STRINGLITERAL,
            NUMERICLITERAL,

            // Keywords
            CLASS,
            FALSE,
            FUN,
            FOR,
            IF,
            ELSE,
            ELIF,
            NIL,
            PRINT,
            RETURN,
            SUPER,
            THIS,
            TRUE,
            VAR,
            CONST,
            WHILE,
            
            IGNORE,
            EOFF
        };

        /// @struct Token
        /// @brief Represents a token in the source code (a lexeme with a type and a literal value)
        struct Token
        {
            TokenType type;
            std::string text, lexeme;
            std::any literal;
            const std::type_info* l_type;

            int line;

            Token() : type(TokenType::NIL), lexeme(""), literal(0), l_type(&typeid(void)), line(0) {}
            Token(TokenType type) : type(type), lexeme(""), literal(0), l_type(&typeid(void)), line(0) {}

            Token(TokenType type, std::string lexeme, std::any literal, int line)
            {
                this->type = type;
                this->lexeme = lexeme;
                this->literal = literal;
                this->line = line;
                this->l_type = &typeid(literal);
            }

            Token(const Token& other) {
                this->type = other.type;
                this->lexeme = other.lexeme;
                this->literal = other.literal;
                this->line = other.line;
                this->l_type = other.l_type;
            }

            ~Token() { }

            /// @brief Converts a TokenType to a string
            static std::string convertTypeString(TokenType type);
            /// @brief Converts a Token to a string
            std::string to_string() const;

            friend std::ostream &operator<<(std::ostream &os, const Token &token) {
                os << std::string("Token(Type=") << token.convertTypeString(token.type)
                /*<< ", Lexeme=\"" << token.lexeme
                << "\", Literal=" << token.literal.type().name()
                << ", Line=" << token.line << ")"*/;
                return os;
            }
            bool operator==(const Token &token) const;

            std::any getLiteral() const;
        };

    }
}