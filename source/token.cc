#include "token.h"

namespace lynx {

    std::string source_location_from_token(const Token& token) {
        return token.filename + ":" + token.line + ":" + token.column;
    }

}

