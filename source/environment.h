#ifndef LYNX_ENVIRONMENT_H
#define LYNX_ENVIRONMENT_H

#include <string>
#include <vector>

#include "value.h"

namespace lynx {

    // TODO: Store data about variable's immutability.
    class Environment {
    public:
        void define(const std::string& name, const Value value);
        void assign(const std::string& name, const Value value);
        Value get(const std::string& name);

    private:
        // Dirty workaround for std::(unordered_)map<std::string, std::variant.
        struct Symbol {
            std::string name;
            Value       value;
        };

        std::vector<Symbol> _symbols;
    };

}

#endif //LYNX_ENVIRONMENT_H

