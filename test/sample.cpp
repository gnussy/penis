#include <gtest/gtest.h>

#include <sample/sample.hpp>

TEST(SampleTest, SampleFunction) { EXPECT_EQ(1, 1); }

TEST(SampleTest, ShouldThrow) { EXPECT_THROW(sample::unimplemented_logic(), std::runtime_error); }
