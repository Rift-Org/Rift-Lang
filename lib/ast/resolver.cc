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

            void resolveLocal(Expr<rift::scanner::Token>* expr, Token name)
            {
                for (int i = scopes.size() - 1; i >= 0; i--) {
                    if (scopes[i].find(name.lexeme) != scopes[i].end()) {
                        eval->resolve(expr, scopes.size() - 1 - i);
                        return;
                    }
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////
        #pragma mark - EXPRESSIONS
        ////////////////////////////////////////////////////////////////////////

        Token Resolver::visit_literal(const Literal<Token>& expr) const
        {
            return Token();
        }

        Token Resolver::visit_unary(const Unary<Token>& expr) const
        {
            expr.expr->accept(*this);
            return Token();
        }

        Token Resolver::visit_binary(const Binary<Token>& expr) const
        {
            expr.left->accept(*this);
            expr.right->accept(*this);
            return Token();
        }

        Token Resolver::visit_grouping(const Grouping<Token>& expr) const
        {
            expr.expr->accept(*this);
            return Token();
        }

        Token Resolver::visit_ternary(const Ternary<Token>& expr) const
        {
            if(truthy(expr.condition->accept(*this))) {
                expr.left->accept(*this);
            } else {
                expr.right->accept(*this);
            }

            return Token();
        }

        Token Resolver::visit_assign(const Assign<Token>& expr) const
        {
            visit_assign(expr);
            Assign<Token>* assign = const_cast<Assign<Token>*>(&expr);
            Resolve::resolveLocal(assign, expr.name);
        }

        Token Resolver::visit_call(const Call<Token>& expr) const
        {
            visit_call(expr);
        }

        Token Resolver::visit_var_expr(const VarExpr<Token>& expr) const
        {
            if (!Resolve::scopes.empty() && 
                Resolve::scopes.back().find(expr.value.lexeme)!=Resolve::scopes.back().end() &&
                Resolve::scopes.back().find(expr.value.lexeme)->second == false) {
                error::report(expr.value.line, "resolve_var_expr", "Cannot read local variable in its own initializer.", expr.value, ResolverException("Cannot read local variable in its own initializer."));
            }
            Resolve::resolveLocal(const_cast<VarExpr<Token>*>(&expr), expr.value);
        }

        ////////////////////////////////////////////////////////////////////////
        #pragma mark -  STATEMENTS
        ////////////////////////////////////////////////////////////////////////

        Token Resolver::visit_expr_stmt(const StmtExpr<Token>& stmt) const
        {
            visit_expr_stmt(stmt);
        }

        Token Resolver::visit_print_stmt(const StmtPrint<Token>& stmt) const
        {
            visit_print_stmt(stmt);
        }

        Token Resolver::visit_if_stmt(const StmtIf<Token>& stmt) const
        {
            visit_if_stmt(stmt);
        }

        Token Resolver::visit_return_stmt(const StmtReturn<Token>& stmt) const
        {
            visit_return_stmt(stmt);
            // maybe set return token to NIL
        }

        Token Resolver::visit_block_stmt(const Block<Token>& block) const
        {
            Resolve::beginScope();
            visit_block_stmt(block);
            Resolve::endScope();
        }

        Token Resolver::visit_for_stmt(const For<Token>& stmt) const
        {
            // visit_for_stmt(stmt.stmt);
            // resolve(stmt.condition);
            // resolve(stmt.body);
        }

        ////////////////////////////////////////////////////////////////////////
        #pragma mark - DECLARATIONS
        ////////////////////////////////////////////////////////////////////////

        Token Resolver::visit_decl_stmt(const DeclStmt<Token> &decl) const
        {
            visit_decl_stmt(decl);
            return Token();
        }

        Token Resolver::visit_decl_var(const DeclVar<Token>& decl) const
        {
            Resolve::declare(decl.identifier);
            if (decl.expr != nullptr) {
                visit_decl_var(decl);
            }
            Resolve::define(decl.identifier);
        }

        Token Resolver::visit_decl_func(const DeclFunc<Token>& decl) const
        {
            Resolve::declare(decl.func->name);
            Resolve::define(decl.func->name);

            // resolve params
            Resolve::beginScope();   

            for (auto param: decl.func->params) {
                Resolve::declare(param);
                Resolve::define(param);
            }

            // resolve block (cant initate abstract class ResolverBlock)
            if (decl.func->blk != nullptr) {
                Resolve::beginScope();
                visit_block_stmt(*decl.func->blk);
                Resolve::endScope();
            }

            Resolve::endScope();
        }
        ////////////////////////////////////////////////////////////////////////
        #pragma mark - PROGRAM
        ////////////////////////////////////////////////////////////////////////

        Tokens Resolver::visit_program(const Program<Tokens>& prgm) const
        {
            // for (auto decl: prgm.decls) {
            //     visit_decl_stmt(*decl);
            // }
        }
    }
}