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
            std::string source;
            std::vector<Token> tokens = std::vector<Token>();
            std::unordered_map<std::string, Type> keywords;

            Scanner(const std::string& source){}
            ~Scanner(){}

            /// @fn scan_token
            /// @brief Scans the source code and returns a list of tokens
            void scan_token();

            /// @fn scan_tokens
            /// @brief Scans the source code and returns a list of tokens
            /// @param cnt The number of tokens to scan
            void scan_tokens(unsigned cnt);
        private:
            static int start, curr, line;

            #pragma mark - Token Management

            void addToken(Type type) {addToken(type, NULL);};
            void addToken(Type type, std::string literal) {
                tokens.push_back(Token(type, source.substr(start, curr-start), literal, line));
            }

            #pragma mark - Helper Functions (Inline)

            inline bool atEnd() { return this->curr >= source.size(); }
            inline char advance() { return source.at(curr++); }
            inline bool isDigit(char c) { return c>='0' && c<='9'; }
            inline bool isAlpha(char c) { return (c>='a' && c<='z') || (c>='A' && c<='Z') || c=='_'; }
            inline bool isAlphaNumeric(char c) { return isAlpha(c) || isDigit(c); }

            /// @brief Peeks at the current character 
            inline bool peek(char expected) { return curr<source.size() && source.at(curr) == expected; };
            /// @brief Peeks at the current character with an offset
            inline bool peek_off(char expected, int offset) { return curr+offset<source.size() && source.at(curr+offset) == expected; };
            /// @brief Peeks at the next character 
            inline char peekNext() { return curr+1<source.size() ? source.at(curr+1) : '\0'; };
            /// @brief Peeks at the next 2 characters
            inline bool peek3(char expected) { return peek_off(expected, 0) && peek_off(expected, 1) && peek_off(expected, 2); }

            /// @brief Scans a comment and advances the cursor
            inline void scanComment() { while (peek('\n')) advance();};
            /// @brief Matches a character and advances the cursor
            inline bool match(char expected) { return !atEnd() && (source.at(curr)==expected ? curr++ : false); }

            #pragma mark - Token Scanners

            /// @brief Scans a string literal
            void string();
            /// @brief Scans a numeric literal
            void num();
            /// @brief Scans an identifier
            void identifier();
        };
    };
}