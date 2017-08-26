#include "expression.h"

namespace lynx {

    Integer::Integer(const std::string& value)
            : value{value} {
        type = Type::INTEGER;
    }

    Float::Float(const std::string& value)
            : value{value} {
        type = Type::FLOAT;
    }

    Unary_Operation::Unary_Operation(const Token::Type operator_, Expr_Ptr&& operand)
            : operator_{operator_}, operand{std::move(operand)} {
        type = Type::UNARY_OPERATION;
    }

    Binary_Operation::Binary_Operation(Expr_Ptr&& left, const Token::Type operator_, Expr_Ptr&& right)
            : left{std::move(left)}, operator_{operator_}, right{std::move(right)} {
        type = Type::BINARY_OPERATION;
    }

}

