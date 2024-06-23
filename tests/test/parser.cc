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
        void SetUp() override { }
        void TearDown() override { }

};

#pragma mark - Rift Parser (Printer Tests)

TEST_F(RiftPrinter, simpleParseExpr) {
    // simple expression for the math operation -1 + 2
    rift::ast::Binary expr = rift::ast::Binary(
        std::make_unique<rift::ast::Unary>(
            rift::scanner::Token(TokenType::MINUS,"-", "", 1),
            std::make_unique<rift::ast::Literal>(Token(TokenType::NUMERICLITERAL, "1", 1, 1))
        ),
        rift::scanner::Token(TokenType::PLUS,"+", "", 1),
        std::make_unique<rift::ast::Literal>(Token(TokenType::NUMERICLITERAL, "2", 2, 1))
    );
    EXPECT_EQ(rift::ast::printer->print(&expr), "(+ (- 1) 2)");
}

TEST_F(RiftPrinter, simpleParseExprGroup) {
    // expression with three groupings
    // two groupings enclosed inside the first grouping ((1 * 2) + 3)

    // [1*2]
    rift::ast::Grouping expr = rift::ast::Grouping(
        std::make_unique<rift::ast::Binary>(
            std::make_unique<rift::ast::Literal>(Token(TokenType::NUMERICLITERAL, "1", 1, 1)),
            rift::scanner::Token(TokenType::STAR,"*", "", 1),
            std::make_unique<rift::ast::Literal>(Token(TokenType::NUMERICLITERAL, "2", 2, 1))
        )
    );

    EXPECT_EQ(rift::ast::printer->print(&expr), "[ (* 1 2)]");

    // 3
    rift::ast::Literal expr2 = rift::ast::Literal(Token(TokenType::NUMERICLITERAL, "3", 3, 1));
    EXPECT_EQ(rift::ast::printer->print(&expr2), "3");

    // ([1*2] + 3)
    rift::ast::Binary expr3 = rift::ast::Binary(
        std::make_unique<rift::ast::Grouping>(std::move(expr)),
        rift::scanner::Token(TokenType::PLUS,"+", "", 1),
        std::make_unique<rift::ast::Literal>(std::move(expr2))
    );
    EXPECT_EQ(rift::ast::printer->print(&expr3), "(+ [ (* 1 2)] 3)");
}