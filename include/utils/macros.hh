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

#pragma mark - Forward Declarations

// for no class
#define __DEFAULT_FORWARD_NONE_1(class_name) __DEFAULT_FORWARD_NONE(class_name)
#define __DEFAULT_FORWARD_NONE_2(class1, class2) __DEFAULT_FORWARD_NONE(class1) __DEFAULT_FORWARD_NONE(class2)
#define __DEFAULT_FORWARD_NONE_3(class1, class2, class3) __DEFAULT_FORWARD_NONE(class1) __DEFAULT_FORWARD_NONE(class2) __DEFAULT_FORWARD_NONE(class3)
#define __DEFAULT_FORWARD_NONE_4(class1, class2, class3, class4) __DEFAULT_FORWARD_NONE(class1) __DEFAULT_FORWARD_NONE(class2) __DEFAULT_FORWARD_NONE(class3) __DEFAULT_FORWARD_NONE(class4)
#define __DEFAULT_FORWARD_NONE_5(class1, class2, class3, class4, class5) __DEFAULT_FORWARD_NONE(class1) __DEFAULT_FORWARD_NONE(class2) __DEFAULT_FORWARD_NONE(class3) __DEFAULT_FORWARD_NONE(class4) __DEFAULT_FORWARD_NONE(class5)
#define __DEFAULT_FORWARD_NONE_6(class1, class2, class3, class4, class5, class6) __DEFAULT_FORWARD_NONE(class1) __DEFAULT_FORWARD_NONE(class2) __DEFAULT_FORWARD_NONE(class3) __DEFAULT_FORWARD_NONE(class4) __DEFAULT_FORWARD_NONE(class5) __DEFAULT_FORWARD_NONE(class6)

// for one class
#define __DEFAULT_FORWARD_1(class_name) __DEFAULT_FORWARD(class_name)
#define __DEFAULT_FORWARD_2(class1, class2) __DEFAULT_FORWARD(class1) __DEFAULT_FORWARD(class2)
#define __DEFAULT_FORWARD_3(class1, class2, class3) __DEFAULT_FORWARD(class1) __DEFAULT_FORWARD(class2) __DEFAULT_FORWARD(class3)
#define __DEFAULT_FORWARD_4(class1, class2, class3, class4) __DEFAULT_FORWARD(class1) __DEFAULT_FORWARD(class2) __DEFAULT_FORWARD(class3) __DEFAULT_FORWARD(class4)
#define __DEFAULT_FORWARD_5(class1, class2, class3, class4, class5) __DEFAULT_FORWARD(class1) __DEFAULT_FORWARD(class2) __DEFAULT_FORWARD(class3) __DEFAULT_FORWARD(class4) __DEFAULT_FORWARD(class5)
#define __DEFAULT_FORWARD_6(class1, class2, class3, class4, class5, class6) __DEFAULT_FORWARD(class1) __DEFAULT_FORWARD(class2) __DEFAULT_FORWARD(class3) __DEFAULT_FORWARD(class4) __DEFAULT_FORWARD(class5) __DEFAULT_FORWARD(class6)

// for multiple classes
#define __DEFAULT_FORWARD_MULTI_1(class_name) __DEFAULT_FORWARD_MULTI(class_name)
#define __DEFAULT_FORWARD_MULTI_2(class1, class2) __DEFAULT_FORWARD_MULTI(class1) __DEFAULT_FORWARD_MULTI(class2)
#define __DEFAULT_FORWARD_MULTI_3(class1, class2, class3) __DEFAULT_FORWARD_MULTI(class1) __DEFAULT_FORWARD_MULTI(class2) __DEFAULT_FORWARD_MULTI(class3)
#define __DEFAULT_FORWARD_MULTI_4(class1, class2, class3, class4) __DEFAULT_FORWARD_MULTI(class1) __DEFAULT_FORWARD_MULTI(class2) __DEFAULT_FORWARD_MULTI(class3) __DEFAULT_FORWARD_MULTI(class4)
#define __DEFAULT_FORWARD_MULTI_5(class1, class2, class3, class4, class5) __DEFAULT_FORWARD_MULTI(class1) __DEFAULT_FORWARD_MULTI(class2) __DEFAULT_FORWARD_MULTI(class3) __DEFAULT_FORWARD_MULTI(class4) __DEFAULT_FORWARD_MULTI(class5)
#define __DEFAULT_FORWARD_MULTI_6(class1, class2, class3, class4, class5, class6) __DEFAULT_FORWARD_MULTI(class1) __DEFAULT_FORWARD_MULTI(class2) __DEFAULT_FORWARD_MULTI(class3) __DEFAULT_FORWARD_MULTI(class4) __DEFAULT_FORWARD_MULTI(class5) __DEFAULT_FORWARD_MULTI(class6)

