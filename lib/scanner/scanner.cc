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


#include <scanner/scanner.hh>
#include <iostream>
#include <format>

typedef rift::scanner::Token Token;
typedef rift::scanner::TokenType Type;
using namespace rift::reader;

namespace rift
{
    namespace scanner
    {
        
        #pragma mark - Initializers
        
        Scanner::Scanner(std::shared_ptr<std::vector<char>> source) : Reader<char>(source) {
            this->source = source;
            this->tokens = std::vector<Token>();

            keywords = std::unordered_map<std::string, Type>();
            keywords["and"] = Type::LOG_AND;
            keywords["class"] = Type::CLASS;
            keywords["false"] = Type::FALSE;
            keywords["for"] = Type::FOR;
            keywords["fun"] = Type::FUN;
            keywords["if"] = Type::IF;
            keywords["else"] = Type::ELSE;
            keywords["elif"] = Type::ELIF;
            keywords["nil"] = Type::NIL;
            keywords["or"] = Type::LOG_OR;
            keywords["print"] = Type::PRINT;
            keywords["return"] = Type::RETURN;
            keywords["super"] = Type::SUPER;
            keywords["this"] = Type::THIS;
            keywords["true"] = Type::TRUE;
            keywords["mut"] = Type::VAR;
            keywords["while"] = Type::WHILE;
        }

        #pragma mark - Token Scanners

        /// if three quotes then its a multiline string
        void Scanner::string() {
            prevance();
            if (peek3('"')) {
                advance();
                while (!peek3('"') && !atEnd()) {
                    line++; advance();
                }
            } else if (peek('"')) {
                advance();
                while (!peek('"') && !atEnd()) {
                    if (peek('\n')) line++;
                    advance();
                }
            }

            if (atEnd()) {
                rift::error::report(line, "string", "Unterminated String", Token(), std::exception());
                return;
            }

            if (peek3('"')) {
                addToken(Type::STRINGLITERAL, std::string(source->begin()+start, source->begin()+curr+3));
                advance(); advance(); advance();
            }
            else {
                addToken(Type::STRINGLITERAL, std::string(source->begin()+start, source->begin()+curr+1));
                advance();
            }
        }

        void Scanner::num() {
            while (isDigit(peek())) advance();
            if (peek('.') && isDigit(peekNext())) {
                advance();
                while (isDigit(advance()));
            }

            addToken(Type::NUMERICLITERAL, std::string(source->begin()+start, source->begin()+curr));
        }
        
        void Scanner::identifier() {
            while (isIdentifier(peek())) advance();
            std::string text = std::string(source->begin()+start, source->begin()+curr);
            text.erase(std::remove_if(text.begin(), text.end(), ::isspace), text.end());
            if (keywords.find(text)!= keywords.end()) {
                addToken(keywords.at(text));
            } else {
                addToken(Type::IDENTIFIER, text);
            }
        }

        bool Scanner::keyword() {
            prevance();
            for (const auto &[k,v] : keywords) {
                std::vector vec = std::vector(k.begin(), k.end());
                if (peek_word(vec, k.size()-1)) {
                    addToken(v);
                    for (size_t i=0; i<k.size(); i++) advance();
                    return true;
                }
            }
            return false;
        }

        #pragma mark - Public API

        void Scanner::scan_token()
        {
            char c = advance();
            switch(c) {
                case '(': addToken(Type::LEFT_PAREN, "(");break;
                case ')': addToken(Type::RIGHT_PAREN, ")");break;
                case '{': addToken(Type::LEFT_BRACE, "{");break;
                case '}': addToken(Type::RIGHT_BRACE, "}");break;
                case ',': addToken(Type::COMMA, ",");break;
                case '.':
                    if(isDigit(peekNext())) num();
                    else addToken(Type::DOT);break;
                case '-': addToken(Type::MINUS, "-");break;
                case '+': addToken(Type::PLUS, "+");break;
                case ';': addToken(Type::SEMICOLON, ";");break;
                case '*': addToken(Type::STAR, "*");break;
                case '!': addToken(match_one('=') ? Type::BANG_EQUAL : Type::BANG);break;
                case '=': addToken(match_one('=') ? Type::EQUAL_EQUAL : Type::EQUAL);break;
                case '<': addToken(match_one('=') ? Type::LESS_EQUAL : Type::LESS);break;
                case '>': addToken(match_one('=') ? Type::GREATER_EQUAL : Type::GREATER);break;
                case '/': match_one('/') ? scanComment() : addToken(Type::SLASH, "/");break;
                case '"': string(); break;
                case ' ': break;
                case '\r': break;
                case '\t': break;
                case '\n': line++; break;

                case '?': twoChar(Type::QUESTION, Type::NULLISH_COAL, c); break;
                case ':': addToken(TokenType::COLON, ":");break;

                case '&': twoChar(Type::BIT_AND, Type::LOG_AND, c); break;
                case '|': twoChar(Type::BIT_OR, Type::LOG_OR, c); break;

                default:
                    if (isDigit(c)) num();
                    else if (keyword()) return;
                    else if (isAlpha(c)) identifier();
                    else rift::error::report(line, "scanToken", std::format("Unorthodox Character {}", c), Token(), std::exception());
            };
        }

        void Scanner::scan_tokens(unsigned cnt)
        {
            while (cnt--) {
                start = curr;
                scan_token();
            }
        }

        void Scanner::scan_source()
        {
            while (!atEnd()) {
                start = curr;
                scan_token();
            }
        }
    }
}