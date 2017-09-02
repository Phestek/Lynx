#ifndef LYNX_PARSER_H
#define LYNX_PARSER_H

#include "lexer.h"
#include "statement.h"

namespace lynx {

    // Parser is using recursive descent parsing to parse both statements and expressions.
    class Parser {
    public:
        Parser(Lexer& lexer);

        std::size_t errors_reported() const noexcept;

        std::vector<Statement_Ptr> parse();

        Statement_Ptr declaration();
        Statement_Ptr function_declaration();
        Statement_Ptr variable_declaration(const bool is_constant);

        Statement_Ptr statement();
        Statement_Ptr if_statement();
        Statement_Ptr for_statement();
        Statement_Ptr while_statement();
        Statement_Ptr do_while_statement();
        Statement_Ptr print_statement();

        Expr_Ptr expression();
        Expr_Ptr assignment();
        Expr_Ptr factor();
        Expr_Ptr term();
        Expr_Ptr unary();
        Expr_Ptr primary();

        Statement_Ptr block();

    private:
        bool match_token(const Token::Type type);
        Token consume(const Token::Type type, const std::string& fail_msg);

        void synchronize();
        
        Lexer&      _lexer;
        std::size_t _errors_reported{};
    };

}

#endif //LYNX_PARSER_H

