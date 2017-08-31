#ifndef LYNX_TOKEN_H
#define LYNX_TOKEN_H

namespace lynx {

    struct Token {
        enum class Type {
            UNDEFINED,
            INTEGER,
            FLOAT,
            STRING,
            IDENTIFIER,
            // Literals.
            TRUE,
            FALSE,
            // Keywords.
            FUNC,
            LET,
            VAR,
            IF,
            ELSE,
            FOR,
            WHILE,
            DO,
            PRINT,
            // Operators.
            L_PAREN,
            R_PAREN,
            L_BRACE,
            R_BRACE,
            L_BRACKET,
            R_BRACKET,
            COLON,
            SEMICOLON,
            EQUALS,
            PLUS,
            MINUS,
            STAR,
            SLASH,
            END_OF_FILE
        };
        Type        type = Token::Type::UNDEFINED;
        std::string value;
        std::string filename;
        std::size_t line;
        std::size_t column;
    };

}

#endif //LYNX_TOKEN_H

