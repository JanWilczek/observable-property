#include <gtest/gtest.h>
#include <juce_events/juce_events.h>
#include <future>
#include <observable_property/juce/LiveObservableProperty.hpp>
#include <thread>

namespace ws::juce {
TEST(LiveObservableProperty, CallsObserversOnMessageThread) {
  LiveObservableProperty<int> property;

  std::optional<std::thread::id> callbackThreadId;
  const auto connection = property.observe([&](const auto& newValue) {
    // this is not a data race because callbackThreadId should be read
    // and written only on the main thread
    callbackThreadId = std::this_thread::get_id();
    ASSERT_EQ(1, newValue);
  });

  auto* const messageManager = ::juce::MessageManager::getInstance();
  messageManager->setCurrentThreadAsMessageThread();

  // change the property from a different thread
  auto future = std::async(std::launch::async, [&] { property.postValue(1); });

  // assert that the callback was called on the MessageManager (GUI) thread
  future.get();
  messageManager->runDispatchLoopUntil(100);
  ASSERT_EQ(std::this_thread::get_id(), callbackThreadId.value());
}

TEST(LiveObservableProperty, SetsValueOnCurrentThread) {
  LiveObservableProperty<int> property;

  std::optional<std::thread::id> callbackThreadId;
  const auto connection = property.observe([&](const auto& newValue) {
    // this is not a data race because callbackThreadId should be read
    // and written only on the main thread
    callbackThreadId = std::this_thread::get_id();
    ASSERT_EQ(1, newValue);
  });

  // change the property from a different thread
  property.setValue(1);

  // assert that the callback was called on the MessageManager (GUI) thread
  ASSERT_EQ(std::this_thread::get_id(), callbackThreadId.value());
}
}  // namespace ws::juce