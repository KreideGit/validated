#pragma once

#include <type_traits>
#include <stdexcept>

namespace ck {

    namespace detail {

        template<typename T, typename = void>
        struct is_stateless : std::false_type {};

        template<typename T>
        struct is_stateless<T, std::void_t<decltype(&T::operator())>> {
            static constexpr bool value = std::is_empty_v<T> && std::is_trivial_v<T>;
        };

        template<typename T>
        constexpr bool is_stateless_v = is_stateless<T>::value;

    }

    struct validation_error : std::exception {};

    template<typename V>
    class validated_base
    {
    public:
        constexpr validated_base() = default;

        template<typename U> requires std::is_convertible_v<U, V>
        constexpr explicit validated_base(U&& validator)
            : validator_(std::forward<U>(validator)) {}

        constexpr V& validator() {
            return validator_;
        }

        constexpr const V& validator() const {
            return validator_;
        }

    private:
        V validator_{};
    };

    template<typename V> requires detail::is_stateless_v<V>
    class validated_base<V> {
    public:
        constexpr V validator() const {
            return V{};
        }
    };

    template<typename T, typename V>
    class basic_validated : public validated_base<V> {
    public:
        constexpr basic_validated() = default;

        template<typename U>
        requires std::is_convertible_v<U, T>
        constexpr basic_validated(U&& value)
            : validated_base<V>{}
            , value_{validate(std::forward<U>(value))} {}

        template<typename U, typename W>
        requires std::is_convertible_v<U, T> && std::is_convertible_v<W, V>
        constexpr basic_validated(U&& value, W&& validator)
            : validated_base<V>{std::forward<W>(validator)}
            , value_{validate(std::forward<U>(value))} {}

        template<typename W>
        requires std::is_convertible_v<W, V>
        constexpr explicit basic_validated(W&& validator)
            : validated_base<V>{std::forward<W>(validator)}
            , value_{} {}

        template<typename U>
        constexpr basic_validated& operator=(U&& value) {
            value_ = validate(std::forward<U>(value));
            return *this;
        }

        constexpr const T& operator*() const {
            return value_;
        }

        constexpr const T& operator->() const {
            return value_;
        }

        template<typename U>
        constexpr bool try_assign(U&& value) {
            if (!validated_base<V>::validator()(value)) {
                return false;
            }

            value_ = std::forward<U>(value);
            return true;
        }

        template<typename U>
        constexpr void assign_unsafe(U&& value) {
            value_ = std::forward<U>(value);
        }

        constexpr const T& value() const {
            return value_;
        }

        constexpr void revalidate() {
            if (!validated_base<V>::validator()(value_)) {
                throw validation_error{};
            }
        }

    private:
        template<typename U>
        constexpr U&& validate(U&& value) {
            if (validated_base<V>::validator()(value)) {
                return std::forward<U>(value);
            }
            throw validation_error{};
        }

        T value_{};
    };

    template<typename T, typename V>
    using validated = basic_validated<T, V>;

}