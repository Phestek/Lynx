#include "statement.h"

namespace lynx {

    Block::Block(std::vector<Statement_Ptr>&& statements)
            : statements{std::move(statements)} {
    }

    void Block::accept(Statement_Visitor& visitor) {
        visitor.visit_block(*this);
    }

    Expression::Expression(Expr_Ptr&& expression)
            : expression{std::move(expression)} {
    }

    void Expression::accept(Statement_Visitor& visitor) {
        visitor.visit_expression(*this);
    }

    Function_Declaration::Function_Declaration(const std::string& name, Statement_Ptr&& body)
            : name{name}, body{std::move(body)} {
    }

    void Function_Declaration::accept(Statement_Visitor& visitor) {
        visitor.visit_function_declaration(*this);
    }

    Variable_Declaration::Variable_Declaration(const bool is_constant, const std::string& identifier,
            const std::string& type, Expr_Ptr&& initializer)
            : is_constant{is_constant}, identifier{identifier}, type{type}, initializer{std::move(initializer)} {
    }

    void Variable_Declaration::accept(Statement_Visitor& visitor) {
        visitor.visit_variable_declaration(*this);
    }

    If::If(Expr_Ptr&& condition, Statement_Ptr&& then_block, Statement_Ptr&& else_block)
            : condition{std::move(condition)}, then_block{std::move(then_block)}, else_block{std::move(else_block)} {
    }

    void If::accept(Statement_Visitor& visitor) {
        visitor.visit_if(*this);
    }

    Print::Print(Expr_Ptr&& expression)
            : expression{std::move(expression)} {
    }

    void Print::accept(Statement_Visitor& visitor) {
        return visitor.visit_print(*this);
    }

}

