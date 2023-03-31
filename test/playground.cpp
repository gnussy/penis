#include <gtest/gtest.h>
#include <stdio.h>

#include <penis/penis.hpp>

TEST(PLAYGROUND, PLAYGROUND) {
  penis::PromptBuilder{}
      .subscribe([](const std::string& command) { std::cout << command << std::endl; })
      ->run();
}
