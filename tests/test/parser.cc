#include <scanner/scanner.hh>
#include <ast/expr.hh>
#include <ast/printer.hh>
#include <gtest/gtest.h>

using namespace rift::scanner;
using string = std::string;

#pragma mark - Rift Parser (Printer Fixtures)

/// @note used mostly for sharing allocations
class RiftPrinter : public ::testing::Test {

    protected:
        RiftPrinter() {}
        ~RiftPrinter() override {}
        void SetUp() override { this->printer = new rift::ast::Printer(); }
        void TearDown() override { delete this->printer; }

        rift::ast::Printer *printer;
};

#pragma mark - Rift Parser (Printer Tests)

TEST_F(RiftPrinter, simpleParseExpr) {
    // simple expression for the math operation -1 + 2
    rift::ast::Expr::Binary<string> expr = rift::ast::Expr::Binary<string>(
        std::make_unique<rift::ast::Expr::Unary<string>>(
            rift::scanner::Token(TokenType::MINUS,"-", "", 1),
            std::make_unique<rift::ast::Expr::Literal<string>>("1")
        ),
        rift::scanner::Token(TokenType::PLUS,"+", "", 1),
        std::make_unique<rift::ast::Expr::Literal<std::string>>("2")
    );

    EXPECT_EQ(printer->print(&expr), "(+ (- 1) 2)");
}

TEST_F(RiftPrinter, simpleParseExprGroup) {
    // expression with three groupings
    // two groupings enclosed inside the first grouping ((1 * 2) + 3)

    // [1*2]
    rift::ast::Expr::Grouping<string> expr = rift::ast::Expr::Grouping<string>(
        std::make_unique<rift::ast::Expr::Binary<string>>(
            std::make_unique<rift::ast::Expr::Literal<string>>("1"),
            rift::scanner::Token(TokenType::STAR,"*", "", 1),
            std::make_unique<rift::ast::Expr::Literal<string>>("2")
        )
    );

    EXPECT_EQ(printer->print(&expr), "[ (* 1 2)]");

    // 3
    rift::ast::Expr::Literal<string> expr2 = rift::ast::Expr::Literal<string>("3");
    EXPECT_EQ(printer->print(&expr2), "3");

    // ([1*2] + 3)
    rift::ast::Expr::Binary<string> expr3 = rift::ast::Expr::Binary<string>(
        std::make_unique<rift::ast::Expr::Grouping<string>>(std::move(expr)),
        rift::scanner::Token(TokenType::PLUS,"+", "", 1),
        std::make_unique<rift::ast::Expr::Literal<string>>(std::move(expr2))
    );
    EXPECT_EQ(printer->print(&expr3), "(+ [ (* 1 2)] 3)");
}