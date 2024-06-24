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
        void SetUp() override { this->eval = new Eval(); }
        void TearDown() override { delete this->eval; }

        Parser *parser;
        Eval *eval;
};

#pragma mark - Rift Evaluator (Tests)

TEST_F(RiftEvaluator, simpleEvalExpr) {
    // evaluate -1 + 2
    rift::ast::Binary expr = rift::ast::Binary(
        std::make_unique<rift::ast::Unary>(
            rift::scanner::Token(TokenType::MINUS,"-", "", 1),
            std::make_unique<rift::ast::Literal>(TOK_NUM(1))
        ),
        rift::scanner::Token(TokenType::PLUS,"+", "", 1),
        std::make_unique<rift::ast::Literal>(TOK_NUM(3))
    );

    auto stmt_expr = std::make_unique<StmtExpr>(std::make_unique<rift::ast::Binary>(std::move(expr)));
    auto stmts = std::vector<std::unique_ptr<Stmt>>{std::make_unique<StmtExpr>(std::make_unique<rift::ast::Binary>(std::move(expr)))};
    auto program = std::make_unique<std::vector<std::unique_ptr<rift::ast::Stmt>>>(std::move(stmts));
    auto x = eval->evaluate(std::move(program));
}