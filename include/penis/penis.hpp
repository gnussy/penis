#pragma once

#include <termios.h>
#include <unistd.h>

#include <functional>
#include <string>

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

  public:
    PromptBuilder();

    Self run();
    Self subscribe(CommandCallback callback);

    ~PromptBuilder();

  private:
    void set_terminal_mode(bool on);
    std::string read_line();
  };
}  // namespace penis