#define __SELECT_FORWARD(_1, _2, _3, _4, _5, _6, NAME, ...) NAME

#define __DEFAULT_FORWARD_NONE(class_name)\
            class class_name; \

#define __DEFAULT_FORWARD(class_name) \
            template <typename T> \
            class class_name; \

#define __DEFAULT_FORWARD_MULTI(class_name) \
            template <typename T, typename V> \
            class class_name; \

#define __DEFAULT_FORWARD_EACH(...) \
            __DEFAULT_FORWARD(__VA_ARGS__)

#define __DEFAULT_FORWARD_VA(...) \
            __SELECT_FORWARD(__VA_ARGS__, __DEFAULT_FORWARD_6, __DEFAULT_FORWARD_5, \
                                __DEFAULT_FORWARD_4, __DEFAULT_FORWARD_3, \
                                    __DEFAULT_FORWARD_2, __DEFAULT_FORWARD_1) (__VA_ARGS__)
#define __DEFAULT_FORWARD_NONE_VA(...) \
            __SELECT_FORWARD(__VA_ARGS__, __DEFAULT_FORWARD_NONE_6, __DEFAULT_FORWARD_NONE_5, \
                                __DEFAULT_FORWARD_NONE_4, __DEFAULT_FORWARD_NONE_3, \
                                    __DEFAULT_FORWARD_NONE_2, __DEFAULT_FORWARD_NONE_1) (__VA_ARGS__)
#define __DEFAULT_FORWARD_MULTI_VA(...) \
            __SELECT_FORWARD(__VA_ARGS__, __DEFAULT_FORWARD_MULTI_6, __DEFAULT_FORWARD_MULTI_5, \
                                __DEFAULT_FORWARD_MULTI_4, __DEFAULT_FORWARD_MULTI_3, \
                                    __DEFAULT_FORWARD_MULTI_2, __DEFAULT_FORWARD_MULTI_1) (__VA_ARGS__)


#pragma mark - Arithmetic

#define _ANY_ARITHMETIC(left, right, op, op_tok) \
    if (left.type() == typeid(double)) \
        return std::any_cast<double>(left) op std::any_cast<double>(right); \
    else if (left.type() == typeid(int)) \
        return std::any_cast<int>(left) op std::any_cast<int>(right); \
    else if (left.type() == typeid(unsigned)) \
        return std::any_cast<unsigned>(left) op std::any_cast<unsigned>(right); \
    else if (left.type() == typeid(short)) \
        return std::any_cast<short>(left) op std::any_cast<short>(right); \
    else if (left.type() == typeid(unsigned long)) \
        return std::any_cast<unsigned long>(left) op std::any_cast<unsigned long>(right); \
    else if (left.type() == typeid(unsigned short)) \
        return std::any_cast<unsigned short>(left) op std::any_cast<unsigned short>(right); \
    else if (left.type() == typeid(unsigned long long)) \
        return std::any_cast<unsigned long long>(left) op std::any_cast<unsigned long long>(right); \
    else if (left.type() == typeid(long long)) \
        return std::any_cast<long long>(left) op std::any_cast<long long>(right); \
    else if (left.type() == typeid(Token))\
        return any_arithmetic(std::any_cast<Token>(left).getLiteral(), std::any_cast<Token>(right).getLiteral(), op_tok);\
    else \
        rift::error::report(op_tok.line, "Arithmetic Error", "Invalid operands for arithmetic operation", Token(), std::exception());


#pragma mark  - Specific Codebase

#define _BOOL_LOGIC(op) if (isNumber(left) && isNumber(right)) {\
            resBool = std::any_cast<bool>(any_arithmetic(left, right, op));\
            return Token(resBool?TokenType::TRUE:TokenType::FALSE, resBool?"true":"false", resBool, op.line);\
        } else if (isString(left) && isString(right)) {\
            resBool = castString(left) > castString(right);\
            return Token(resBool?TokenType::TRUE:TokenType::FALSE, resBool?"true":"false", resBool, op.line);\
        }\
                