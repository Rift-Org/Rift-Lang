#include <scanner/scanner.hh>
#include <ast/expr.hh>
#include <ast/printer.hh>
#include <gtest/gtest.h>

using namespace rift::scanner;
using string = std::string;

#pragma mark - Rift Scanner (Fixtures)

/// @note used mostly for sharing allocations
class RiftScanner : public ::testing::Test {

    protected:
        RiftScanner() {}
        ~RiftScanner() override {}
        void SetUp() override { this->scanner = new Scanner(""); }
        void TearDown() override { delete this->scanner; }
        Scanner *scanner;
};

#pragma mark - Rift Scanner (Tests)

TEST_F(RiftScanner, simpleScanner) { 
    scanner->source = "1 + 2";
    scanner->scan_source();
    auto token = scanner->tokens;

    EXPECT_EQ(token[0].lexeme, "1");
    EXPECT_EQ(token[1].lexeme, "+");
    EXPECT_EQ(token[2].lexeme, "2");
}