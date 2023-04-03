<h1 align="center"> PENIS 🍆 </h1>

<p align="center">
  <img src="https://raw.githubusercontent.com/gnussy/penis/main/assets/logo.png" width=200 />
</p>
<p align="center">
  <i>Portable Efficient Navigation Interface System</i>
</p>

---

# Introduction

PENIS is a C++ library that provides a subscription-based API to allow clients to access commands from a prompt. The library is a wrapper around the <termios.h> library, which controls keys, maintains a history, and provides auto-complete functionality to the user.

# Features

- [X] Subscription-based API to access commands from a prompt
- [ ] Auto-complete functionality for faster input
- [X] Command history tracking
- [ ] Navigation features for ease of use
- [ ] Portable and easy-to-use

# Requirements
- A C++ compiler that supports C++11 or later
- The <termios.h> library

# Installation
- [ ] Describe installation

# Usage
- [ ] Describe usage

# Examples
Here's an example of how to use the PENIS library to create a simple calculator:

```c++
#include <penis/penis.hpp>
#include <iostream>

void my_callback(const std::string& command) {
  std::cout << "Received command: " << command << std::endl;
}

int main() {
  penis::PromptBuilder repl;
  repl.subscribe_command(my_callback).run();
  return 0;
}
```

This example creates a subscription to the PENIS prompt and waits for the user to input a command. If the command is "exit", the program will exit. Otherwise, it tries to convert the command string to an integer and prints the result to the console. If the conversion fails, it prints an error message.

# License
PENIS is licensed under the GPL-3 License. See the LICENSE file for more information.

Contact
- [ ] Create a gnussy e-mail
