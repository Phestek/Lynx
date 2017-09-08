#include "lexer.h"

#include <cctype>

#include <iostream>
#include <stdexcept>

namespace {

    class Lexer_Error : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

}

namespace lynx {

    const std::map<std::string, Token::Type> Lexer::_KEYWORDS {
        {"func",  Token::Type::FUNC},
        {"let",   Token::Type::LET},
        {"var",   Token::Type::VAR},
        {"if",    Token::Type::IF},
        {"else",  Token::Type::ELSE},
        {"for",   Token::Type::FOR},
        {"while", Token::Type::WHILE},
        {"do",    Token::Type::DO},
        {"true",  Token::Type::TRUE},
        {"false", Token::Type::FALSE},
        {"print", Token::Type::PRINT},
    };

    const std::map<std::string, Token::Type> Lexer::_OPERATORS{
        {"(",  Token::Type::L_PAREN},
        {")",  Token::Type::R_PAREN},
        {"{",  Token::Type::L_BRACE},
        {"}",  Token::Type::R_BRACE},
        {"[",  Token::Type::L_BRACKET},
        {"]",  Token::Type::R_BRACKET},
        {":",  Token::Type::COLON},
        {";",  Token::Type::SEMICOLON},
        {"=",  Token::Type::EQUALS},
        {"+",  Token::Type::PLUS},
        {"-",  Token::Type::MINUS},
        {"*",  Token::Type::STAR},
        {"/",  Token::Type::SLASH},
        {"!",  Token::Type::BANG},
        {"==", Token::Type::EQUALS_EQUALS},
        {"!=", Token::Type::BANG_EQUALS},
        {"<",  Token::Type::LESS},
        {"<=", Token::Type::LESS_EQUALS},
        {">",  Token::Type::GREATER},
        {">=", Token::Type::GREATER_EQUALS},
    };

    Lexer::Lexer(const std::string& filename, std::string&& code)
            : _code{std::move(code)}, _filename{filename} {
        while(_code_pos < _code.length()) {
            try {
                const char c = _code[_code_pos];
                if(is_whitespace(c)) {
                    handle_whitespace(c);
                    continue;
                }
                if(c == '#') {
                    handle_comment(c);
                    continue;
                }
                if(c == '"') {
                    tokenize_string(c);
                    continue;
                }
                if(is_digit(c)) {
                    tokenize_number(c);
                    continue;
                }
                if(is_identifier_character(c)) {
                    tokenize_identifier(c);
                    continue;
                }
                if(is_operator_character(c)) {
                    tokenize_operator(c);
                    continue;
                }
            } catch(const Lexer_Error& e) {
                ++_errors_reported;
                std::cerr << "Error: " << e.what() << ".\n";
                continue;
            }
        }
        _tokens.push_back(Token{Token::Type::END_OF_FILE, "", _filename, _line, _code_pos - _last_newline});
        _tokens.shrink_to_fit();
    }

    std::size_t Lexer::errors_reported() const noexcept {
        return _errors_reported;
    }

    Token Lexer::next_token() noexcept {
        if(_current_token < _tokens.size()) {
            return _tokens[_current_token++];
        }
        return _tokens.back();
    }

    Token Lexer::peek_token(int depth) const noexcept {
        if(_current_token + depth < _tokens.size()) {
            return _tokens[_current_token + depth];
        }
        return _tokens.back();
    }

    bool Lexer::is_at_end() const {
        return peek_token(0).type == Token::Type::END_OF_FILE;
    }

    char Lexer::get_next_character() {
        char c;
        try {
            c = _code.at(++_code_pos);
        } catch(const std::out_of_range& e) {
            c = 0;
        }
        return c;
    }

    void Lexer::handle_whitespace(const char c) {
        if(c == '\n') {
            ++_line;
            _last_newline = _code_pos + 1;
        }
        ++_code_pos;
    }

    void Lexer::handle_comment(char c) {
        while(c != '\n') {
            c = get_next_character();
        }
    }

