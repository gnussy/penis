#include <iostream>
#include <penis/penis.hpp>

int main(void) {
  penis::PromptBuilder builder;

  builder.prompt("$> ")
      ->subscribe([&builder](const std::string &command) {
        if (command == "prompt") {
          builder.prompt("$/sub> ");
        }
      })
      ->run();
  return 0;
}
