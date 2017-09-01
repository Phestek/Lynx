#include <gtest/gtest.h>

#include "parser.h"

TEST(Parser, Free_Expression) {
    std::string input{"20+1;"};
    lynx::Lexer lexer{"", std::move(input)};
    lynx::Parser parser{lexer};
    auto result = parser.parse();
    ASSERT_EQ(lexer.errors_reported(), 0);
    ASSERT_EQ(parser.errors_reported(), 0);
    ASSERT_EQ(result[0]->stmt_type, lynx::Statement::Type::EXPRESSION);
    ASSERT_EQ(static_cast<lynx::Expression&>(*result[0]).expression->type, lynx::Expr::Type::BINARY_OPERATION);
}

TEST(Parser, Function_Declaration) {
    std::string input{"func test() {}"};
    lynx::Lexer lexer{"", std::move(input)};
    lynx::Parser parser{lexer};
    auto result = parser.parse();
    ASSERT_EQ(lexer.errors_reported(), 0);
    ASSERT_EQ(parser.errors_reported(), 0);
    ASSERT_EQ(result[0]->stmt_type, lynx::Statement::Type::FUNCTION_DECLARATION);
}

