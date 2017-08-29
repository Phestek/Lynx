#ifndef LYNX_INTERPRETER_H
#define LYNX_INTERPRETER_H

#include <vector>

#include "statement.h"

namespace lynx {

    class Interpreter final : public Expression_Visitor, public Statement_Visitor {
    public:
        Interpreter(const std::vector<Statement_Ptr>& statements);
        ~Interpreter() = default;

        void interpret();

        Value evaluate(const Expr_Ptr& expression);

        void visit_expression(const Expression& expression) override;
        void visit_function_declaration(const Function_Declaration& function_declaration) override;
        void visit_variable_declaration(const Variable_Declaration& variable_declaration) override;
        
        Value visit_integer(const Integer& integer) override;
        Value visit_float(const Float& floating) override;
        Value visit_string(const String& string) override;
        Value visit_unary(const Unary_Operation& unary) override;
        Value visit_binary(const Binary_Operation& binary) override;

    private:
        const std::vector<Statement_Ptr>& _statements;
    };

}

#endif //LYNX_INTERPRETER_H

