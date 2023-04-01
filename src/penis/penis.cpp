#include <iostream>
#include <penis/penis.hpp>

namespace penis {

  PromptBuilder::PromptBuilder() {
    // Save the current terminal settings
    tcgetattr(STDIN_FILENO, &termios_settings_);

    // Set the terminal to non-canonical mode without echoing
    set_terminal_mode(false);
  }

  PromptBuilder::~PromptBuilder() {
    // Restore the terminal settings
    set_terminal_mode(true);
  }

  PromptBuilder::Self PromptBuilder::run() {
    while (true) {
      std::cout << prompt_;
      std::string input = read_line();
      if (input.empty()) {
        continue;
      }
      if (input == "quit" || input == "exit") {
        break;
      }
      history_.push_back(input);
      history_pos_ = history_.size();
      for (const auto& callback : command_callbacks_) {
        callback(input);
      }
    }

    return this;
  }

  PromptBuilder::Self PromptBuilder::subscribe(PromptBuilder::CommandCallback callback) {
    command_callbacks_.push_back(callback);
    return this;
  }

  PromptBuilder::Self PromptBuilder::prompt(const std::string_view& prompt) {
    this->prompt_ = prompt;
    return this;
  }

  void PromptBuilder::set_terminal_mode(bool on) {
    if (on) {
      tcsetattr(STDIN_FILENO, TCSANOW, &termios_settings_);
      return;
    }

    termios new_settings = termios_settings_;
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
            if (!history_.empty() && history_pos_ > 0) {
              history_pos_--;
              line = history_[history_pos_];
              pos = line.size();
              std::cout << prompt_;
              std::cout << line;
            } else {
              std::cout << prompt_;
            }
            break;
          case 'B':                   // Down arrow
            std::cout << "\r\x1B[K";  // clear line
            if (history_pos_ < history_.size() - 1) {
              history_pos_++;
              line = history_[history_pos_];
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
