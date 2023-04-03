#include <iostream>
#include <penis/penis.hpp>

namespace penis {

  PromptBuilder::PromptBuilder() {
    // Save the current terminal settings
    tcgetattr(STDIN_FILENO, &termios_settings);

    // Set the terminal to non-canonical mode without echoing
    set_terminal_mode(false);
  }

  PromptBuilder::~PromptBuilder() {
    // Restore the terminal settings
    set_terminal_mode(true);
  }

  PromptBuilder::Self PromptBuilder::run() {
    std::thread event_thread([this] {
      while (!stop_event_listener) {
        std::unique_lock<std::mutex> lock(event_mutex);
        event_cond.wait(lock, [this] { return stop_event_listener || !event_queue.empty(); });
        if (stop_event_listener) break;
        const auto& event = event_queue.front();
        for (const auto& callback : event_callbacks) {
          callback(event.first + " " + event.second);
        }
        event_queue.pop();
      }
    });

    while (true) {
      std::cout << prompt_;
      std::string input = read_line();
      if (input.empty()) {
        continue;
      }
      if (input == "quit" || input == "exit") {
        break;
      }
      history.push_back(input);
      history_pos = history.size();
      for (const auto& callback : command_callbacks) {
        callback(input);
      }
    }

    stop_event_listener = true;
    event_cond.notify_one();
    event_thread.join();
    return this;
  }

  PromptBuilder::Self PromptBuilder::subscribe_command(PromptBuilder::CommandCallback callback) {
    command_callbacks.push_back(callback);
    return this;
  }

  PromptBuilder::Self PromptBuilder::subscribe_event(PromptBuilder::CommandCallback callback) {
    event_callbacks.push_back(callback);
    return this;
  }

  PromptBuilder::Self PromptBuilder::emit_event(const std::string& event_type,
                                                const std::string& event_data) {
    std::lock_guard<std::mutex> lock(event_mutex);
    event_queue.push({event_type, event_data});
    event_cond.notify_one();
    return this;
  }

  PromptBuilder::Self PromptBuilder::prompt(const std::string_view& prompt) {
    this->prompt_ = prompt;
    return this;
  }

  void PromptBuilder::set_terminal_mode(bool on) {
    if (on) {
      tcsetattr(STDIN_FILENO, TCSANOW, &termios_settings);
      return;
    }

    termios new_settings = termios_settings;
    new_settings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
  }

  std::string PromptBuilder::read_line() {
    std::string line;
    int c;
    int pos = 0;  // current position of the cursor
    while ((c = getchar()) != EOF && c != '\n') {
      if (c == 127 || c == '\b') {  // Handle backspace/delete
        if (pos > 0) {
          std::cout << "\b \b";
          line.erase(--pos, 1);
        }
      } else if (c == '\033') {  // Handle escape codes (arrow keys)
        getchar();               // skip '['
        switch (getchar()) {
          case 'A':                   // Up arrow
            std::cout << "\r\x1B[K";  // clear line
            if (!history.empty() && history_pos > 0) {
              history_pos--;
              line = history[history_pos];
              pos = line.size();
              std::cout << prompt_;
              std::cout << line;
            } else {
              std::cout << prompt_;
            }
            break;
          case 'B':                   // Down arrow
            std::cout << "\r\x1B[K";  // clear line
            if (history_pos < history.size() - 1) {
              history_pos++;
              line = history[history_pos];
              pos = line.size();
              std::cout << prompt_;
              std::cout << line;
            } else {
              std::cout << prompt_;
            }
            break;
          case 'C':  // Right arrow
            if (pos < line.size()) {
              std::cout << line[pos];
              pos++;
            }
            break;
          case 'D':  // Left arrow
            if (pos > 0) {
              pos--;
              std::cout << "\b";
            }
            break;
          default:
            break;
        }
      } else {
        line.insert(pos, 1, static_cast<char>(c));
        std::cout << static_cast<char>(c);
        pos++;
      }
    }
    std::cout << std::endl;
    return line;
  }
}  // namespace penis
