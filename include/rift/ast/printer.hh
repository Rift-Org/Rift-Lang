#pragma once

namespace rift
{
    namespace ast
    {
        class printer
        {
            public:
                printer() = default;
                ~printer() = default;
                std::string print(rift::ast::Expr *expr);
                std::string parenthesize(std::string name, rift::ast::Expr *expr);
        }
    }
}