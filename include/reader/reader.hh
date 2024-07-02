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

#include <error/error.hh>
#include <exception>
#include <string>

namespace rift
{
    namespace reader
    {

        class ReaderException;

        /// @class Reader
        /// @tparam T The type of the reader <Token(Parser), Char(Scanner)>
        /// @brief The base class for reading through lines with utilities
        template <typename T>
        class Reader
        {
            public:
                Reader(std::shared_ptr<std::vector<T>> &source): source(source) {start=0;curr=0;line=1;};
                ~Reader() = default;

            protected:
                std::shared_ptr<std::vector<T>> source;
                unsigned start, curr, line;
                unsigned long long match_length;

                #pragma mark - Reader Methods

                inline bool atEnd() { return this->curr >= source->size(); }
                inline T advance() { return (!atEnd()) ? source->at(curr++) : T(); }
                inline void prevance() { if (curr>0) curr--; }
                // inline typename std::enable_if<std::is_same<T, char>::value, T>::type advance() { return (!atEnd()) ? source->at(curr++) : '\0'; }
                // inline typename std::enable_if<!std::is_same<T, char>::value, T>::type advance() { return (!atEnd()) ? source->at(curr++) : nullptr; }

                /// @brief Peeks at the current character 
                inline bool peek(T expected) {
                    return !atEnd() && source->at(curr) == expected; 
                };
                /// @brief Peeks at the current character with an offset
                inline bool peek_off(T expected, int offset) { return curr+offset<source->size() && source->at(curr+offset) == expected; };
                /// @brief Peeks at the current character
                inline T peek() { return !atEnd() ? source->at(curr) : T(); };
                /// @brief Peeks at the current character with an offset
                inline T peek(int offset) { return curr+offset<source->size() ? source->at(curr+offset) : T(); };
                /// @brief Peeks at the next character 
                inline T peekNext() { return curr+1<source->size() ? source->at(curr+1) : T(); };
                /// @brief Peeks at the next 2 characters
                inline bool peek3(T expected) { return peek_off(expected, 0) && peek_off(expected, 1) && peek_off(expected, 2); }
                /// @brief Peeks the previous character
                inline T peekPrev() { return curr-1>=0 ? source->at(curr-1) : T(); };
                /// @brief Peeks previous with offset
                inline T peekPrev(int offset) { return curr-offset>=0 ? source->at(curr-offset) : T(); };
                /// @brief matches a single character and advances the cursor 
                inline bool match_one(T expected) { return peek(expected) ? advance() : false; }


                /// @brief Scans a comment and advances the cursor
                inline void scanComment() { while (!peek('\n')) advance(); advance(); };
                /// @brief Consumes a T and advances the cursor (Error if not found)
                inline T consume (T expected, std::unique_ptr<ReaderException> error) { 
                    if (peek(expected)) return advance();
                    if (error) rift::error::report(line, "consume", "expected token not found", expected, *error);
                    return T();
                }

                inline T consume_va (std::vector<T> expected, std::unique_ptr<ReaderException> error) { 
                    for (const auto& elem: expected) {
                        auto ret = consume(elem, nullptr);
                        if (ret != T()) return ret;
                    }
                    if (error) rift::error::report(line, "consume", "expected token not found", expected.at(0), *error);
                    return T();
                }
                /// @brief 
                inline bool consume(T expected) {
                    if(peek(expected)) {
                        advance(); return true;
                    }
                    return false;
                }

                /// @brief Matches a T from a set of T's{token, character} and advances the cursor
                inline bool match(std::vector<T> expected) {
                    match_length = 0;
                    for (auto &c : expected) {
                        if (peek(c)) {
                            advance();
                            return true;
                        }
                    }
                    return false;
                }

                inline T match_consume(std::vector<T> expected) {
                    match_length = 0;
                    for (auto &c : expected) {
                        if (peek(c)) {
                            return advance();
                        }
                    }
                    return T();
                }

                /// @brief peeks a word (useful for keywords/identifiers/statements) 
                inline bool peek_word(std::vector<T> expected, int n) {
                    for (int i=0; i<n; i++) {
                        if (!peek_off(expected[i], i)) {
                            return false;
                        }
                    }
                    return true;
                }
        };

        /// @class ReaderException
        /// @brief The base exception for the reader (implement in derived classes)
        class ReaderException: public std::exception
        {
            public:
                ReaderException() = default;
                virtual ~ReaderException() = default;
                virtual const char* what() const noexcept { return "BaseException"; }
        };
    }
}