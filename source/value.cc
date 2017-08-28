#include "value.h"

#include <stdexcept>

namespace lynx {

    Value operator+(const Value& left, const Value& right) {
        if(left.type != right.type) {
            throw Incompatible_Value_Types{};
        }
        if(left.type == Value::Type::INTEGER) {
            return Value{left.type, std::get<long long>(left.data) + std::get<long long>(right.data)};
        }
        if(left.type == Value::Type::FLOAT) {
            return Value{left.type, std::get<long double>(left.data) + std::get<long double>(right.data)};
        }
        throw std::runtime_error{"Should never reach this point."};
    }
    
    Value operator-(const Value& left, const Value& right) {
        if(left.type != right.type) {
            throw Incompatible_Value_Types{};
        }
        if(left.type == Value::Type::INTEGER) {
            return Value{left.type, std::get<long long>(left.data) - std::get<long long>(right.data)};
        }
        if(left.type == Value::Type::FLOAT) {
            return Value{left.type, std::get<long double>(left.data) - std::get<long double>(right.data)};
        }
        throw std::runtime_error{"Should never reach this point."};
    }
    
    Value operator*(const Value& left, const Value& right) {
        if(left.type != right.type) {
            throw Incompatible_Value_Types{};
        }
        if(left.type == Value::Type::INTEGER) {
            return Value{left.type, std::get<long long>(left.data) * std::get<long long>(right.data)};
        }
        if(left.type == Value::Type::FLOAT) {
            return Value{left.type, std::get<long double>(left.data) * std::get<long double>(right.data)};
        }
        throw std::runtime_error{"Should never reach this point."};
    }
    
    Value operator/(const Value& left, const Value& right) {
        if(left.type != right.type) {
            throw Incompatible_Value_Types{};
        }
        if(left.type == Value::Type::INTEGER) {
            return Value{left.type, std::get<long long>(left.data) / std::get<long long>(right.data)};
        }
        if(left.type == Value::Type::FLOAT) {
            return Value{left.type, std::get<long double>(left.data) / std::get<long double>(right.data)};
        }
        throw std::runtime_error{"Should never reach this point."};
    }

}

