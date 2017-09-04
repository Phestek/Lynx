#ifndef LYNX_UTILS_IS_DOWNCASTABLE_H
#define LYNX_UTILS_IS_DOWNCASTABLE_H

#include <type_traits>

namespace lynx {

    template <typename To, typename From>
    bool is_downcastable(const From& from) {
        return std::is_base_of<From, To>();
    }

}

#endif //LYNX_UTILS_IS_DOWNCASTABLE_H

