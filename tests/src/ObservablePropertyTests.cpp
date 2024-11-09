// Copyright (c) 2024
// Jan Wilczek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <observable_property/ObservableProperty.hpp>

namespace wolfsound {
TEST(ObservableProperty, CanBePublicMember) {
  class Holder {
  public:
    [[nodiscard]] ObservableProperty<int>& property() noexcept {
      return property_;
    }

    void incrementValue() { property_.setValue(property_.value() + 1); }

  private:
    MutableObservableProperty<int> property_{0};
  } holder;

  bool called = false;
  const auto connection = holder.property().observe([&](const int& value) {
    called = true;
    ASSERT_EQ(1, value);
  });

  // Doesn't work although property() is public.
  // holder.property().setValue(1);
  ASSERT_FALSE(called);

  // Tightly restricted API
  holder.incrementValue();

  ASSERT_TRUE(called);
}

TEST(ObservableProperty, PublicMemberMacro) {
  class Holder {
  public:
    Holder() : property_{0} {}

    OBSERVABLE_PROPERTY(property, int)

    void incrementValue() { property_.setValue(property_.value() + 1); }
  } holder;

  bool called = false;
  const auto connection = holder.property().observe([&](const int& value) {
    called = true;
    ASSERT_EQ(1, value);
  });

  // Doesn't work although property() is public.
  // holder.property().setValue(1);
  ASSERT_FALSE(called);

  // Tightly restricted API
  holder.incrementValue();

  ASSERT_TRUE(called);
}
}  // namespace wolfsound
