#include <gtest/gtest.h>
#include "button.h"

TEST(Button, CreateActivate) {
    Button b("let down", RED, [](){throw std::runtime_error("bye");});
    EXPECT_EQ("let down", b.get_name());
    EXPECT_THROW(b(), std::runtime_error);
}