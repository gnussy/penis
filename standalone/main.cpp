#include <termios.h>
#include <unistd.h>

#include <functional>
#include <iostream>
#include <string>
#include <vector>

class PromptBuilder {
public:
  using CommandCallback = std::function<void(const std::string&)>;

  PromptBuilder() {
    // Save the current terminal settings
    tcgetattr(STDIN_FILENO, &termios_settings_);

    // Set the terminal to non-canonical mode without echoing
    set_terminal_mode(false);
  }

  ~PromptBuilder() {
    // Restore the terminal settings
    set_terminal_mode(true);
  }

  void run() {
    while (true) {
      std::cout << "> ";
      std::string input = read_line();
      if (input.empty()) {
        continue;
      }
      if (input == "quit" || input == "exit") {
        break;
      }
      std::cout << "You entered: " << input << std::endl;
      history_.push_back(input);
      history_pos_ = history_.size();
      for (const auto& callback : command_callbacks_) {
        callback(input);
      }
    }
  }

  void subscribe(CommandCallback callback) { command_callbacks_.push_back(callback); }

private:
  termios termios_settings_;
  std::vector<CommandCallback> command_callbacks_;
  std::vector<std::string> history_;
  int history_pos_;

  void set_terminal_mode(bool on) {
    if (on) {
      tcsetattr(STDIN_FILENO, TCSANOW, &termios_settings_);
      return;
    }

    termios new_settings = termios_settings_;
    new_settings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
  }

  std::string read_line() {
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
              std::cout << line;
            }
            break;
          case 'B':                   // Down arrow
            std::cout << "\r\x1B[K";  // clear line
            if (history_pos_ < history_.size() - 1) {
              history_pos_++;
              line = history_[history_pos_];
              pos = line.size();
              std::cout << line;
            } else {
              history_pos_ = history_.size();
              line = "";
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
};

void my_callback(const std::string& command) {
  std::cout << "Received command: " << command << std::endl;
}

int main() {
  PromptBuilder repl;
  repl.subscribe(my_callback);
  repl.run();
  return 0;
}
