#include "value.h"

#include <stdexcept>

namespace lynx {

    Value operator==(const Value& left, const Value& right) {
        if(left.type != right.type) {
            throw Incompatible_Value_Types{};
        }
        if(left.type == Value::Type::INTEGER) {
            return Value{left.type, std::get<long long>(left.data) == std::get<long long>(right.data)};
        }
        if(left.type == Value::Type::FLOAT) {
            return Value{left.type, std::get<long double>(left.data) == std::get<long double>(right.data)};
        }
        if(left.type == Value::Type::STRING) {
            return Value{left.type, std::get<std::string>(left.data) == std::get<std::string>(right.data)};
        }
        if(left.type == Value::Type::BOOL) {
            return Value{left.type, std::get<bool>(left.data) == std::get<bool>(right.data)};
        }
        throw std::runtime_error{"Should never reach this point."};
    }

    Value operator!=(const Value& left, const Value& right) {
        if(left.type != right.type) {
            throw Incompatible_Value_Types{};
        }
        if(left.type == Value::Type::INTEGER) {
            return Value{left.type, std::get<long long>(left.data) != std::get<long long>(right.data)};
        }
        if(left.type != Value::Type::FLOAT) {
            return Value{left.type, std::get<long double>(left.data) != std::get<long double>(right.data)};
        }
        if(left.type != Value::Type::STRING) {
            return Value{left.type, std::get<std::string>(left.data) != std::get<std::string>(right.data)};
        }
        if(left.type != Value::Type::BOOL) {
            return Value{left.type, std::get<bool>(left.data) != std::get<bool>(right.data)};
        }
        throw std::runtime_error{"Should never reach this point."};
    }

    Value operator<(const Value& left, const Value& right) {
        if(left.type != right.type) {
            throw Incompatible_Value_Types{};
        }
        if(left.type == Value::Type::INTEGER) {
            return Value{left.type, std::get<long long>(left.data) < std::get<long long>(right.data)};
        }
        if(left.type == Value::Type::FLOAT) {
            return Value{left.type, std::get<long double>(left.data) < std::get<long double>(right.data)};
        }
        throw std::runtime_error{"Should never reach this point."};
    }

    Value operator>(const Value& left, const Value& right) {
        if(left.type != right.type) {
            throw Incompatible_Value_Types{};
        }
        if(left.type > Value::Type::INTEGER) {
            return Value{left.type, std::get<long long>(left.data) > std::get<long long>(right.data)};
        }
        if(left.type > Value::Type::FLOAT) {
            return Value{left.type, std::get<long double>(left.data) > std::get<long double>(right.data)};
        }
        throw std::runtime_error{"Should never reach this point."};
    }

    Value operator<=(const Value& left, const Value& right) {
        if(left.type != right.type) {
            throw Incompatible_Value_Types{};
        }
        if(left.type <= Value::Type::INTEGER) {
            return Value{left.type, std::get<long long>(left.data) <= std::get<long long>(right.data)};
        }
        if(left.type <= Value::Type::FLOAT) {
            return Value{left.type, std::get<long double>(left.data) <= std::get<long double>(right.data)};
        }
        throw std::runtime_error{"Should never reach this point."};
    }

    Value operator>=(const Value& left, const Value& right) {
        if(left.type != right.type) {
            throw Incompatible_Value_Types{};
        }
        if(left.type >= Value::Type::INTEGER) {
            return Value{left.type, std::get<long long>(left.data) >= std::get<long long>(right.data)};
        }
        if(left.type >= Value::Type::FLOAT) {
            return Value{left.type, std::get<long double>(left.data) >= std::get<long double>(right.data)};
        }
        throw std::runtime_error{"Should never reach this point."};
    }

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
        if(left.type == Value::Type::STRING) {
            return Value{left.type, std::get<std::string>(left.data) + std::get<std::string>(right.data)};
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
        if(left.type == Value::Type::STRING) {
            throw std::runtime_error{"Can't substract two strings."};
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
        if(left.type == Value::Type::STRING) {
            throw std::runtime_error{"Can't multiply two strings."};
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
        if(left.type == Value::Type::STRING) {
            throw std::runtime_error{"Can't divide two strings."};
        }
        throw std::runtime_error{"Should never reach this point."};
    }

}

