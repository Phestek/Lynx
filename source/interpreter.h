#ifndef LYNX_INTERPRETER_H
#define LYNX_INTERPRETER_H

#include <vector>

#include "environment.h"
#include "statement.h"

namespace lynx {

    class Interpreter final : public Expression_Visitor, public Statement_Visitor {
    public:
        Interpreter(const std::vector<Statement_Ptr>& statements);
        ~Interpreter() = default;

        bool interpret();

        void execute(Statement& expression);
        void execute_block(const Block& block);
        Value evaluate(const Expr_Ptr& expression);

        void visit_block(const Block& block) override;
        void visit_expression(const Expression& expression) override;
        void visit_function_declaration(const Function_Declaration& function_declaration) override;
        void visit_variable_declaration(const Variable_Declaration& variable_declaration) override;
        void visit_if(const If& if_stmt) override;
        void visit_print(const Print& print) override;
        
        Value visit_literal(const Literal& literal) override;
        Value visit_identifier(const Identifier& identifier) override;
        Value visit_unary(const Unary_Operation& unary) override;
        Value visit_binary(const Binary_Operation& binary) override;

    private:
        const std::vector<Statement_Ptr>& _statements;

        Environment _environment;
    };

}

#endif //LYNX_INTERPRETER_H

