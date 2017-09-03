#include "expression.h"

namespace lynx {

    Literal::Literal(const Value& value)
            : value{value} {
    }

    Value Literal::accept(Expression_Visitor& visitor) {
        return visitor.visit_literal(*this);
    }

    Identifier::Identifier(const Token name)
            : name{name} {
    }

    Value Identifier::accept(Expression_Visitor& visitor) {
        return visitor.visit_identifier(*this);
    }

    Unary_Operation::Unary_Operation(const Token operator_, Expr_Ptr&& operand)
            : operator_{operator_}, operand{std::move(operand)} {
    }

    Value Unary_Operation::accept(Expression_Visitor& visitor) {
        return visitor.visit_unary(*this);
    }

    Binary_Operation::Binary_Operation(Expr_Ptr&& left, const Token operator_, Expr_Ptr&& right)
            : left{std::move(left)}, operator_{operator_}, right{std::move(right)} {
    }

    Value Binary_Operation::accept(Expression_Visitor& visitor) {
        return visitor.visit_binary(*this);
    }

}

