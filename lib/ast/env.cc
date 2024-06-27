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

        void Environment::setEnv(const str_t& name, const Token& value)
        {
            if (!values.contains(name) && child != nullptr) {
                child->setEnv(name, value);
            } else {
                values[name] = value;
            }
        }

        void Environment::printState()
        {
            Environment *curr = this;
            while (curr != nullptr) {
                std::cout << "IN  ";
                for (const auto& [key, value] : curr->values) {
                    std::cout << key << " => " << value.to_string() << std::endl;
                }
                curr = curr->child;
            }
        }
    }
}