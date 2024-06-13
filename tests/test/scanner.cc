#include <initializer_list>
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
        void SetUp() override { this->scanner = new Scanner({}); }
        void TearDown() override { delete this->scanner; }
        Scanner *scanner;
};

#pragma mark - Rift Scanner (Tests)

TEST_F(RiftScanner, simpleScanner)
{
    scanner->source = {};
    scanner->source.push_back('1');
    scanner->source.push_back('+');
    scanner->source.push_back('2');
    scanner->scan_source();
    auto &tokens = scanner->tokens;

    EXPECT_EQ(tokens[0].lexeme, "1");
    EXPECT_EQ(tokens[1].lexeme, "+");
    EXPECT_EQ(tokens[2].lexeme, "2");
}