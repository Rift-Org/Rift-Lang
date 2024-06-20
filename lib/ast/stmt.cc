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

#include <ast/stmt.hh>
#include <ast/eval.hh>

namespace rift::ast
{
    // ****
    // TODO: Have visit_expression and visit_print inside eval.cc
    //       which then evaluate() gets trinkled down into 
    // ****
    void StmtVisitor::visit_expression_stmt(const Stmt& expr) const
    {
        auto *expression = dynamic_cast<const Expression*>(&expr);
        if (expression)
        {
            Eval eval;
            eval.evaluate(*expression->expr);
        }
    }

    void StmtVisitor::visit_print_stmt(const Stmt& expr) const
    {
        auto *print = dynamic_cast<const Print*>(&expr);
        if (print)
        {
            Eval eval;
            eval.evaluate(*print->expr);
        }
    }

    // void StmtVisitor::visit_var_stmt(const Stmt& expr) const
    // {
    //     auto *var = dynamic_cast<const Var*>(&expr);
    //     if (var)
    //     {
    //         Eval eval;
    //         eval.evaluate(*var->expr);
    //     }
    // }
}