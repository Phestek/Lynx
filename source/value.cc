#include "value.h"

namespace lynx {

    Value operator+(const Value& left, const Value& right) {
        if(left.type != right.type) {
            throw Incompatible_Value_Types{};
        }
        if(left.type == Value::Type::INTEGER) {
            return Value{left.type, std::get<long long>(left.data) + std::get<long long>(right.data)};
        }
        if(left.type == Value::Type::FLOAT) {
        }
    }
    
    Value operator-(const Value& left, const Value& right) {
        if(left.type != right.type) {
            throw Incompatible_Value_Types{};
        }
    }
    
    Value operator*(const Value& left, const Value& right) {
        if(left.type != right.type) {
            throw Incompatible_Value_Types{};
        }
    }
    
    Value operator/(const Value& left, const Value& right) {
        if(left.type != right.type) {
            throw Incompatible_Value_Types{};
        }
    }

}

