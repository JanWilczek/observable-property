#pragma once
#include <juce_events/juce_events.h>
#include <functional>

namespace ws::juce {
/**
 * @brief Calls callback \p f on the message (main, GUI) thread if such is
 * available.
 *
 * Otherwise, executes the call inline.
 *
 * @param f function to call
 */
inline void callOnMessageThreadIfNotNull(std::function<void()> f) {
  if (const auto* messageManager =
          ::juce::MessageManager::getInstanceWithoutCreating();
      messageManager != nullptr) {
    // Execute on the GUI thread
    messageManager->callAsync(std::move(f));
  } else {
    // Execute inline
    f();
  }
}
}  // namespace ws::juce
