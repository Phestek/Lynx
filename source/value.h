#ifndef LYNX_VALUE_H
#define LYNX_VALUE_H

#include <exception>
#include <string>
#include <variant>

namespace lynx {

    // TODO: Add strings and objects (if I decide to add operators overloading).

    struct Value {
        enum class Type {
            INTEGER, FLOAT, BOOL, STRING
        };
        using Data = std::variant<long long, long double, bool, std::string>;

        Value(const Type type, const Data data)
                : type{type}, data{data} {
        }
        
        Type type;
        Data data;
    };

    static_assert(std::is_copy_constructible<Value>());

    class Incompatible_Value_Types : public std::exception {
    public:
        Incompatible_Value_Types() noexcept = default;
    };

    Value operator+(const Value& left, const Value& right);
    Value operator-(const Value& left, const Value& right);
    Value operator*(const Value& left, const Value& right);
    Value operator/(const Value& left, const Value& right);

}

#endif //LYNX_VALUE_H

