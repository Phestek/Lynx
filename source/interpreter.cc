#include "interpreter.h"

#include <iostream>

namespace lynx {

    Interpreter::Interpreter(const std::vector<Statement_Ptr>& statements)
            : _statements{statements} {
    }

    bool Interpreter::interpret() {
        try {
            for(const auto& statement : _statements) {
                execute(*statement);
            }
        } catch(const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << ".\n";
            return false;
        }
        return true;
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
        _environment.define(variable_declaration.identifier, evaluate(variable_declaration.initializer));
    }

    void Interpreter::visit_if(const If& if_stmt) {
        bool execute_then_block = false;
        auto condition_result = evaluate(if_stmt.condition);
        if(is_truthy(condition_result)) {
            execute_then_block = true;
        }
        if(execute_then_block) {
            execute(*if_stmt.then_block);
            return;
        }
        if(dynamic_cast<Block*>(if_stmt.else_block.get()) != nullptr
                || dynamic_cast<If*>(if_stmt.else_block.get()) != nullptr) {
            execute(*if_stmt.else_block);
            return;
        }
        throw std::runtime_error{"Expected 'if' or block after 'else'"};
    }

    void Interpreter::visit_for(const For& for_stmt) {
    }

    void Interpreter::visit_while(const While& while_stmt) {
    }

    void Interpreter::visit_do_while(const Do_While& do_while) {
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

    Value Interpreter::visit_literal(const Literal& literal) {
        return literal.value;
    }

    Value Interpreter::visit_identifier(const Identifier& identifier) {
        return _environment.get(identifier.name.value);
    }

    Value Interpreter::visit_unary(const Unary_Operation& unary) {
        const auto operand = evaluate(unary.operand);
        if(unary.operator_.type == Token::Type::MINUS) {
            if(operand.type == Value::Type::INTEGER) {
                return Value{Value::Type::INTEGER, -std::get<long long>(operand.data)};
            }
            if(operand.type == Value::Type::FLOAT) {
                return Value{Value::Type::FLOAT, -std::get<long double>(operand.data)};
            }
            // TODO: Something bad should happen.
        }
        if(unary.operator_.type == Token::Type::BANG) {
            if(operand.type != Value::Type::BOOL) {
                throw std::runtime_error{"Unary '!' may only be used on 'bool' types"};
            }
            return Value{Value::Type::BOOL, !std::get<bool>(operand.data)};
        }
        throw std::runtime_error{"Should never reach this point."};
    }

    Value Interpreter::visit_binary(const Binary_Operation& binary) {
        const auto left = evaluate(binary.left);
        const auto right = evaluate(binary.right);
        if(left.type != right.type) {
            throw std::runtime_error{"Incompatible operands in binary operation"};
        }
        if(binary.operator_.type == Token::Type::PLUS) {
            return left + right;
        }
        if(binary.operator_.type == Token::Type::MINUS) {
            return left - right;
        }
        if(binary.operator_.type == Token::Type::STAR) {
            return left * right;
        }
        if(binary.operator_.type == Token::Type::SLASH) {
            return left / right;
        }
        if(binary.operator_.type == Token::Type::EQUALS_EQUALS) {
            return left == right;
        }
        if(binary.operator_.type == Token::Type::BANG_EQUALS) {
            return left != right;
        }
        if(binary.operator_.type == Token::Type::LESS) {
            return left < right;
        }
        if(binary.operator_.type == Token::Type::LESS_EQUALS) {
            return left <= right;
        }
        if(binary.operator_.type == Token::Type::GREATER) {
            return left > right;
        }
        if(binary.operator_.type == Token::Type::GREATER_EQUALS) {
            return left >= right;
        }
        throw std::runtime_error{"Should never reach this point."};
    }

    bool Interpreter::is_truthy(const Value& value) const {
        if(value.type == Value::Type::INTEGER) {
            return std::get<long long>(value.data) != 0;
        }
        if(value.type == Value::Type::FLOAT) {
            return std::get<long double>(value.data) != 0.0;
        }
        if(value.type == Value::Type::BOOL) {
            return std::get<bool>(value.data);
        }
        throw std::runtime_error{"Only numbers and booleans can be used as condition."};
    }

}

