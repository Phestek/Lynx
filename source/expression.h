#ifndef LYNX_EXPRESSION_H
#define LYNX_EXPRESSION_H

#include <memory>
#include <string>

#include "token.h"
#include "value.h"

namespace lynx {

    class Expression_Visitor;

    struct Expr {
        enum class Type {
            UNDEFINED,
            INTEGER,
            FLOAT,
            STRING,
            UNARY_OPERATION,
            BINARY_OPERATION
        };

        virtual ~Expr() = default;
        virtual Value accept(Expression_Visitor& visitor) = 0;
        
        Type type = Type::UNDEFINED;
    };
    using Expr_Ptr = std::unique_ptr<Expr>;

    struct Integer : Expr {
        Integer(const std::string& value);
        Value accept(Expression_Visitor& visitor) override;

        std::string value;
    };

    struct Float : Expr {
        Float(const std::string& value);
        Value accept(Expression_Visitor& visitor) override;
        
        std::string value;
    };

    struct String : Expr {
        String(const std::string& value);
        Value accept(Expression_Visitor& visitor) override;
        
        std::string value;
    };


    struct Unary_Operation : Expr {
        Unary_Operation(const Token::Type operator_, Expr_Ptr&& operand);
        Value accept(Expression_Visitor& visitor) override;
        
        Token::Type operator_;
        Expr_Ptr    operand;
    };

    struct Binary_Operation : Expr {
        Binary_Operation(Expr_Ptr&& left, const Token::Type operator_, Expr_Ptr&& right);
        Value accept(Expression_Visitor& visitor) override;
        
        Expr_Ptr    left;
        Token::Type operator_;
        Expr_Ptr    right;
    };

    class Expression_Visitor {
    public:
        virtual ~Expression_Visitor() = default;
        virtual Value visit_integer(const Integer& integer) = 0;
        virtual Value visit_float(const Float& floating) = 0;
        virtual Value visit_string(const String& floating) = 0;
        virtual Value visit_unary(const Unary_Operation& unary) = 0;
        virtual Value visit_binary(const Binary_Operation& binary) = 0;
    };

}

#endif //LYNX_EXPRESSION_H

