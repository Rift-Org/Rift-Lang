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

#include <ast/env.hh>
#include <error/error.hh>

namespace rift
{
    namespace ast
    {
        Token Environment::getEnv(const str_t& name) const
        {
            if (!values.contains(name)) {
                
                if(child != nullptr)
                    return child->getEnv(name);

                return Token();
            }
            return values.at(name);
        }

        void Environment::setEnv(const str_t& name, const Token& value, bool is_const)
        {
            if (!values.contains(name) && child != nullptr) {
                child->setEnv(name, value, is_const);
            } else {
                if (values.contains(name) && const_keys.contains(name)) {
                    error::report(0, "Environment", "Cannot reassign a constant variable", values.at(name), std::exception());
                } else {
                    values[name] = value;
                    if (is_const) const_keys.insert(name);
                }
            }
        }

        void Environment::printState()
        {
            Environment *curr = this;
            while (curr != nullptr) {
                for (const auto& [key, value] : curr->values) {
                    std::cout << key << " => " << value.to_string() << std::endl;
                }
                curr = curr->child;
            }
        }
    }
}