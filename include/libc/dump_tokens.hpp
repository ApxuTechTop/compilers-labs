#pragma once

#include <iosfwd>

namespace ccompiler {
void dump_tokens(std::istream& in, std::ostream& out);
} // namespace ccompiler