
#pragma once

#include <error/error.hh>
#include <exception>

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
                Reader(const std::vector<T> &source): source(source) {start=0;curr=0;line=1;};
                ~Reader() = default;

            protected:
                std::vector<T> source;
                unsigned start, curr, line;

                #pragma mark - Reader Methods

                inline bool atEnd() { return this->curr >= source.size(); }
                inline T advance() { return (!atEnd()) ? source.at(curr++) : T(); }
                // inline typename std::enable_if<std::is_same<T, char>::value, T>::type advance() { return (!atEnd()) ? source.at(curr++) : '\0'; }
                // inline typename std::enable_if<!std::is_same<T, char>::value, T>::type advance() { return (!atEnd()) ? source.at(curr++) : nullptr; }

                /// @brief Peeks at the current character 
                inline bool peek(T expected) { return !atEnd() && source.at(curr) == expected; };
                /// @brief Peeks at the current character with an offset
                inline bool peek_off(T expected, int offset) { return curr+offset<source.size() && source.at(curr+offset) == expected; };
                /// @brief Peeks at the current character
                inline T peek() { return !atEnd() ? source.at(curr) : T(); };
                /// @brief Peeks at the next character 
                inline T peekNext() { return curr+1<source.size() ? source.at(curr+1) : T(); };
                /// @brief Peeks at the next 2 characters
                inline bool peek3(T expected) { return peek_off(expected, 0) && peek_off(expected, 1) && peek_off(expected, 2); }
                /// @brief Peeks the previous character
                inline T peekPrev() { return curr-1>=0 ? source.at(curr-1) : T(); };

                /// @brief Scans a comment and advances the cursor
                inline void scanComment() { while (peek('\n')) advance();};
                /// @brief Consumes a T and advances the cursor (Error if not found)
                inline T consume (T expected, std::unique_ptr<ReaderException> error) { 
                    if (peek(expected)) return advance();
                    rift::error::report(line, "consume", "expected " << expected);
                    if (error) throw error;
                }
                /// @brief Matches a T from a set of T's{token, character} and advances the cursor
                inline bool match(std::vector<T> expected) {
                    for (auto &c : expected) {
                        if (peek(c)) {
                            advance();
                            return true;
                        }
                    }
                    return false;
                }

                inline bool match_one(T expected) { return peek(expected) ? advance() : false; }
        };

        /// @class ReaderException
        /// @brief The base exception for the reader (implement in derived classes)
        class ReaderException
        {
            public:
                ReaderException() = default;
                ~ReaderException() = default;
                virtual const char* what() const noexcept { return "BaseException"; }
        };
    }
}