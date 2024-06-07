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

            Scanner(const std::string& source)
                : source(source), 
                  keywords(std::unordered_map<std::string, Type>()){}
            ~Scanner(){}
        private:
            int start=0, curr=0, line=1;
            inline bool atEnd() {return this->curr >= source.size();}
            static std::vector<Token> scanTokens(){};
            char advance() {return source.at(curr++);}
            void addToken(Type type) {addToken(type, NULL);};
            void addToken(Type type, std::string literal) {
                tokens.push_back(Token(type, source.substr(start, curr-start), literal, line));
            }
            inline bool isDigit(char c) {return c>='0' && c<='9';}
            inline bool isAlpha(char c) {return (c>='a' && c<='z') || (c>='A' && c<='Z') || c=='_';}
            inline bool isAlphaNumeric(char c) {return isAlpha(c) || isDigit(c);}
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
            
            void identifier() {
                while (isAlphaNumeric(advance()));
                std::string text = source.substr(start, curr-start);
                if (keywords.find(text)!= keywords.end()) {
                    addToken(keywords.at(text));
                } else {
                    addToken(Type::IDENTIFIER, text);
                }
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
                        else if (isAlpha(c)) {
                            identifier();
                        }
                        else rift::error::report(line, "scanToken", "Unorthodox Character");break;
                };
            };

            void initalizeKeywords() {
                keywords["and"] = Type::AND;
                keywords["class"] = Type::CLASS;
                keywords["else"] = Type::ELSE;
                keywords["false"] = Type::FALSE;
                keywords["for"] = Type::FOR;
                keywords["fun"] = Type::FUN;
                keywords["if"] = Type::IF;
                keywords["nil"] = Type::NIL;
                keywords["or"] = Type::OR;
                keywords["print"] = Type::PRINT;
                keywords["return"] = Type::RETURN;
                keywords["super"] = Type::SUPER;
                keywords["this"] = Type::THIS;
                keywords["true"] = Type::TRUE;
                keywords["var"] = Type::VAR;
                keywords["while"] = Type::WHILE;
            }
        };
    };
}