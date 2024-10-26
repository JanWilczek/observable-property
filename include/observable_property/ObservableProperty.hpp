#pragma once
#include <observable_property/detail/boost/signals2.hpp>

namespace ws {
using ScopedConnection = boost::signals2::scoped_connection;

/**
 * @class ObservableProperty
 * @brief A value that can accept listeners to be called on value change.
 *
 * This class is NOT thread-safe.
 *
 * @tparam T type of the stored value
 *
 * @see MutableObservableProperty LiveObservableProperty
 */
template <typename T>
class ObservableProperty {
public:
  template <typename Signature>
  using signal = boost::signals2::signal<Signature>;
  using value_type = T;
  using SignalType = signal<void(const T&)>;

  [[nodiscard]] ScopedConnection observe(
      const typename SignalType::slot_type& onChangedCallback) {
    return onChanged_.connect(onChangedCallback);
  }

  [[nodiscard]] const T& value() const noexcept { return value_; }

protected:
  explicit ObservableProperty(const T& initialValue) : value_{initialValue} {}

  T value_;
  SignalType onChanged_;
};

/**
 * @class MutableObservableProperty
 * @brief Settable ObservableProperty.
 *
 * It should typically be an implementation detail; access to it should be
 * granted via a reference to the parent class.
 *
 * @tparam T
 *
 * @see ObservableProperty
 */
template <typename T>
class MutableObservableProperty : public ObservableProperty<T> {
public:
  explicit MutableObservableProperty(const T& initialValue = {})
      : ObservableProperty<T>(initialValue) {}

  /**
   * @brief Sets the value only if it's different from the current value.
   *
   * Calls the observers afterward.
   *
   * @param newValue
   */
  void setValue(const T& newValue) {
    if (newValue != this->value_) {
      setValueForced(newValue);
    }
  }

  /**
   * @brief Sets the value without checking if it's different from the one
   * already stored.
   *
   * Calls the observers afterward.
   *
   * Use this function if there's no suitable or cheap equality comparison
   * operator for T.
   *
   * @param newValue
   */
  void setValueForced(const T& newValue) {
    this->value_ = newValue;
    this->onChanged_(this->value_);
  }
};

/**
 * @brief A convenience macro to be put into the public section of a class
 * declaration. Requires the property to be initialized in the class
 * constructor.
 */
#define OBSERVABLE_PROPERTY_IMPL(propertyName, Type, ObservablePropertyType) \
public:                                                                      \
  [[nodiscard]] ws::ObservableProperty<Type>& propertyName() noexcept {    \
    return propertyName##_;                                                  \
  }                                                                          \
                                                                             \
  [[nodiscard]] const ws::ObservableProperty<Type>& propertyName()         \
      const noexcept {                                                       \
    return propertyName##_;                                                  \
  }                                                                          \
                                                                             \
private:                                                                     \
  ObservablePropertyType<Type> propertyName##_;                              \
                                                                             \
public:

#define OBSERVABLE_PROPERTY(propertyName, Type) \
  OBSERVABLE_PROPERTY_IMPL(propertyName, Type, ws::MutableObservableProperty)
}  // namespace ws
