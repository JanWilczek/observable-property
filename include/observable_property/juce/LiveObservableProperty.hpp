// Copyright (c) 2024
// Jan Wilczek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <observable_property/ObservableProperty.hpp>
#include <observable_property/juce/callOnMessageThreadIfNotNull.hpp>

namespace wolfsound::juce {
/**
 * @class LiveObservableProperty
 * @brief An ObservableProperty that is suitable for GUI interaction.
 *
 * It provides setValue() and setValueForced() equivalents (post*() functions)
 * that execute the value update and subsequent observer notification on the
 * main thread.
 *
 * Note that this class is still not thread safe, i.e., simultaneous reading and
 * writing from different threads constitutes a data race.
 * You should only call value(), setValue(), and setValueForced() from the main
 * thread. You can call post*() functions from any thread.
 */
template <typename T>
class LiveObservableProperty : public MutableObservableProperty<T> {
public:
  explicit LiveObservableProperty(const T& initialValue = {})
      : MutableObservableProperty<T>(initialValue) {}

  void postValue(const T& newValue) {
    callOnMessageThreadIfNotNull(
        [this, newValue] { this->setValue(newValue); });
  }

  void postValueForced(const T& newValue) {
    callOnMessageThreadIfNotNull(
        [this, newValue] { this->setValueForced(newValue); });
  }
};

#define LIVE_OBSERVABLE_PROPERTY(propertyName, Type) \
  OBSERVABLE_PROPERTY_IMPL(propertyName, Type,       \
                           wolfsound::juce::LiveObservableProperty)
}  // namespace wolfsound::juce
