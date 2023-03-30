#include <fmt/core.h>

#include <iostream>
#include <penis/penis.hpp>
#include <string>

int main() {
  int counter_a = 0;
  int counter_b = 0;

  penis::PromptBuilder{}
      .subscribe([&counter_a, &counter_b](const std::string& command) {
        if (command == "increase_a") {
          fmt::print("Counter a: {}\n", ++counter_a);
        } else if (command == "increase_b") {
          fmt::print("Counter b: {}\n", ++counter_b);
        }
      })
      ->run();
  return 0;
}
