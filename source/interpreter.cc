#include "interpreter.h"

#include <iostream>

namespace lynx {

    Interpreter::Interpreter(const std::vector<Statement_Ptr>& statements)
            : _statements{statements} {
    }

    void Interpreter::interpret() {
        for(const auto& statement : _statements) {
            execute(*statement);
        }
    }

    void Interpreter::execute(Statement& expression) {
        expression.accept(*this);
    }

    void Interpreter::execute_block(const Block& block) {
        for(const auto& statement : block.statements) {
            execute(*statement);
        }
    }

    Value Interpreter::evaluate(const Expr_Ptr& expression) {
        return expression->accept(*this);
    }

    void Interpreter::visit_expression(const Expression& expression) {
        const auto result = evaluate(expression.expression);
    }

    void Interpreter::visit_block(const Block& block) {
        for(const auto& statement : block.statements) {
            execute(*statement);
        }
    }

    void Interpreter::visit_function_declaration(const Function_Declaration& function_declaration) {
    }

    void Interpreter::visit_variable_declaration(const Variable_Declaration& variable_declaration) {
    }

    void Interpreter::visit_if(const If& if_stmt) {
        bool execute_then_block = false;
        auto condition_result = evaluate(if_stmt.condition);
        if(condition_result.type == Value::Type::BOOL && std::get<bool>(condition_result.data)) {
            execute_then_block = true;
        } else {
            // TODO: Throw an error.
        }
        if(execute_then_block) {
            execute_block(dynamic_cast<Block&>(*if_stmt.then_block));
        } else {
            if(if_stmt.else_block == nullptr) {
                return;
            }
            execute_block(dynamic_cast<Block&>(*if_stmt.else_block));
        }
    }

    void Interpreter::visit_print(const Print& print) {
        const auto expr = evaluate(print.expression);
        switch(expr.type) {
            case Value::Type::INTEGER:
                std::cout << std::get<long long>(expr.data);
                break;
            case Value::Type::FLOAT:
                std::cout << std::get<long double>(expr.data);
                break;
            case Value::Type::BOOL:
                if(std::get<bool>(expr.data)) {
                    std::cout << "true";
                } else {
                    std::cout << "false";
                }
                break;
            case Value::Type::STRING:
                std::cout << std::get<std::string>(expr.data);
                break;
        }
    }

    Value Interpreter::visit_integer(const Integer& integer) {
        return Value{Value::Type::INTEGER, std::stoll(integer.value)};
    }

    Value Interpreter::visit_float(const Float& floating) {
        return Value{Value::Type::FLOAT, std::stold(floating.value)};
    }

    Value Interpreter::visit_bool(const Bool& boolean) {
        return Value{Value::Type::BOOL, boolean.value};
    }

    Value Interpreter::visit_string(const String& string) {
        return Value{Value::Type::STRING, string.value};
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
        throw std::runtime_error{"Should never reach this point."};
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
        throw std::runtime_error{"Should never reach this point."};
    }

}

