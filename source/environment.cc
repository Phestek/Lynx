#include "environment.h"

#include <iomanip>
#include <stdexcept>

namespace lynx {

    void Environment::define(const std::string& name, const Value value) {
        for(const auto& symbol : _symbols) {
            if(symbol.name == name) {
                throw std::runtime_error{"Redefinition of '" + name + "'"};
            }
        }
        _symbols.emplace_back(Symbol{name, value});
    }

    void Environment::assign(const std::string& name, const Value value) {
        for(auto& symbol : _symbols) {
            if(symbol.name == name) {
                symbol.value = std::move(value);
                return;
            }
        }
        throw std::runtime_error{"Redefinition of '" + name + "'"};
    }

    Value Environment::get(const std::string& name) {
        for(const auto& symbol : _symbols) {
            if(symbol.name == name) {
                return symbol.value;
            }
        }
        throw std::runtime_error{"'" + name + "' is undefined"};
    }

}

