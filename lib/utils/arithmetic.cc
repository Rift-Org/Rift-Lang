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

#include <utils/arithmetic.hh>
#include <stdexcept>

namespace rift
{
    // for now don't allow different types (although we should be able to add diff types for example: int and double)
    any any_arithmetic(any left, any right, const Token& op)
    {
        if (left.type() != right.type())
            rift::error::report(op.line, "any_arithmetic", "not able to do arithmetic ops on different types (future work)", Token(), std::runtime_error("unable to do arithmetic ops on different types (future work)"));

        if (op.type == TokenType::PLUS) {
            _ANY_ARITHMETIC(left,right,+, op);
        } else if (op.type == TokenType::MINUS) {    
            _ANY_ARITHMETIC(left,right,-, op);
        } else if (op.type == TokenType::STAR) {
            _ANY_ARITHMETIC(left,right,*, op);
        } else if (op.type == TokenType::SLASH) {
            _ANY_ARITHMETIC(left,right,/, op); 
        } else if (op.type == TokenType::LESS) {
            _ANY_ARITHMETIC(left,right,<, op);
        } else if (op.type == TokenType::LESS_EQUAL) {
            _ANY_ARITHMETIC(left,right,<=, op);
        } else if (op.type == TokenType::GREATER) {
            _ANY_ARITHMETIC(left,right,>, op);
        } else if (op.type == TokenType::GREATER_EQUAL) {
            _ANY_ARITHMETIC(left,right,>=, op);
        } else if (op.type == TokenType::BANG_EQUAL) {
            _ANY_ARITHMETIC(left,right,!=, op);
        } else if (op.type == TokenType::EQUAL_EQUAL) {
            _ANY_ARITHMETIC(left,right,==, op);
        } else {
            rift::error::report(op.line, "any_arithmetic", "unsupported operand (future work)", Token(), std::runtime_error("unsupported operand (future work)"));
        }
        return any();
    }
}