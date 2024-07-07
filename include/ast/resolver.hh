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

#include <unordered_map>
#include <stack>
#include <ast/eval.hh>

using std::stack;
using std::unordered_map;
using std::string;
using std::vector;

namespace rift
{
    namespace ast
    {
        
        template <typename T>
        class Resolver : public Accept<T>
        {
            public:
                Resolver() = default;

                enum FunctionType
                {
                    NONE,
                    FUNCTION,
                    INITALIZER,
                    METHOD
                };

                friend class Visitor;

                virtual T accept(const Visitor &visitor) const override = 0;
                virtual ~Resolver() = default;

            private:
                FunctionType f_type = NONE;
        };

        // EXPRESSIONS

        class ResolverAssign: public Resolver<Token>
        {
            public:
                ResolverAssign(Assign &expr) : expr(expr) {};

                Assign &expr;
                Token accept(const Visitor &visitor) const override { return visitor.resolve_assign(*this); }
                ~ResolverAssign() = default;
        };

        class ResolverBinary: public Resolver<Token>
        {
            public:
                ResolverBinary(Binary &expr) : expr(expr) {};

                Binary &expr;
                Token accept(const Visitor &visitor) const override { return visitor.resolve_binary(*this); }
                ~ResolverBinary() = default;
        };

        class ResolverGrouping: public Resolver<Token>
        {
            public:
                ResolverGrouping(Grouping &expr) : expr(expr) {};

                Grouping &expr;
                Token accept(const Visitor &visitor) const override { return visitor.resolve_grouping(*this); }
                ~ResolverGrouping() = default;
        };

        class ResolverUnary: public Resolver<Token>
        {
            public:
                ResolverUnary(Unary &expr) : expr(expr) {};

                Unary &expr;
                Token accept(const Visitor &visitor) const override { return visitor.resolve_unary(*this); }
                ~ResolverUnary() = default;
        };

        class ResolverTernary: public Resolver<Token>
        {
            public:
                ResolverTernary(Ternary &expr) : expr(expr) {};

                Ternary &expr;
                Token accept(const Visitor &visitor) const override { return visitor.resolve_ternary(*this); }
                ~ResolverTernary() = default;
        };

        class ResolverCall: public Resolver<Token>
        {
            public:
                ResolverCall(Call &expr) : expr(expr) {};

                Call &expr;
                Token accept(const Visitor &visitor) const override { return visitor.resolve_call(*this); }
                ~ResolverCall() = default;
        };

        class ResolverVarExpr: public Resolver<Token>
        {
            public:
                ResolverVarExpr(VarExpr &expr) : expr(expr) {};

                VarExpr &expr;
                Token accept(const Visitor &visitor) const override { return visitor.resolve_var_expr(*this); }
                ~ResolverVarExpr() = default;
        };

        class ResolverLiteral: public Resolver<Token>
        {
            public:
                ResolverLiteral(Literal &expr) : expr(expr) {};

                Literal &expr;
                Token accept(const Visitor &visitor) const override { return visitor.resolve_literal(*this); }
                ~ResolverLiteral() = default;
        };

        // STATEMENTS

        class ResolverStmtExpr: public Resolver<Token>
        {
            public:
                ResolverStmtExpr(StmtExpr &stmt): stmt(stmt) {};
                
                StmtExpr &stmt;
                Token accept(const Visitor &visitor) const override { return visitor.resolve_expr_stmt(*this); }
                ~ResolverStmtExpr() = default;
        };

        class ResolverPrint: public Resolver<Token>
        {
            public:
                ResolverPrint(StmtPrint &stmt): stmt(stmt) {};

                StmtPrint &stmt;
                Token accept(const Visitor &visitor) const override { return visitor.resolve_print_stmt(*this); }
                ~ResolverPrint() = default;
        };

        class ResolverIf: public Resolver<Token>
        {
            public:
                ResolverIf(StmtIf &stmt): stmt(stmt) {};

                StmtIf &stmt;
                Token accept(const Visitor &visitor) const override { return visitor.resolve_if_stmt(*this); }
                ~ResolverIf() = default;
        };

        class ResolverReturn: public Resolver<Token>
        {
            public:
                ResolverReturn(StmtReturn &stmt): stmt(stmt) {};

                StmtReturn &stmt;
                Token accept(const Visitor &visitor) const override { return visitor.resolve_return_stmt(*this); }
                ~ResolverReturn() = default;
        };

        class ResolverBlock: public Resolver<Token>
        {
            public:
                ResolverBlock(Block &block): block(block) {};

                Block &block;
                Token accept(const Visitor &visitor) const override { return visitor.resolve_block_stmt(*this); }
                ~ResolverBlock() = default;
        };

        class ResolverFor: public Resolver<Token>
        {
            public:
                ResolverFor(For &stmt): stmt(stmt) {};

                For &stmt;
                Token accept(const Visitor &visitor) const override { return visitor.resolve_for_stmt(*this); }
                ~ResolverFor() = default;
        };

        // DECLARATIONS

        class ResolverDeclVar: public Resolver<Tokens>
        {
            public:
                ResolverDeclVar(DeclVar &decl): decl(decl) {};

                DeclVar &decl;
                Tokens accept(const Visitor &visitor) const override { return visitor.resolve_decl_var(*this); }
                ~ResolverDeclVar() = default;
        };

        class ResolverDeclFunc: public Resolver<Tokens>
        {
            public:
                ResolverDeclFunc(DeclFunc &decl): decl(decl) {};

                DeclFunc &decl;
                Tokens accept(const Visitor &visitor) const override { return visitor.resolve_decl_func(*this); }
                ~ResolverDeclFunc() = default;
        };

        class ResolverDeclStmt : public Resolver<Tokens>
        {
            public:
                ResolverDeclStmt(DeclStmt &decl): decl(decl) {};

                DeclStmt &decl;
                Tokens accept(const Visitor &visitor) const override { return visitor.resolve_decl_stmt(*this); }
                ~ResolverDeclStmt() = default;
        };

        // PROGRAM

        class ResolverProgram : public Resolver<Tokens>
        {
            public:
                ResolverProgram(Program &prgm): prgm(prgm) {};

                Program &prgm;
                Tokens accept(const Visitor &visitor) const override { return visitor.resolve_program(*this); }
                ~ResolverProgram() = default;
        };

        // ERROR
        class ResolverException : public std::exception
        {
            public:
                ResolverException(const string &message): message(message) {};
                const char *what() const noexcept override { return message.c_str(); }
            private:
                string message;
        };


    }
}