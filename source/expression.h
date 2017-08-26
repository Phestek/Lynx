#ifndef LYNX_EXPRESSION_H
#define LYNX_EXPRESSION_H

#include <memory>
#include <string>

#include "token.h"

namespace lynx {

    struct Expr {
        enum class Type {
            UNDEFINED,
            INTEGER,
            FLOAT,
            UNARY_OPERATION,
            BINARY_OPERATION
        };
        virtual ~Expr() = default;
        Type type = Type::UNDEFINED;
    };
    using Expr_Ptr = std::unique_ptr<Expr>;

    struct Integer : Expr {
        Integer(const std::string& value);
        std::string value;
    };

    struct Float : Expr {
        Float(const std::string& value);
        std::string value;
    };

    struct Unary_Operation : Expr {
        Unary_Operation(const Token::Type operator_, Expr_Ptr&& operand);
        Token::Type operator_;
        Expr_Ptr    operand;
    };

    struct Binary_Operation : Expr {
        Binary_Operation(Expr_Ptr&& left, const Token::Type operator_, Expr_Ptr&& right);
        Expr_Ptr    left;
        Token::Type operator_;
        Expr_Ptr    right;
    };

}

#endif //LYNX_EXPRESSION_H

