#ifndef TUNISBITMASK_H
#define TUNISBITMASK_H

#include <type_traits>

/*!
 * Helpers for using bitmasks with enum classes
 *
 * Inspried by this blog:
 * http://blog.bitwigglers.org/using-enum-classes-as-type-safe-bitmasks/
 * Special thanks to Andre Haupt.
 */

namespace tunis
{
    namespace detail
    {
        template<typename E>
        struct EnableBitMaskOperators {};

#define TUNIS_ENABLE_BITMASK_OPERATORS(E)\
        template<> struct EnableBitMaskOperators<E>\
        {\
            static const bool enable = true;\
        };

        template<typename E>
        typename std::enable_if<EnableBitMaskOperators<E>::enable, bool>::type
        operator!(E rhs)
        {
            return rhs == static_cast<E>(0);
        }

        template<typename E>
        typename std::enable_if<EnableBitMaskOperators<E>::enable, E>::type
        operator |(E lhs, E rhs)
        {
            using underlying = typename std::underlying_type<E>::type;
            return static_cast<E> (
                static_cast<underlying>(lhs) |
                static_cast<underlying>(rhs)
            );
        }

        template<typename E>
        typename std::enable_if<EnableBitMaskOperators<E>::enable, E>::type
        operator &(E lhs, E rhs)
        {
            using underlying = typename std::underlying_type<E>::type;
            return static_cast<E> (
                static_cast<underlying>(lhs) &
                static_cast<underlying>(rhs)
            );
        }

        template<typename E>
        typename std::enable_if<EnableBitMaskOperators<E>::enable, E>::type
        operator ^(E lhs, E rhs)
        {
            using underlying = typename std::underlying_type<E>::type;
            return static_cast<E> (
                static_cast<underlying>(lhs) ^
                static_cast<underlying>(rhs)
            );
        }

        template<typename E>
        typename std::enable_if<EnableBitMaskOperators<E>::enable, E>::type
        operator ~(E rhs)
        {
            return static_cast<E> (
                ~static_cast<typename std::underlying_type<E>::type>(rhs)
            );
        }
        template<typename E>
        typename std::enable_if<EnableBitMaskOperators<E>::enable, E>::type&
        operator |=(E &lhs, E rhs)
        {
            lhs = static_cast<E> (
                static_cast<typename std::underlying_type<E>::type>(lhs) |
                static_cast<typename std::underlying_type<E>::type>(rhs)
            );
            return lhs;
        }

        template<typename E>
        typename std::enable_if<EnableBitMaskOperators<E>::enable, E>::type&
        operator &=(E &lhs, E rhs)
        {
            lhs = static_cast<E> (
                static_cast<typename std::underlying_type<E>::type>(lhs) &
                static_cast<typename std::underlying_type<E>::type>(rhs)
            );
            return lhs;
        }

        template<typename E>
        typename std::enable_if<EnableBitMaskOperators<E>::enable, E>::type&
        operator ^=(E &lhs, E rhs)
        {
            lhs = static_cast<E> (
                static_cast<typename std::underlying_type<E>::type>(lhs) ^
                static_cast<typename std::underlying_type<E>::type>(rhs)
            );
            return lhs;
        }

    }

}


#endif // TUNISBITMASK_H
