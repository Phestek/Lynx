#ifndef LYNX_TOKEN_H
#define LYNX_TOKEN_H

namespace lynx {

    struct Token {
        enum class Type {
            UNDEFINED,
            INTEGER,
            FLOAT,
            IDENTIFIER,
            // Keywords.
            FUNC,
            LET,
            VAR,
            // Operators.
            L_PAREN,
            R_PAREN,
            L_BRACE,
            R_BRACE,
            L_BRACKET,
            R_BRACKET,
            EQUALS,
            END_OF_FILE
        };
        Type        type = Token::Type::UNDEFINED;
        std::string value;
        std::size_t line;
    };

}

#endif //LYNX_TOKEN_H