    void Lexer::tokenize_string(char c) {
        static std::map<char, char> escape_sequences{
                {'\'', '\''},
                {'\"', '\"'},
                {'\?', '\?'},
                {'\\', '\\'},
                {'a', '\a'},
                {'b', '\b'},
                {'f', '\f'},
                {'n', '\n'},
                {'r', '\r'},
                {'t', '\t'},
                {'v', '\v'},
        };
        std::string str{};
        c = get_next_character();
        while(c != '"') {
            if(c == '\\') {
                c = get_next_character();
                if(auto result = escape_sequences.find(c); result != escape_sequences.cend()) {
                    str += result->second;
                } else {
                    ++_code_pos;
                    throw Lexer_Error{"Unknown escape sequence '\\" + std::string{c} + "'"};
                }
            } else {
                str += c;
            }
            c = get_next_character();
        }
        get_next_character();
        _tokens.push_back(Token{Token::Type::STRING, str, _filename, _line, _code_pos - _last_newline});
    }

    // FIXME: Hangs when more than one dot.
    void Lexer::tokenize_number(char c) {
        std::string number{};
        bool is_float = false;
        while(is_digit(c) || c == '.') {
            if(c == '.') {
                if(is_float) {
                    ++_code_pos;    // Skip dot.
                    throw Lexer_Error{"Too many decimal points"};
                }
                is_float = true;
            }
            number += c;
            c = get_next_character();
        }
        if(is_float) {
            _tokens.push_back(Token{Token::Type::FLOAT, number, _filename, _line, _code_pos - _last_newline});
        } else {
            _tokens.push_back(Token{Token::Type::INTEGER, number, _filename, _line, _code_pos - _last_newline});
        }
    }

    void Lexer::tokenize_identifier(char c) {
        std::string identifier{};
        while(is_identifier_character(c)) {
            identifier += c;
            c = get_next_character();
        }
        if(auto keyword = is_keyword(identifier); keyword.has_value()) {
            _tokens.push_back(Token{*keyword, "", _filename, _line, _code_pos - _last_newline});
        } else {
            _tokens.push_back(Token{Token::Type::IDENTIFIER, identifier, _filename, _line, _code_pos - _last_newline});
        }
    }

    void Lexer::tokenize_operator(char c) {
        std::string operator_{};
        std::size_t length{};
        while(is_operator_character(c)) {
            operator_ += c;
            ++length;
            try {
                c = _code.at(_code_pos + length);
            } catch(const std::out_of_range& e) {
                break;
            }
        }
        while(length > 0) {
            if(auto result = is_valid_opearator(operator_); result.has_value()) {
                _code_pos += length;
                _tokens.push_back(Token{*result, "", _filename, _line, _code_pos - _last_newline});
                return;
            }
            operator_.pop_back();
            --length;
        }
        throw Lexer_Error{"Uknown operator \"" + operator_ + "\""};
    }

    bool Lexer::is_whitespace(const char c) const noexcept {
        return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v';
    }

    bool Lexer::is_digit(const char c) const noexcept {
        return c >= '0' && c <= '9';
    }

    bool Lexer::is_identifier_character(const char c) const noexcept {
        if(c >= 'a' && c <= 'z') {
            return true;
        }
        if(c >= 'A' && c <= 'Z') {
            return true;
        }
        if(c >= '0' && c <= '0') {
            return true;
        }
        if(c == '_') {
            return true;
        }
        return false;
    }

    bool Lexer::is_operator_character(const char c) const noexcept {
        return std::ispunct(c);
    }

    std::optional<Token::Type> Lexer::is_keyword(const std::string& identifier) const {
        auto result = _KEYWORDS.find(identifier);
        if(result != _KEYWORDS.cend()) {
            return result->second;
        }
        return {};
    }

    std::optional<Token::Type> Lexer::is_valid_opearator(const std::string& operator_) const {
        auto result = _OPERATORS.find(operator_);
        if(result != _OPERATORS.cend()) {
            return result->second;
        }
        return {};
    }

}

