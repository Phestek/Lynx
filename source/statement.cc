#include "statement.h"

namespace lynx {

    Expression::Expression(Expr_Ptr&& expression)
            : expression{std::move(expression)} {
        stmt_type = Type::EXPRESSION;
    }

    Function_Declaration::Function_Declaration(const std::string& name, Block&& body)
            : name{name}, body{std::move(body)} {
        stmt_type = Type::FUNCTION_DECLARATION;
    }

    Variable_Declaration::Variable_Declaration(const bool is_constant, const std::string& identifier,
            const std::string& type, Expr_Ptr&& initializer)
            : is_constant{is_constant}, identifier{identifier}, type{type}, initializer{std::move(initializer)} {
        stmt_type = Type::VARIABLE_DECLARATION;
    }

}

