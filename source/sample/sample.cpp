#include <fmt/core.h>

#include <sample/sample.hpp>
#include <stdexcept>

namespace sample {
  void sample_function() { fmt::print("Hello, {}!\n", "world"); }

  void unimplemented_logic() { throw std::runtime_error("Unimplemented logic"); }
}  // namespace sample
