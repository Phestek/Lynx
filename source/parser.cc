#include "parser.h"

#include <iostream>
#include <stdexcept>

namespace lynx {

    namespace {

        class Parse_Error : public std::runtime_error {
        public:
            explicit Parse_Error(const std::string& message, const lynx::Token token)
                    : std::runtime_error{message}, _token{token} {
            }

            const Token& token() const {
                return _token;
            }

        private:
            Token _token;
        };

    }

    Parser::Parser(Lexer& lexer)
            : _lexer{lexer} {
    }

    std::size_t Parser::errors_reported() const noexcept {
        return _errors_reported;
    }

    std::vector<std::unique_ptr<Statement>> Parser::parse() {
        std::vector<std::unique_ptr<Statement>> statements;
        while(!_lexer.is_at_end()) {
            try {
                statements.push_back(declaration());
            } catch(const Parse_Error& e) {
                std::cerr << "Error: <filename>:" << e.token().line << ":<column>: " << e.what() << ".\n";
                ++_errors_reported;
                synchronize();
            }
        }
        return statements;
    }

    Statement_Ptr Parser::declaration() {
        if(match_token(Token::Type::FUNC)) {
            return function_declaration();
        }
        if(match_token(Token::Type::LET)) {
            return variable_declaration(true);
        }
        if(match_token(Token::Type::VAR)) {
            return variable_declaration(false);
        }
        return statement();
    }

    Statement_Ptr Parser::function_declaration() {
        auto name = consume(Token::Type::IDENTIFIER, "Expected an identifier after 'func' declaration");
        consume(Token::Type::L_PAREN, "");
        consume(Token::Type::R_PAREN, "");
        auto body = block();
        return std::make_unique<Function_Declaration>(name.value, std::move(body));
    }

    Statement_Ptr Parser::variable_declaration(const bool is_constant) {
        auto identifier = consume(Token::Type::IDENTIFIER, "Expected identifier after 'var' and 'let'").value;
        // TODO: Does not support type inference yet.
        consume(Token::Type::COLON, "");
        auto type = consume(Token::Type::IDENTIFIER, "").value;
        Expr_Ptr initializer{};
        if(match_token(Token::Type::EQUALS)) {
            initializer = std::move(expression());
        }
        consume(Token::Type::SEMICOLON, "Expected ';' after variable declaration");
        return std::make_unique<Variable_Declaration>(is_constant, identifier, type, std::move(initializer));
    }

    Statement_Ptr Parser::statement() {
        return std::make_unique<Expression>(expression());
    }

    Expr_Ptr Parser::expression() {
        return assignment();
    }

    Expr_Ptr Parser::assignment() {
        auto left = factor();
        if(match_token(Token::Type::EQUALS)) {
            auto operator_ = _lexer.peek_token(-1).type;
            auto right = factor();
            return std::make_unique<Binary_Operation>(std::move(left), operator_, std::move(right));
        }
        return left;
    }

    Expr_Ptr Parser::factor() {
        auto left = term();
        while(match_token(Token::Type::STAR) || match_token(Token::Type::SLASH)) {
            auto operator_ = _lexer.peek_token(-1).type;
            auto right = factor();
            return std::make_unique<Binary_Operation>(std::move(left), operator_, std::move(right));
        }
        return left;
    }

    Expr_Ptr Parser::term() {
        auto left = unary();
        while(match_token(Token::Type::PLUS) || match_token(Token::Type::MINUS)) {
            auto operator_ = _lexer.peek_token(-1).type;
            auto right = factor();
            return std::make_unique<Binary_Operation>(std::move(left), operator_, std::move(right));
        }
        return left;
    }

    Expr_Ptr Parser::unary() {
        while(match_token(Token::Type::PLUS) || match_token(Token::Type::MINUS)) {
            auto operator_ = _lexer.peek_token(-1).type;
            auto operand = factor();
            return std::make_unique<Unary_Operation>(operator_, std::move(operand));
        }
        return primary();
    }

    Expr_Ptr Parser::primary() {
        if(match_token(Token::Type::INTEGER)) {
            return std::make_unique<Integer>(_lexer.peek_token(0).value);
        }
        if(match_token(Token::Type::FLOAT)) {
            return std::make_unique<Float>(_lexer.peek_token(0).value);
        }
        throw Parse_Error{"Not a primary expression", _lexer.peek_token(0)};
    }

    Block Parser::block() {
        Block block{};
        consume(Token::Type::L_BRACE, "");
        while(_lexer.peek_token(0).type != Token::Type::R_BRACE && !_lexer.is_at_end()) {
            block.push_back(declaration());
        }
        consume(Token::Type::R_BRACE, "");
        return block;
    }

    bool Parser::match_token(const Token::Type type) {
        if(_lexer.peek_token(0).type == type) {
            _lexer.next_token();
            return true;
        }
        return false;
    }
    
    Token Parser::consume(const Token::Type type, const std::string& fail_msg) {
        const auto token = _lexer.next_token();
        if(token.type != type) {
            throw Parse_Error{fail_msg, token};
        }
        return token;
    }

    void Parser::synchronize() {
        _lexer.next_token();
        while(!_lexer.is_at_end()) {
            if(_lexer.peek_token(-1).type == Token::Type::SEMICOLON) {
                return;
            }
            switch(_lexer.peek_token(0).type) {
                case Token::Type::FUNC:
                case Token::Type::LET:
                case Token::Type::VAR:
                case Token::Type::IF:
                case Token::Type::FOR:
                case Token::Type::WHILE:
                case Token::Type::DO: {
                    return;
                }
                default: {
                    _lexer.next_token();
                    continue;
                }
            }
        }
    }

}

