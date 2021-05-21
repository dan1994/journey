#pragma once

namespace std {

template <typename Enum>
using underlying_type_t = __underlying_type(Enum);

}
