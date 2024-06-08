
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
                std::string text, lexeme, literal;

                int line;

                Token(Type type, std::string lexeme, std::string literal, int line)
                {
                    this->type = type; this->lexeme = lexeme;
                    this->literal = literal; this->line = line;
                }

                std::string convertTypeString(Type type) {
                    switch (type) {
                        case LEFT_PAREN: return "LEFT_PAREN";
                        case RIGHT_PAREN: return "RIGHT_PAREN";
                        case LEFT_BRACE: return "LEFT_BRACE";
                        case RIGHT_BRACE: return "RIGHT_BRACE";
                        case COMMA: return "COMMA";
                        case DOT: return "DOT";
                        case MINUS: return "MINUS";
                        case PLUS: return "PLUS";
                        case SEMICOLON: return "SEMICOLON";
                        case SLASH: return "SLASH";
                        case STAR: return "STAR";
                        case BANG: return "BANG";
                        case BANG_EQUAL: return "BANG_EQUAL";
                        case EQUAL: return "EQUAL";
                        case EQUAL_EQUAL: return "EQUAL_EQUAL";
                        case GREATER: return "GREATER";
                        case GREATER_EQUAL: return "GREATER_EQUAL";
                        case LESS: return "LESS";
                        case LESS_EQUAL: return "LESS_EQUAL";
                        case SEPARATOR: return "SEPARATOR";
                        case WHITESPACE: return "WHITESPACE";
                        case OPERATOR: return "OPERATOR";
                        case IDENTIFIER: return "IDENTIFIER";
                        case STRINGLITERAL: return "STRINGLITERAL";
                        case NUMERICLITERAL: return "NUMERICLITERAL";
                        case AND: return "AND";
                        case CLASS: return "CLASS";
                        case ELSE: return "ELSE";
                        case FALSE: return "FALSE";
                        case FUN: return "FUN";
                        case FOR: return "FOR";
                        case IF: return "IF";
                        case NIL: return "NIL";
                        case OR: return "OR";
                        case PRINT: return "PRINT";
                        case RETURN: return "RETURN";
                        case SUPER: return "SUPER";
                        case THIS: return "THIS";
                        case TRUE: return "TRUE";
                        case VAR: return "VAR";
                        case WHILE: return "WHILE";
                        case EOFF: return "EOFF";
                    }
                }

                std::string to_string()
                {
                    return std::string(convertTypeString(type) + " " + lexeme + " " + literal);
                }

            };
        }
    }
}