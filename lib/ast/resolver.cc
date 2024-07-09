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

#include <ast/resolver.hh>

using std::stack;

namespace rift
{
    namespace ast
    { 
        #pragma mark - Statics for Resolve

        namespace Resolve
        {
            static Eval *eval = new Eval();
            static vector<unordered_map<string, bool>> scopes = {};

            void beginScope()
            {
                scopes.push_back(unordered_map<string, bool>());
            }

            void endScope()
            {
                scopes.pop_back();
            }

            void declare(Token name) 
            {
                if (scopes.empty()) return;
                unordered_map<string, bool>& scope = scopes.back();
                if (scope.find(name.lexeme) != scope.end()) {
                    error::report(name.line, "at declaration", "Variable with this name already declared in this scope.", name, ResolverException("Variable with this name already declared in this scope."));
                }
                scope[name.lexeme] = false;
            }

            void define(Token name)
            {
                if (scopes.empty()) return;
                unordered_map<string, bool>& scope = scopes.back();
                scope[name.lexeme] = true;
            }

            void resolveLocal(Expr* expr, Token name)
            {
                for (int i = scopes.size() - 1; i >= 0; i--) {
                    if (scopes[i].find(name.lexeme) != scopes[i].end()) {
                        eval->resolve(expr, scopes.size() - 1 - i);
                        return;
                    }
                }
            }
        }

        #pragma mark - EXPRESSIONS

        Token Visitor::resolve_literal(const ResolverLiteral& expr) const
        {
            return Token();
        }

        Token Visitor::resolve_unary(const ResolverUnary& expr) const
        {
            expr.expr->accept(*this);
            return Token();
        }

        Token Visitor::resolve_binary(const ResolverBinary& expr) const
        {
            expr.left->accept(*this);
            expr.right->accept(*this);
            return Token();
        }

        Token Visitor::resolve_grouping(const ResolverGrouping& expr) const
        {
            expr.expr->accept(*this);
            return Token();
        }

        Token Visitor::resolve_ternary(const ResolverTernary& expr) const
        {
            if(truthy(expr.condition->accept(*this))) {
                expr.left->accept(*this);
            } else {
                expr.right->accept(*this);
            }

            return Token();
        }

        Token Visitor::resolve_assign(const ResolverAssign& expr) const
        {
            // visit_assign(expr.expr.);
            Resolve::resolveLocal(&expr.expr, expr.expr.name);
        }

        Token Visitor::resolve_call(const ResolverCall& expr) const
        {
            visit_call(expr.expr);
        }

        Token Visitor::resolve_var_expr(const ResolverVarExpr& expr) const
        {
            if (!Resolve::scopes.empty() && 
                Resolve::scopes.back().find(expr.expr.value.lexeme)!=Resolve::scopes.back().end() &&
                Resolve::scopes.back().find(expr.expr.value.lexeme)->second == false) {
                error::report(expr.expr.value.line, "resolve_var_expr", "Cannot read local variable in its own initializer.", expr.expr.value, ResolverException("Cannot read local variable in its own initializer."));
            }
            Resolve::resolveLocal(&expr.expr, expr.expr.value);
        }

        #pragma mark -  STATEMENTS

        Token Visitor::resolve_expr_stmt(const ResolverStmtExpr& stmt) const
        {
            visit_expr_stmt(stmt.stmt);
        }

        Token Visitor::resolve_print_stmt(const ResolverPrint& stmt) const
        {
            visit_print_stmt(stmt.stmt);
        }

        Token Visitor::resolve_if_stmt(const ResolverIf& stmt) const
        {
            visit_if_stmt(stmt.stmt);
        }

        Token Visitor::resolve_return_stmt(const ResolverReturn& stmt) const
        {
            visit_return_stmt(stmt.stmt);
            // maybe set return token to NIL
        }

        Token Visitor::resolve_block_stmt(const ResolverBlock& block) const
        {
            Resolve::beginScope();
            visit_block_stmt(block.block);
            Resolve::endScope();
        }

        Token Visitor::resolve_for_stmt(const ResolverFor& stmt) const
        {
            // visit_for_stmt(stmt.stmt);
            // resolve(stmt.condition);
            // resolve(stmt.body);
        }

        #pragma mark - DECLARATIONS

        Tokens Visitor::resolve_decl_var(const ResolverDeclVar& decl) const
        {
            Resolve::declare(decl.decl.identifier);
            if (decl.decl.expr != nullptr) {
                visit_decl_var(decl.decl);
            }
            Resolve::define(decl.decl.identifier);
        }

        Tokens Visitor::resolve_decl_func(const ResolverDeclFunc& decl) const
        {
            Resolve::declare(decl.decl.func->name);
            Resolve::define(decl.decl.func->name);

            // resolve params
            Resolve::beginScope();   
            for (auto param: decl.decl.func->params) {
                Resolve::declare(param);
                Resolve::define(param);
            }

            // resolve block (cant initate abstract class ResolverBlock)
            if (decl.decl.func->blk != nullptr) {
                Resolve::beginScope();
                visit_block_stmt(*decl.decl.func->blk);
                Resolve::endScope();
            }

            Resolve::endScope();
        }

        Tokens Visitor::resolve_decl_stmt(const ResolverDeclStmt& decl) const
        {
            // visit_decl_stmt(decl.decl);
        }

        #pragma mark - PROGRAM

        Tokens Visitor::resolve_program(const Program& prgm) const
        {
            // for (auto decl: prgm.decls) {
            //     visit_decl_stmt(*decl);
            // }
        }
    }
}