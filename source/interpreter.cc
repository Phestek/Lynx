#include "interpreter.h"

#include <iostream>

namespace lynx {

    Interpreter::Interpreter(const std::vector<Statement_Ptr>& statements)
            : _statements{statements} {
    }

    void Interpreter::interpret() {
        for(const auto& statement : _statements) {
            statement->accept(*this);
        }
    }

    Value Interpreter::evaluate(const Expr_Ptr& expression) {
        return expression->accept(*this);
    }

    void Interpreter::visit_expression(const Expression& expression) {
        const auto result = evaluate(expression.expression);
        if(result.type == Value::Type::INTEGER) {
            std::cout << "Expression: " << std::get<long long>(result.data) << "\n";
        } else if(result.type == Value::Type::FLOAT) {
            std::cout << "Expression: " << std::get<long double>(result.data) << "\n";
        }
    }

    void Interpreter::visit_function_declaration(const Function_Declaration& function_declaration) {
    }

    void Interpreter::visit_variable_declaration(const Variable_Declaration& variable_declaration) {
    }

    Value Interpreter::visit_integer(const Integer& integer) {
        return Value{Value::Type::INTEGER, std::stoll(integer.value)};
    }

    Value Interpreter::visit_float(const Float& floating) {
        return Value{Value::Type::FLOAT, std::stold(floating.value)};
    }

    Value Interpreter::visit_unary(const Unary_Operation& unary) {
        const auto operand = evaluate(unary.operand);
        if(unary.operator_ == Token::Type::MINUS) {
            if(operand.type == Value::Type::INTEGER) {
                return Value{Value::Type::INTEGER, -std::get<long long>(operand.data)};
            }
            if(operand.type == Value::Type::FLOAT) {
                return Value{Value::Type::FLOAT, -std::get<long double>(operand.data)};
            }
            // TODO: Something bad should happen.
        }
    }

    Value Interpreter::visit_binary(const Binary_Operation& binary) {
        const auto left = evaluate(binary.left);
        const auto right = evaluate(binary.right);
        try {
            if(binary.operator_ == Token::Type::PLUS) {
                return left + right;
            }
            if(binary.operator_ == Token::Type::MINUS) {
                return left - right;
            }
            if(binary.operator_ == Token::Type::STAR) {
                return left * right;
            }
            if(binary.operator_ == Token::Type::SLASH) {
                return left / right;
            }
        } catch(const Incompatible_Value_Types& e) {
        }
    }

}

