#include <gtest/gtest.h>

#include "lexer.h"

TEST(Lexer, Identifier) {
    std::string input{"identifier"};
    lynx::Lexer lexer{std::move(input)};
    const auto token = lexer.next_token();
    ASSERT_EQ(token.type, lynx::Token::Type::IDENTIFIER);
    ASSERT_EQ(token.value, "identifier");
}

TEST(Lexer, Keywords) {
    std::string input{"func let var"};
    lynx::Lexer lexer{std::move(input)};
    ASSERT_EQ(lexer.next_token().type, lynx::Token::Type::FUNC);
    ASSERT_EQ(lexer.next_token().type, lynx::Token::Type::LET);
    ASSERT_EQ(lexer.next_token().type, lynx::Token::Type::VAR);
}

TEST(Lexer, Numbers) {
    std::string input{"6453 23.6"};
    lynx::Lexer lexer{std::move(input)};
    const auto token1 = lexer.next_token();
    ASSERT_EQ(token1.type, lynx::Token::Type::INTEGER);
    ASSERT_EQ(token1.value, "6453");
    const auto token2 = lexer.next_token();
    ASSERT_EQ(token2.type, lynx::Token::Type::FLOAT);
    ASSERT_EQ(token2.value, "23.6");
}

TEST(Lexer, Brackets) {
    std::string input{"(){}[]"};
    lynx::Lexer lexer{std::move(input)};
    ASSERT_EQ(lexer.next_token().type, lynx::Token::Type::L_PAREN);
    ASSERT_EQ(lexer.next_token().type, lynx::Token::Type::R_PAREN);
    ASSERT_EQ(lexer.next_token().type, lynx::Token::Type::L_BRACE);
    ASSERT_EQ(lexer.next_token().type, lynx::Token::Type::R_BRACE);
    ASSERT_EQ(lexer.next_token().type, lynx::Token::Type::L_BRACKET);
    ASSERT_EQ(lexer.next_token().type, lynx::Token::Type::R_BRACKET);
}

