#pragma once

#include <termios.h>
#include <unistd.h>

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <string_view>
#include <thread>
#include <atomic>

namespace penis {
  class PromptBuilder {
  public:
    using CommandCallback = std::function<void(const std::string&)>;
    using Self = PromptBuilder*;

  private:
    termios termios_settings;
    std::vector<CommandCallback> command_callbacks;
    std::vector<CommandCallback> event_callbacks;
    std::vector<std::string> history;
    int history_pos;
    std::string prompt_;

    std::mutex event_mutex;
    std::condition_variable event_cond;
    std::queue<std::pair<std::string, std::string>> event_queue;
    std::atomic<bool> stop_event_listener = false;

  public:
    PromptBuilder();

    Self subscribe_command(CommandCallback callback);
    Self subscribe_event(CommandCallback callback);
    Self emit_event(const std::string& event_type, const std::string& event_data);
    Self prompt(const std::string& prompt);
    Self run();

    ~PromptBuilder();

  private:
    void set_terminal_mode(bool on);
    std::string read_line();
  };
}  // namespace penis
