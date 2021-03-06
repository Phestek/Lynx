#include <gtest/gtest.h>

#include "parser.h"

TEST(Parser, Free_Expression) {
    std::string input{"20+1;"};
    lynx::Lexer lexer{"", std::move(input)};
    lynx::Parser parser{lexer};
    auto result = parser.parse();
    ASSERT_EQ(lexer.errors_reported(), 0);
    ASSERT_EQ(parser.errors_reported(), 0);
}

TEST(Parser, Function_Declaration) {
    std::string input{"func test() {}"};
    lynx::Lexer lexer{"", std::move(input)};
    lynx::Parser parser{lexer};
    auto result = parser.parse();
    ASSERT_EQ(lexer.errors_reported(), 0);
    ASSERT_EQ(parser.errors_reported(), 0);
}

