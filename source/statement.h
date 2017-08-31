#ifndef LYNX_STATEMENT_H
#define LYNX_STATEMENT_H

#include <vector>

#include "expression.h"

namespace lynx {

    class Statement_Visitor;

    struct Statement {
        enum class Type {
            UNDEFINED,
            BLOCK,
            EXPRESSION,
            FUNCTION_DECLARATION,
            VARIABLE_DECLARATION,
            IF,
            FOR,
            WHILE,
            DO_WHILE,
            PRINT
        };

        virtual ~Statement() = default;
        virtual void accept(Statement_Visitor& visitor) = 0;

        Type stmt_type = Type::UNDEFINED;
    };
    using Statement_Ptr = std::unique_ptr<Statement>;

    struct Block : Statement {
        Block(std::vector<Statement_Ptr>&& statements);
        void accept(Statement_Visitor& visitor) override;

        std::vector<Statement_Ptr> statements;
    };

    struct Expression : Statement {
        Expression(Expr_Ptr&& expression);
        void accept(Statement_Visitor& visitor) override;

        Expr_Ptr expression;
    };

    struct Function_Declaration : Statement {
        Function_Declaration(const std::string& name, Statement_Ptr&& body);
        void accept(Statement_Visitor& visitor) override;

        std::string   name;
        Statement_Ptr body;
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
        If(Expr_Ptr&& condition, Statement_Ptr&& then_block, Statement_Ptr&& else_block);
        void accept(Statement_Visitor& visitor) override;

        Expr_Ptr      condition;
        Statement_Ptr then_block;
        Statement_Ptr else_block;
    };

    struct For : Statement {
    };

    struct While : Statement {
    };

    struct Do_While : Statement {
    };

    struct Print : Statement {
        Print(Expr_Ptr&& expression);
        void accept(Statement_Visitor& visitor) override;

        Expr_Ptr expression;
    };

    class Statement_Visitor {
    public:
        virtual ~Statement_Visitor() = default;
        virtual void visit_block(const Block& block) = 0;
        virtual void visit_expression(const Expression& expression) = 0;
        virtual void visit_function_declaration(const Function_Declaration& function_declaration) = 0;
        virtual void visit_variable_declaration(const Variable_Declaration& variable_declaration) = 0;
        virtual void visit_if(const If& if_stmt) = 0;
        virtual void visit_print(const Print& print) = 0;
    };

}

#endif //LYNX_STATEMENT_H

