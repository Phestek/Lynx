#ifndef LYNX_EXPRESSION_H
#define LYNX_EXPRESSION_H

#include <memory>
#include <string>

#include "token.h"
#include "value.h"

namespace lynx {

    class Expression_Visitor;

    struct Expr {
        virtual ~Expr() = default;
        virtual Value accept(Expression_Visitor& visitor) = 0;
    };
    using Expr_Ptr = std::unique_ptr<Expr>;

    struct Literal : Expr {
        Literal(const Value& value);
        Value accept(Expression_Visitor& visitor) override;

        Value value;
    };

    struct Identifier : Expr {
        Identifier(const Token value);
        Value accept(Expression_Visitor& visitor) override;

        Token name;
    };

    struct Unary_Operation : Expr {
        Unary_Operation(const Token operator_, Expr_Ptr&& operand);
        Value accept(Expression_Visitor& visitor) override;
        
        Token       operator_;
        Expr_Ptr    operand;
    };

    struct Binary_Operation : Expr {
        Binary_Operation(Expr_Ptr&& left, const Token operator_, Expr_Ptr&& right);
        Value accept(Expression_Visitor& visitor) override;
        
        Expr_Ptr    left;
        Token       operator_;
        Expr_Ptr    right;
    };

    class Expression_Visitor {
    public:
        virtual ~Expression_Visitor() = default;
        virtual Value visit_literal(const Literal& floating) = 0;
        virtual Value visit_identifier(const Identifier& identifier) = 0;
        virtual Value visit_unary(const Unary_Operation& unary) = 0;
        virtual Value visit_binary(const Binary_Operation& binary) = 0;
    };

}

#endif //LYNX_EXPRESSION_H

