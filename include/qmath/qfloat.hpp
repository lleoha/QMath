#ifndef __QMATH_HPP_INCLUDED__
#define __QMATH_HPP_INCLUDED__

#include <concepts>
#include <limits>
#include <cmath>

template<std::signed_integral _Type, std::signed_integral _OverflowType, int _q, bool round = true, bool saturate = false>
requires (sizeof(_Type) <= sizeof(_OverflowType))
class QFloat {
public:
    using Type = _Type;
    using OverflowType = _OverflowType;
    static constexpr auto q = _q;

    static constexpr QFloat pi() {
        return QFloat(std::cos(-1.0L));
    }

    explicit constexpr QFloat(Type _value, std::nullptr_t): value(_value) {
    }

    constexpr QFloat(auto _value) requires std::integral<decltype(_value)> {
        value = static_cast<Type>(_value) << q;
    }

    constexpr QFloat(auto _value) requires std::floating_point<decltype(_value)> {
        value = static_cast<Type>(_value) * (1 << q);
    }

    constexpr QFloat(const QFloat&) = default;

    constexpr QFloat& operator=(const QFloat&) = default;

    template<std::integral T>
    constexpr operator T() const {
        return static_cast<T>(value >> q);
    }

    template<std::floating_point T>
    constexpr operator T() const {
        return static_cast<T>(value) / (1 << q);
    }

    constexpr QFloat operator*(QFloat that) const {
        auto rawValue = static_cast<OverflowType>(value) * that.value;

        if constexpr (round) {
            rawValue += (1 << (q - 1));
        }
        
        if constexpr (saturate) {
            constexpr auto min = std::numeric_limits<Type>::min();
            constexpr auto max = std::numeric_limits<Type>::max();
            if ((rawValue >> q) < min) {
                return QFloat(min, nullptr);
            } else if ((rawValue >> q) > max) {
                return QFloat(max, nullptr);
            }
        }
        
        return QFloat(rawValue >> q, nullptr);
    }

    constexpr QFloat operator/(QFloat that) const {
        auto lhs = (static_cast<OverflowType>(value) << q);
        auto rhs = that.value;

        auto rawValue = lhs / rhs;
        
        if constexpr (saturate) {
            constexpr auto min = std::numeric_limits<Type>::min();
            constexpr auto max = std::numeric_limits<Type>::max();
            if (rawValue < min) {
                return QFloat(min, nullptr);
            } else if (rawValue > max) {
                return QFloat(max, nullptr);
            }
        }

        return QFloat(rawValue, nullptr);
    }

    constexpr QFloat operator*(auto that) const {
        return this->operator*(static_cast<QFloat>(that));
    }

    constexpr QFloat operator/(auto that) const {
        return this->operator/(static_cast<QFloat>(that));
    }

    constexpr auto getRaw() const {
        return value;
    }

private:
    Type value;
};

template<std::signed_integral _Type, std::signed_integral _OverflowType, int _q, bool _round, bool _saturate>
constexpr auto operator*(auto lhs, QFloat<_Type, _OverflowType, _q, _round, _saturate> rhs) {
    return QFloat<_Type, _OverflowType, _q, _round, _saturate>(lhs) * rhs;
}

template<std::signed_integral _Type, std::signed_integral _OverflowType, int _q, bool _round, bool _saturate>
constexpr auto operator/(auto lhs, QFloat<_Type, _OverflowType, _q, _round, _saturate> rhs) {
    return QFloat<_Type, _OverflowType, _q, _round, _saturate>(lhs) / rhs;
}

#endif /* __QMATH_HPP_INCLUDED__ */