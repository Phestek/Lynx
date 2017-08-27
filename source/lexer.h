#ifndef LYNX_LEXER_H
#define LYNX_LEXER_H

#include <map>
#include <string>
#include <vector>

#include "token.h"

namespace lynx {

    class Lexer {
    public:
        Lexer(const std::string& filename, std::string&& code);

        std::size_t errors_reported() const noexcept;

        Token next_token() noexcept;
        Token peek_token(int depth = 1) const noexcept;

        bool is_at_end() const;

    private:
        char get_next_character();

        void handle_whitespace(const char c);
        void tokenize_number(char c);
        void tokenize_identifier(char c);
        void tokenize_operator(char c);

        bool is_whitespace(const char c) const noexcept;
        bool is_digit(const char c) const noexcept;
        bool is_identifier_character(const char c) const noexcept;
        bool is_operator_character(const char c) const noexcept;

        const Token::Type* is_keyword(const std::string& identifier) const;
        const Token::Type* is_valid_opearator(const std::string& operator_) const;

        std::string _code;
        std::size_t _code_pos{};
        std::string _filename;
        std::size_t _line{1};
        std::size_t _last_newline{1};
        std::size_t _errors_reported{};

        std::vector<Token> _tokens;
        std::size_t        _current_token{};

        static const std::map<std::string, Token::Type> _KEYWORDS;
        static const std::map<std::string, Token::Type> _OPERATORS;
    };

}

#endif //LYNX_LEXER_H

