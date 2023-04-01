#pragma once

#include <termios.h>
#include <unistd.h>

#include <functional>
#include <string>
#include <string_view>

namespace penis {
  class PromptBuilder {
  public:
    using CommandCallback = std::function<void(const std::string&)>;
    using Self = PromptBuilder*;

  private:
    termios termios_settings_;
    std::vector<CommandCallback> command_callbacks_;
    std::vector<std::string> history_;
    int history_pos_;
    std::string_view prompt_;

  public:
    PromptBuilder();

    Self subscribe(CommandCallback callback);
    Self prompt(const std::string_view& prompt);
    Self run();

    ~PromptBuilder();

  private:
    void set_terminal_mode(bool on);
    std::string read_line();
  };
}  // namespace penis
