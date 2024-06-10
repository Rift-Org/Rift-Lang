#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <scanner/scanner.hh>
#include <ast/expr.hh>
#include <ast/printer.hh>

using namespace rift::scanner;
using namespace rift::ast;
using string = std::string;

#pragma mark - Rift Parser (Mock Fixtures)

/// @note main mock class
class MockVisitorPrinter : public rift::ast::VisitorPrinter {
public:
    MockVisitorPrinter(Printer &printer) : VisitorPrinter(printer) { }

    MOCK_METHOD(std::string, visit_binary, (rift::ast::Expr::Binary<std::string>* expr), (const));
    MOCK_METHOD(std::string, visit_grouping, (rift::ast::Expr::Grouping<std::string>* expr), (const));
    MOCK_METHOD(std::string, visit_literal, (rift::ast::Expr::Literal<std::string>* expr), (const));
    MOCK_METHOD(std::string, visit_unary, (rift::ast::Expr::Unary<std::string>* expr), (const));
};


/// @note fixture
class RiftPrinterMock : public ::testing::Test {
protected:
    void SetUp() override {
        mockVisitor = new MockVisitorPrinter(*new Printer());
    }

    void TearDown() override {
        delete mockVisitor;
    }

    MockVisitorPrinter* mockVisitor;
};


#pragma mark - Rift Parser (Mock Tests)

TEST_F(RiftPrinterMock, simpleParseExpr) {
    // simple expression for the math operation -1 + 2
    rift::ast::Expr::Binary<string> expr = rift::ast::Expr::Binary<string>(
        std::make_unique<rift::ast::Expr::Unary<string>>(
            rift::scanner::Token(TokenType::MINUS,"-", "", 1),
            std::make_unique<rift::ast::Expr::Literal<string>>("1")
        ),
        rift::scanner::Token(TokenType::PLUS,"+", "", 1),
        std::make_unique<rift::ast::Expr::Literal<std::string>>("2")
    );

    EXPECT_CALL(*mockVisitor, visit_unary(testing::_))
        .WillRepeatedly(testing::Invoke([](rift::ast::Expr::Unary<string>* unary) {
            return unary->op.lexeme;
        }));
    EXPECT_CALL(*mockVisitor, visit_literal(testing::_))
        .WillRepeatedly(testing::Invoke([](rift::ast::Expr::Literal<string>* literal) {
            return literal->value;
        }));
    EXPECT_CALL(*mockVisitor, visit_binary(testing::_))
        .WillRepeatedly(testing::Invoke([](rift::ast::Expr::Binary<string>* binary) {
            return binary->op.lexeme;
        }));

    // Test Call Assertions (Mock)
    EXPECT_EQ(mockVisitor->printer->print(&expr), "(+ (- 1) 2)");
}