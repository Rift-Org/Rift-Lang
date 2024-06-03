
#pragma once
#include <string>

namespace rift
{
    namespace scanner
    {
        namespace tokens
        {
            enum Type
            {
                // Single-character tokens.
                LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
                COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

                // One or two character tokens
                BANG, BANG_EQUAL,
                EQUAL, EQUAL_EQUAL,
                GREATER, GREATER_EQUAL,
                LESS, LESS_EQUAL, 
                SEPARATOR, WHITESPACE,
                OPERATOR,

                // Literals.
                IDENTIFIER, STRINGLITERAL, NUMERICLITERAL,

                // Keywords.
                AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
                PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

                EOFF
            };

            struct Token
            {
                Type type;
                std::string text, mini, literal;

                int line;

                Token(Type type, std::string mini, std::string literal, int line)
                {
                    this->type = type; this->mini = mini;
                    this->literal = literal; this->line = line;
                }

                std::string to_string()
                {
                    return std::string(type + " " + mini + " " + literal);
                }

            };
        }
    }
}