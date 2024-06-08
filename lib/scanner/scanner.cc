
#include <scanner/scanner.hh>

typedef rift::scanner::tokens::Token Token;
typedef rift::scanner::tokens::Type Type;

namespace rift
{
    namespace scanner
    {
        
        #pragma mark - Initializers

        // static members
        unsigned Scanner::start = 0;
        unsigned Scanner::curr = 0;
        unsigned Scanner::line = 1;
        
        Scanner::Scanner(const std::string& source) {
            this->source = source;

            keywords = std::unordered_map<std::string, Type>();
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

        #pragma mark - Token Scanners

        /// if three quotes then its a multiline string
        void Scanner::string() {
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

        void Scanner::num() {
            while (isDigit(advance()));
            if (peek('.') && isDigit(peekNext())) {
                advance();
                while (isDigit(advance()));
            }
            addToken(Type::NUMERICLITERAL, source.substr(start, curr));
        }
        
        void Scanner::identifier() {
            while (isAlphaNumeric(advance()));
            std::string text = source.substr(start, curr-start);
            if (keywords.find(text)!= keywords.end()) {
                addToken(keywords.at(text));
            } else {
                addToken(Type::IDENTIFIER, text);
            }
        }

        #pragma mark - Public API

        void Scanner::scan_tokens(unsigned cnt)
        {
            while (cnt) {
                start = curr;
                scan_token();
                cnt-=1;
            }
        }

        void Scanner::scan_token()
        {
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
                    else if (isAlpha(c)) identifier();
                    else rift::error::report(line, "scanToken", "Unorthodox Character");
                    break;
            };
        }
    }
}