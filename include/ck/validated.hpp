#pragma once

#include <type_traits>

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

    template<typename V>
    class validated_base
    {
    public:
        template<typename U> requires std::is_convertible_v<U, V>
        constexpr explicit validated_base(U&& validator)
            : validator_(std::forward<U>(validator)) {}

        constexpr V& validator() {
            return validator_;
        }

    private:
        V validator_{};
    };

    template<typename V> requires detail::is_stateless_v<V>
    class validated_base<V> {
    public:
        constexpr V validator() {
            return V{};
        }
    };

    template<typename T, typename V>
    class validated : validated_base<V> {
    public:
        constexpr validated() = default;

        template<typename U>
        requires std::is_convertible_v<U, T>
        constexpr validated(U&& value)
            : validated_base<V>{}
            , value_{validate(std::forward<U>(value))} {}

        template<typename U, typename W>
        requires std::is_convertible_v<U, T> && std::is_convertible_v<W, V>
        constexpr validated(U&& value, W&& validator)
            : validated_base<V>{std::forward<W>(validator)}
            , value_{validate(std::forward<U>(value))} {}

        template<typename W>
        requires std::is_convertible_v<W, V>
        constexpr validated(W&& validator)
            : validated_base<V>{std::forward<W>(validator)}
            , value_{} {}

        template<typename U>
        constexpr validated& operator=(U&& value) {
            value_ = validate(std::forward<U>(value));
            return *this;
        }

    private:
        template<typename U>
        constexpr U&& validate(U&& value) {
            if (validated_base<V>::validator()(value)) {
                return std::forward<U>(value);
            }
            throw std::invalid_argument("Validation failed");
        }

        T value_{};
    };

}