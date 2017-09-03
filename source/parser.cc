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
                std::cerr << "Error: " << e.token().filename << ':' << e.token().line << ':' << e.token().column << ": "
                        << e.what() << ".\n";
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
        if(match_token(Token::Type::IF)) {
            return if_statement();
        }
        if(match_token(Token::Type::FOR)) {
            return for_statement();
        }
        if(match_token(Token::Type::WHILE)) {
            return while_statement();
        }
        if(match_token(Token::Type::DO)) {
            return do_while_statement();
        }
        if(match_token(Token::Type::PRINT)) {
            return print_statement();
        }
        if(_lexer.peek_token(0).type == Token::Type::L_BRACE) {
            return block();
        }
        auto expr = std::make_unique<Expression>(expression());
        consume(Token::Type::SEMICOLON, "Expected ';' after expression.");
        return expr;
    }

    Statement_Ptr Parser::if_statement() {
        auto condition = expression();
        auto then_branch = block();
        Statement_Ptr else_branch;
        if(match_token(Token::Type::ELSE)) {
            else_branch = std::move(statement());
        }
        return std::make_unique<If>(std::move(condition), std::move(then_branch), std::move(else_branch));
    }

    Statement_Ptr Parser::for_statement() {
    }

    Statement_Ptr Parser::while_statement() {
    }

    Statement_Ptr Parser::do_while_statement() {
    }

    Statement_Ptr Parser::print_statement() {
        auto expr = expression();
        consume(Token::Type::SEMICOLON, "Expected ';' after 'print' statement.");
        return std::make_unique<Print>(std::move(expr));
    }

    Expr_Ptr Parser::expression() {
        return assignment();
    }

    Expr_Ptr Parser::assignment() {
        auto left = factor();
        if(match_token(Token::Type::EQUALS)) {
            auto operator_ = _lexer.peek_token(-1);
            auto right = factor();
            return std::make_unique<Binary_Operation>(std::move(left), operator_, std::move(right));
        }
        return left;
    }

    Expr_Ptr Parser::factor() {
        auto left = term();
        while(match_token(Token::Type::STAR) || match_token(Token::Type::SLASH)) {
            auto operator_ = _lexer.peek_token(-1);
            auto right = factor();
            return std::make_unique<Binary_Operation>(std::move(left), operator_, std::move(right));
        }
        return left;
    }

    Expr_Ptr Parser::term() {
        auto left = unary();
        while(match_token(Token::Type::PLUS) || match_token(Token::Type::MINUS)) {
            auto operator_ = _lexer.peek_token(-1);
            auto right = factor();
            return std::make_unique<Binary_Operation>(std::move(left), operator_, std::move(right));
        }
        return left;
    }

    Expr_Ptr Parser::unary() {
        while(match_token(Token::Type::PLUS) || match_token(Token::Type::MINUS)) {
            auto operator_ = _lexer.peek_token(-1);
            auto operand = factor();
            return std::make_unique<Unary_Operation>(operator_, std::move(operand));
        }
        return primary();
    }

    Expr_Ptr Parser::primary() {
        auto token = _lexer.peek_token(0);
        if(match_token(Token::Type::INTEGER)) {
            return std::make_unique<Literal>(Value{Value::Type::INTEGER, std::stoll(token.value)});
        }
        if(match_token(Token::Type::FLOAT)) {
            return std::make_unique<Literal>(Value{Value::Type::FLOAT, std::stold(token.value)});
        }
        if(match_token(Token::Type::STRING)) {
            return std::make_unique<Literal>(Value{Value::Type::STRING,token.value});
        }
        if(match_token(Token::Type::TRUE)) {
            return std::make_unique<Literal>(Value{Value::Type::BOOL, true});
        }
        if(match_token(Token::Type::FALSE)) {
            return std::make_unique<Literal>(Value{Value::Type::BOOL, false});
        }
        if(match_token(Token::Type::IDENTIFIER)) {
            return std::make_unique<Identifier>(token);
        }
        throw Parse_Error{"Not a primary expression", token};
    }

    Statement_Ptr Parser::block() {
        std::vector<Statement_Ptr> statements;
        consume(Token::Type::L_BRACE, "Every block should start with '{'");
        while(_lexer.peek_token(0).type != Token::Type::R_BRACE && !_lexer.is_at_end()) {
            statements.push_back(declaration());
        }
        consume(Token::Type::R_BRACE, "");
        return std::make_unique<Block>(std::move(statements));
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

