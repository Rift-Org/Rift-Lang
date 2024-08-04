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

#include <ostream>
#include <scanner/tokens.hh>
#include <error/error.hh>
#include <iostream>

using namespace rift::scanner;

std::string Token::convertTypeString(TokenType type) {
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
        case QUESTION: return "QUESTION";
        case COLON: return "COLON";
        case NULLISH_COAL: return "NULLISH_COAL";
        case IDENTIFIER: return "IDENTIFIER";
        case STRINGLITERAL: return "STRINGLITERAL";
        case NUMERICLITERAL: return "NUMERICLITERAL";
        case BIT_AND: return "BIT_AND";
        case BIT_OR: return "BIT_OR";
        case LOG_AND: return "LOG_AND";
        case CLASS: return "CLASS";
        case ELSE: return "ELSE";
        case ELIF: return "ELIF";
        case FALSE: return "FALSE";
        case FUN: return "FUN";
        case FOR: return "FOR";
        case IF: return "IF";
        case NIL: return "NIL";
        case LOG_OR: return "LOG_OR";
        case PRINT: return "PRINT";
        case RETURN: return "RETURN";
        case SUPER: return "SUPER";
        case THIS: return "THIS";
        case TRUE: return "TRUE";
        case VAR: return "VAR";
        case CONST: return "CONST";
        case WHILE: return "WHILE";
        case C_IDENTIFIER: return "C_IDENTIFIER";
        case IGNORE: return "IGNORE";
        case FAT_ARROW: return "FAT_ARROW";
        case EOFF: return "EOFF";
    }
}

std::string Token::to_string() const
{
    return std::string(convertTypeString(type) + " " + lexeme);
}

#pragma mark - Literal Type Check

bool isInteger(const char* str) {
    char* end;
    errno = 0;
    long val = std::strtol(str, &end, 10);
    return errno == 0 && *end == '\0' && val >= INT_MIN && val <= INT_MAX;
}

bool isShort(const char* str) {
    char* end;
    errno = 0;
    long val = std::strtol(str, &end, 10);
    return errno == 0 && *end == '\0' && val >= SHRT_MIN && val <= SHRT_MAX;
}

bool isLong(const char* str) {
    char* end;
    errno = 0;
    long val = std::strtol(str, &end, 10);
    return errno == 0 && *end == '\0' && val >= LONG_MIN && val <= LONG_MAX;
}

bool isLongLong(const char* str) {
    char* end;
    errno = 0;
    long val = std::strtol(str, &end, 10);
    return errno == 0 && *end == '\0' && val >= LLONG_MIN && val <= LLONG_MAX;
}

bool isUnsignedShort(const char* str) {
    char* end;
    errno = 0;
    unsigned long val = std::strtoul(str, &end, 10);
    return errno == 0 && *end == '\0' && val <= USHRT_MAX;
}

bool isUnsignedInteger(const char* str) {
    char* end;
    errno = 0;
    unsigned long val = std::strtoul(str, &end, 10);
    return errno == 0 && *end == '\0' && val <= UINT_MAX;
}

bool isUnsignedLong(const char* str) {
    char* end;
    errno = 0;
    unsigned long val = std::strtoul(str, &end, 10);
    return errno == 0 && *end == '\0' && val <= ULONG_MAX;
}

bool isUnsignedLongLong(const char* str) {
    char* end;
    errno = 0;
    unsigned long val = std::strtoul(str, &end, 10);
    return errno == 0 && *end == '\0' && val <= ULLONG_MAX;
}

bool isFloat(const char* str) {
    char* end;
    errno = 0;
    float val = std::strtof(str, &end);
    return errno == 0 && *end == '\0' && val != HUGE_VALF && val != -HUGE_VALF;
}

bool isDouble(const char* str) {
    char* end;
    errno = 0;
    double val = std::strtod(str, &end);
    return errno == 0 && *end == '\0' && val != HUGE_VAL && val != -HUGE_VAL;
}

bool isBoolean(const char* str) {
    return strcmp(str, "true") == 0 || strcmp(str, "false") == 0;
}

bool isString(const char* str) {
    return str[0] == '"' && str[strlen(str) - 1] == '"';
}

bool isChar(const char* str) {
    return str[0] == '\'' && str[strlen(str) - 1] == '\'';
}

std::any Token::getLiteral() const
{
    if (type == STRINGLITERAL) {
        return std::any{std::string(lexeme)};
    } else if (type == IDENTIFIER || type == C_IDENTIFIER) {
        return std::any{std::string(lexeme)};
    }/* else if (type == NUMERICLITERAL) {
        return std::any{std::stod(lexeme)};
    }*/ else if (type == TRUE) {
        return std::any{true};
    } else if (type == FALSE) {
        return std::any{false};
    } else if (type == NIL) {
        return std::any{nullptr};
    } else if (type == FUN) {
        return literal;
    }

    if (isInteger(lexeme.c_str()))
        return std::any{std::stoi(lexeme.c_str())};
    if (isUnsignedInteger(lexeme.c_str()))
        return std::any{std::stoul(lexeme.c_str())};
    if (isShort(lexeme.c_str()))
        return std::any{std::stoi(lexeme.c_str())};
    if (isUnsignedShort(lexeme.c_str()))
        return std::any{std::stoul(lexeme.c_str())};
    if (isUnsignedLong(lexeme.c_str()))
        return std::any{std::stoul(lexeme.c_str())};
    if (isLongLong(lexeme.c_str()))
        return std::any{std::stoll(lexeme.c_str())};
    if (isUnsignedLongLong(lexeme.c_str()))
        return std::any{std::stoull(lexeme.c_str())};
    if (isFloat(lexeme.c_str()))
        return std::any{std::strtof(lexeme.c_str(), nullptr)};
    if (isDouble(lexeme.c_str()))
        return std::any{std::stod(lexeme.c_str())};
    if (isBoolean(lexeme.c_str()))
        return std::any{bool(lexeme == "true")};
    if (isString(lexeme.c_str()))
        return std::any{std::string(lexeme.substr(1, lexeme.length() - 2))};
    if (isChar(lexeme.c_str()))
        return std::any{char(lexeme[1])};

    rift::error::report(line, "getLiteral", "Unknown Literal Type", Token(), std::exception());
    return std::any(); // should never hit
}

#pragma mark - Operators

bool Token::operator==(const Token &token) const
{
    return this->type == token.type;
}

bool Token::operator!=(const Token &token) const
{
    return this->type != token.type;
}