#pragma once

#include "tokens.hh"
#include "../error/error.hh"
#include <string>
#include <stdlib.h>
#include <map>

#include <vector> // Include the necessary header file

typedef rift::scanner::tokens::Token Token;
typedef rift::scanner::tokens::Type Type;
namespace rift
{
    namespace scanner
    {
        struct Scanner
        {
            const std::string source;
            std::vector<Token> tokens = std::vector<Token>();
            std::unordered_map<std::string, Type> keywords;
            int start=0, curr=0, line=1;

            Scanner(const std::string& source)
            : source(source) {
                keywords = {
                {"and", Type::AND},
                {"class", Type::CLASS},
                {"else", Type::ELSE},
                {"false", Type::FALSE},
                {"fun", Type::FUN},
                {"for", Type::FOR},
                {"if", Type::IF},
                {"nil", Type::NIL},
                {"or", Type::OR},
                {"print", Type::PRINT},
                {"return", Type::RETURN},
                {"super", Type::SUPER},
                {"this", Type::THIS},
                {"true", Type::TRUE},
                {"var", Type::VAR},
                {"while", Type::WHILE}
            };
            }
            inline bool atEnd() {return this->curr >= source.size();}

        private:
            static std::vector<Token> scanTokens(){};
            char advance() {return source.at(curr++);}
            void addToken(Type type) {addToken(type, NULL);};
            inline bool isDigit(char c) {return c>='0' && c<='9';}
            inline bool peek(char expected) {return curr<source.size() && source.at(curr) == expected;};
            inline char peekNext() {return curr+1<source.size() ? source.at(curr+1) : '\0';};
            inline bool peek3(char expected) {
                if (curr<source.size() && source.at(curr) != expected) return false;
                if (curr+1<source.size() && source.at(curr+1) != expected) return false;
                if (curr+2<source.size() && source.at(curr+2) != expected) return false;
                return true;
            }
            inline void scanComment() { while (peek('\n')) advance();};
            inline bool match(char expected) {
                return !atEnd() && (source.at(curr)==expected ? curr++ : false);
            }

            /// if three quotes then its a multiline string
            void string() {
                if (peek3('"')) {
                    while (!peek3('"') && !atEnd()) {
                        line++; advance();
                    }
                } else if (peek('"')) {
                    while (!peek('"') && !atEnd()) {
                        if (peek('\n')) line++;
                        advance();
                    }
                }

                if (atEnd()) {
                    rift::error::report(line, "string", "Unterminated String");
                    return;
                }

                if (peek3('"')) {advance(); advance(); advance();}
                else advance();

                if (peek3('"')) addToken(Type::STRINGLITERAL, source.substr(start+3, curr-3));
                else addToken(Type::STRINGLITERAL, source.substr(start+1, curr-1));
            }

            void num() {
                while (isDigit(advance()));
                if (peek('.') && isDigit(peekNext())) {
                    advance();
                    while (isDigit(advance()));
                }
                addToken(Type::NUMERICLITERAL, source.substr(start, curr));
            }

            void scanToken() {
                char c = advance();
                switch(c) {
                    case '(': addToken(Type::LEFT_PAREN);break;
                    case ')': addToken(Type::RIGHT_PAREN);break;
                    case '{': addToken(Type::LEFT_BRACE);break;
                    case '}': addToken(Type::RIGHT_BRACE);break;
                    case ',': addToken(Type::COMMA);break;
                    case '.': 
                        if(isDigit(peekNext())) num();
                        else addToken(Type::DOT);break;
                    case '-': addToken(Type::MINUS);break;
                    case '+': addToken(Type::PLUS);break;
                    case ';': addToken(Type::SEMICOLON);break;
                    case '*': addToken(Type::STAR);break;
                    case '!': addToken(match('=') ? Type::BANG_EQUAL : Type::BANG);break;
                    case '=': addToken(match('=') ? Type::EQUAL_EQUAL : Type::EQUAL);break;
                    case '<': addToken(match('=') ? Type::LESS_EQUAL : Type::LESS);break;
                    case '>': addToken(match('=') ? Type::GREATER_EQUAL : Type::GREATER);break;
                    case '/': match('/') ? scanComment() : addToken(Type::SLASH);break;
                    case '"': string(); break;
                    case ' ': break;
                    case '\r': break;
                    case '\t': break;
                    case '\n': line++; break;

                    default:
                        if (isDigit(c)) num();
                        else rift::error::report(line, "scanToken", "Unorthodox Character");break;
                };
            };
        };
    };
}