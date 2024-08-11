#include <string>

#include <gtest/gtest.h>
#include <ast/expr.hh>
#include <ast/parser.hh>
#include <ast/eval.hh>

using namespace rift::ast;
using string = std::string;

#pragma mark - Rift Evaluator (Fixtures)

#define TOK_NUM(n) Token(TokenType::NUMERICLITERAL, #n, n, 1)

/// @note used mostly for sharing allocations
class RiftEvaluator : public ::testing::Test {

    protected:
        RiftEvaluator() {}
        ~RiftEvaluator() override {}
        void SetUp() override { /*this->eval = new Eval(); */ }
        void TearDown() override { /* delete this->eval; */ }

        Parser *parser;
        Eval *eval;
};

#pragma mark - Rift Evaluator (Tests)

TEST_F(RiftEvaluator, simpleEvalExpr) {
    // evaluate -1 + 2
    auto expr = std::make_unique<Binary<Token>>(rift::ast::Binary<Token>(
        std::make_unique<rift::ast::Unary<Token>>(
            rift::scanner::Token(TokenType::MINUS,"-", "", 1),
            std::make_unique<rift::ast::Literal<Token>>(TOK_NUM(1))
        ),
        rift::scanner::Token(TokenType::PLUS,"+", "", 1),
        std::make_unique<rift::ast::Literal<Token>>(TOK_NUM(3))
    ));

    auto stmt_expr = std::make_unique<StmtExpr<void>>(std::move(expr));
    auto decl_stmt = std::make_unique<DeclStmt<Token>>(std::move(stmt_expr));
    // emplace for no copies
    std::vector<std::unique_ptr<Decl<Token>>> decls;
    decls.emplace_back(std::move(decl_stmt));

    // Create a unique_ptr to a vector of unique_ptr<Stmt>
    auto program_statements = std::vector<std::unique_ptr<Decl<Token>>>(std::move(decls));
    auto program = std::make_unique<Program<Tokens>>(std::move(program_statements));
    auto x = eval->evaluate(program, true);
    EXPECT_EQ(x.at(0), "2");
}