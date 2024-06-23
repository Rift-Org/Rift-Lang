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
class MockVisitorPrinter : public rift::ast::Visitor {
public:
    MockVisitorPrinter(Printer &printer) {};

    MOCK_METHOD(Token, visit_binary, (const rift::ast::Binary& expr), (const));
    MOCK_METHOD(Token, visit_grouping, (const rift::ast::Grouping& expr), (const));
    MOCK_METHOD(Token, visit_literal, (const rift::ast::Literal& expr), (const));
    MOCK_METHOD(Token, visit_unary, (const rift::ast::Unary& expr), (const));
};


/// @note fixture
class RiftPrinterMock : public ::testing::Test {
protected:
    void SetUp() override {
        printer = new Printer();
        mockVisitor = new MockVisitorPrinter(*printer);
    }

    void TearDown() override {
        delete mockVisitor;
        delete printer;
    }

    MockVisitorPrinter* mockVisitor;
    Printer *printer;
};


#pragma mark - Rift Parser (Mock Tests)

TEST_F(RiftPrinterMock, simpleParseExpr) {
    // simple expression for the math operation -1 + 2
    rift::ast::Binary expr = rift::ast::Binary(
        std::make_unique<rift::ast::Unary>(
            rift::scanner::Token(TokenType::MINUS,"-", "", 1),
            std::make_unique<rift::ast::Literal>("1")
        ),
        rift::scanner::Token(TokenType::PLUS,"+", "", 1),
        std::make_unique<rift::ast::Literal>("2")
    );

    EXPECT_CALL(*mockVisitor, visit_unary(testing::_))
        .WillRepeatedly(testing::Invoke([](const rift::ast::Unary& unary) {
            return unary.op;
        }));
    EXPECT_CALL(*mockVisitor, visit_literal(testing::_))
        .WillRepeatedly(testing::Invoke([](const rift::ast::Literal& literal) {
            return literal.value;
        }));
    EXPECT_CALL(*mockVisitor, visit_binary(testing::_))
        .WillRepeatedly(testing::Invoke([](const rift::ast::Binary& binary) {
            return binary.op;
        }));

    // Test Call Assertions (Mock)
    EXPECT_EQ(printer->print(&expr), "(+ (- 1) 2)");
}