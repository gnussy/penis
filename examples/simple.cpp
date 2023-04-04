#include <iostream>
#include <penis/penis.hpp>

int main(void) {
  penis::PromptBuilder builder;

  builder.prompt("$> ")
      ->subscribe_command([&builder](const std::string &command) {
        if (command == "prompt") builder.prompt("$/sub> ");
        if (command == "event") builder.emit_event("change_prompt", "$/event> ");
      })
      ->subscribe_event([](const std::string &command) {})
      ->run();
  return 0;
}
