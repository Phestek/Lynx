#ifndef LYNX_STATEMENT_H
#define LYNX_STATEMENT_H

#include <vector>

#include "expression.h"

namespace lynx {

    class Statement_Visitor;

    struct Statement {
        enum class Type {
            UNDEFINED,
            EXPRESSION,
            FUNCTION_DECLARATION,
            VARIABLE_DECLARATION,
        };

        virtual ~Statement() = default;
        virtual void accept(Statement_Visitor& visitor) = 0;

        Type stmt_type = Type::UNDEFINED;
    };
    using Statement_Ptr = std::unique_ptr<Statement>;

    using Block = std::vector<Statement_Ptr>;

    struct Expression : Statement {
        Expression(Expr_Ptr&& expression);
        void accept(Statement_Visitor& visitor) override;

        Expr_Ptr expression;
    };

    struct Function_Declaration : Statement {
        Function_Declaration(const std::string& name, Block&& body);
        void accept(Statement_Visitor& visitor) override;

        std::string name;
        Block       body;
        // TODO: Param list.
    };

    struct Variable_Declaration : Statement {
        Variable_Declaration(const bool is_constant, const std::string& identifier, const std::string& type,
                Expr_Ptr&& initializer);
        void accept(Statement_Visitor& visitor) override;

        bool        is_constant;
        std::string identifier;
        std::string type;
        Expr_Ptr    initializer;
    };

    struct If : Statement {
    };

    struct For : Statement {
    };

    struct While : Statement {
    };

    struct Do_While : Statement {
    };

    class Statement_Visitor {
    public:
        virtual ~Statement_Visitor() = default;
        virtual void visit_expression(const Expression& expression) = 0;
        virtual void visit_function_declaration(const Function_Declaration& function_declaration) = 0;
        virtual void visit_variable_declaration(const Variable_Declaration& variable_declaration) = 0;
    };

}

#endif //LYNX_STATEMENT_H

