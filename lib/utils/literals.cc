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


#pragma mark

#include <utils/literals.hh>

namespace rift
{
    bool truthy(const Token& tok) {
        any val = tok.getLiteral();
        if (val.type() == typeid(bool)) {
            return std::any_cast<bool>(val);
        }
        return true;
    }

    bool equal(const Token& tl, const Token& tr) {
        any left = tl.getLiteral();
        any right = tr.getLiteral();
        if (left.type() != right.type()) return false;
        else if (left.type() == typeid(bool)) return std::any_cast<bool>(left) == std::any_cast<bool>(right);
        else if (left.type() == typeid(std::string)) return std::any_cast<std::string>(left) == std::any_cast<std::string>(right);
        else if (left.type() == typeid(double)) return std::any_cast<double>(left) == std::any_cast<double>(right);
        return false;
    }

    bool isNumber(const Token& tok) {
        any val = tok.getLiteral();
        return val.type() == typeid(double) || val.type() == typeid(float) || 
                val.type() == typeid(int) || val.type() == typeid(long) ||
                val.type() == typeid(unsigned) || val.type() == typeid(short) || 
                val.type() == typeid(unsigned long) || val.type() == typeid(unsigned short) ||
                val.type() == typeid(unsigned long long) || val.type() == typeid(long long);
    }

    bool isString(const Token& tok) {
        any val = tok.getLiteral();
        if(val.type() == typeid(Token)) return isString(std::any_cast<Token>(val));
        return val.type() == typeid(std::string) || val.type() == typeid(char*) || val.type() == typeid(const char*) ||
                val.type() == typeid(char) || val.type() == typeid(unsigned char) || val.type() == typeid(signed char);
    }

    any castNumber(const Token& tok) {
        any val = tok.getLiteral();
        if (val.type() == typeid(double)) {
            return std::any_cast<double>(val);
        } else if (val.type() == typeid(float)) {
            return std::any_cast<float>(val);
        } else if (val.type() == typeid(int)) {
            return std::any_cast<int>(val);
        } else if (val.type() == typeid(long)) {
            return std::any_cast<long>(val);
        } else if (val.type() == typeid(unsigned)) {
            return std::any_cast<unsigned>(val);
        } else if (val.type() == typeid(short)) {
            return std::any_cast<short>(val);
        } else if (val.type() == typeid(unsigned long)) {
            return std::any_cast<unsigned long>(val);
        } else if (val.type() == typeid(unsigned short)) {
            return std::any_cast<unsigned short>(val);
        } else if (val.type() == typeid(unsigned long long)) {
            return std::any_cast<unsigned long long>(val);
        } else if (val.type() == typeid(long long)) {
            return std::any_cast<long long>(val);
        }

        rift::error::runTimeError("Expected a number");
        return "";
    }

    std::string castNumberString(const any& val) {
        if (val.type() == typeid(double)) {
            return std::to_string(std::any_cast<double>(val));
        } else if (val.type() == typeid(float)) {
            return std::to_string(std::any_cast<float>(val));
        } else if (val.type() == typeid(int)) {
            return std::to_string(std::any_cast<int>(val));
        } else if (val.type() == typeid(long)) {
            return std::to_string(std::any_cast<long>(val));
        } else if (val.type() == typeid(unsigned)) {
            return std::to_string(std::any_cast<unsigned>(val));
        } else if (val.type() == typeid(short)) {
            return std::to_string(std::any_cast<short>(val));
        } else if (val.type() == typeid(unsigned long)) {
            return std::to_string(std::any_cast<unsigned long>(val));
        } else if (val.type() == typeid(unsigned short)) {
            return std::to_string(std::any_cast<unsigned short>(val));
        } else if (val.type() == typeid(unsigned long long)) {
            return std::to_string(std::any_cast<unsigned long long>(val));
        } else if (val.type() == typeid(long long)) {
            return std::to_string(std::any_cast<long long>(val));
        } else if (val.type() == typeid(Token)) {
            return castNumberString(std::any_cast<Token>(val).getLiteral());
        }

        rift::error::runTimeError("Expected a number or token");
        return "";
    }

    std::string castString(const Token& tok) {
        any val = tok.getLiteral();
        if (val.type() == typeid(std::string)) {
            return std::any_cast<std::string>(val);
        } else if (val.type() == typeid(char*)) {
            return std::string(std::any_cast<char*>(val));
        } else if (val.type() == typeid(const char*)) {
            return std::string(std::any_cast<const char*>(val));
        } else if (val.type() == typeid(char)) {
            return std::string(1, std::any_cast<char>(val));
        } else if (val.type() == typeid(unsigned char)) {
            return std::string(1, std::any_cast<unsigned char>(val));
        } else if (val.type() == typeid(signed char)) {
            return std::string(1, std::any_cast<signed char>(val));
        }
        
        rift::error::runTimeError("Expected a string");
        return "";
    }
}

