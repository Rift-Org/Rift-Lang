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
            Assign<Token>* assign = const_cast<Assign<Token>*>(&expr);
            Resolve::resolveLocal(assign, expr.name);
            return  Token();
        }

        Token Resolver::visit_call(const Call<Token>& expr) const
        {
            // expr.accept(*this);
            return Token();
        }

        Token Resolver::visit_var_expr(const VarExpr<Token>& expr) const
        {
            if (!Resolve::scopes.empty() && 
                Resolve::scopes.back().find(expr.value.lexeme)!=Resolve::scopes.back().end() &&
                Resolve::scopes.back().find(expr.value.lexeme)->second == false) {
                error::report(expr.value.line, "resolve_var_expr", "Cannot read local variable in its own initializer.", expr.value, ResolverException("Cannot read local variable in its own initializer."));
            }
            Resolve::resolveLocal(const_cast<VarExpr<Token>*>(&expr), expr.value);
            return Token();
        }

        ////////////////////////////////////////////////////////////////////////
        #pragma mark -  STATEMENTS
        ////////////////////////////////////////////////////////////////////////

        void Resolver::visit_expr_stmt(const StmtExpr<void>& stmt) const
        {
            stmt.expr->accept(*this);
        }

        void Resolver::visit_print_stmt(const StmtPrint<void>& stmt) const
        {
            stmt.expr->accept(*this);
        }

        void Resolver::visit_return_stmt(const StmtReturn<void>& stmt) const
        {
            stmt.expr->accept(*this);
            // visit_return_stmt(stmt);
            // maybe set return token to NIL
        }

        // FIX Compound Statements

        void Resolver::visit_if_stmt(const StmtIf<void>& stmt) const
        {
            // stmt.if_stmt->stmt->accept(*this);
            // stmt.else_stmt->stmt->accept(*this);
        }

        void Resolver::visit_block_stmt(const Block<void>& block) const
        {
            Resolve::beginScope();
            //
            Resolve::endScope();
        }

        void Resolver::visit_for_stmt(const For<void>& stmt) const
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
            decl.stmt->accept(*this);
            return Token();
        }

        Token Resolver::visit_decl_var(const DeclVar<Token>& decl) const
        {
            Resolve::declare(decl.identifier);
            if (decl.expr != nullptr) {
                decl.expr->accept(*this);
            }
            Resolve::define(decl.identifier);
            return Token();
        }

        Token Resolver::visit_decl_class(const DeclClass<Token>& decl) const
        {
            Resolve::declare(decl.identifier);
            Resolve::define(decl.identifier);
            return Token();
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
                // TODO: Fix
                // Resolve::beginScope();
                // auto blk = *decl.func->blk;

                // // Create a new vector to hold the copied declarations
                // Block<Token>::vec_prog copied_decls;

                // // Reserve space in the new vector to avoid reallocations
                // copied_decls.reserve(blk.decls.size());

                // // Copy each declaration
                // for (const auto &decl : blk.decls)
                // {
                //     if (auto var_decl = dynamic_cast<const DeclVar<Token> *>(decl.get()))
                //     {
                //         copied_decls.push_back(std::make_unique<DeclVar<Token>>(
                //             var_decl->identifier,
                //             var_decl->expr ? std::make_unique<Expr<Token>>(*var_decl->expr) : nullptr));
                //     }
                //     else if (auto func_decl = dynamic_cast<const DeclFunc<Token> *>(decl.get()))
                //     {
                //         auto new_func = std::make_unique<DeclFunc<Token>::Func>();
                //         new_func->name = func_decl->func->name;
                //         new_func->params = func_decl->func->params;
                //         new_func->closure = func_decl->func->closure;
                //         new_func->blk = func_decl->func->blk ? std::make_unique<Block<void>>(*func_decl->func->blk) : nullptr;
                //         copied_decls.push_back(std::make_unique<DeclFunc<Token>>(std::move(new_func)));
                //     }
                //     else if (auto stmt_decl = dynamic_cast<const DeclStmt<Token> *>(decl.get()))
                //     {
                //         copied_decls.push_back(std::make_unique<DeclStmt<Token>>(
                //             stmt_decl->stmt ? std::make_unique<Stmt<void>>(*stmt_decl->stmt) : nullptr));
                //     }
                // }

                // Block<Token> new_blk = Block<Token>(std::move(copied_decls));
                // visit_block_stmt(new_blk);
                // Resolve::endScope();
            }

            Resolve::endScope();

            return Token();
        }

        ////////////////////////////////////////////////////////////////////////
        #pragma mark - PROGRAM
        ////////////////////////////////////////////////////////////////////////

        Tokens Resolver::visit_program(const Program<Tokens>& prgm) const
        {
            // for (auto decl: prgm.decls) {
            //     visit_decl_stmt(*decl);
            // }
            return Tokens();
        }
    }
}