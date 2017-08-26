#ifndef LYNX_STATEMENT_H
#define LYNX_STATEMENT_H

#include <vector>

#include "expression.h"

namespace lynx {

    struct Statement {
        enum class Type {
            UNDEFINED,
            EXPRESSION,
            FUNCTION_DECLARATION,
            VARIABLE_DECLARATION,
        };
        virtual ~Statement() = default;
        Type stmt_type = Type::UNDEFINED;
    };
    using Statement_Ptr = std::unique_ptr<Statement>;

    using Block = std::vector<Statement_Ptr>;

    struct Expression : Statement {
        Expression(Expr_Ptr&& expression);
        Expr_Ptr expression;
    };

    struct Function_Declaration : Statement {
        Function_Declaration(const std::string& name, Block&& body);
        std::string name;
        Block       body;
        // TODO: Param list.
    };

    struct Variable_Declaration : Statement {
        Variable_Declaration(const bool is_constant, const std::string& identifier, const std::string& type,
                Expr_Ptr&& initializer);
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

}

#endif //LYNX_STATEMENT_H

