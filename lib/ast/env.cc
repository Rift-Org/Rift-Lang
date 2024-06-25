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
        Token Environment::getEnv(const strv_t& name) const
        {
            if (!values.contains(name)) {
                rift::error::runTimeError("🛑 Undefined variable '" + std::string(name) + "'");
            }
            return values.at("env");
        }

        void Environment::setEnv(const strv_t& name, const Token& value)
        {
            if (values.contains(name)) {
                std::cout << "🟡 [line " << value.line << "] Warn: Variable '" << name << "' already defined." << std::endl;
            }
            values[name] = value;
        }
    }
}