/*  This file is part of the Vc library. {{{

    Copyright (C) 2012-2013 Matthias Kretz <kretz@kde.org>

    Vc is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    Vc is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with Vc.  If not, see <http://www.gnu.org/licenses/>.

}}}*/

#ifndef VC_COMMON_TYPES_H
#define VC_COMMON_TYPES_H

#ifdef VC_CHECK_ALIGNMENT
#include <cstdlib>
#include <cstdio>
#endif

#include "../traits/type_traits.h"
#include "macros.h"

namespace Vc_VERSIONED_NAMESPACE
{
namespace Vc_IMPL_NAMESPACE
{
    template<typename T> class Vector;
    template<typename T> class Mask;
}

template <typename T, std::size_t N> struct array;

template <typename T, std::size_t N, typename VectorType, std::size_t> class simdarray;
template <typename T, std::size_t N, typename VectorType, std::size_t> class simd_mask_array;

namespace Traits
{
template <typename T, std::size_t N, typename V> struct is_atomic_simdarray_internal<simdarray<T, N, V, N>> : public std::true_type {};
template <typename T, std::size_t N, typename V> struct is_atomic_simd_mask_array_internal<simd_mask_array<T, N, V, N>> : public std::true_type {};

template <typename T, std::size_t N, typename VectorType, std::size_t M> struct is_simdarray_internal<simdarray<T, N, VectorType, M>> : public std::true_type {};
template <typename T, std::size_t N, typename VectorType, std::size_t M> struct is_simd_mask_array_internal<simd_mask_array<T, N, VectorType, M>> : public std::true_type {};
template <typename T, std::size_t N, typename V, std::size_t M> struct is_integral_internal      <simdarray<T, N, V, M>, false> : public std::is_integral<T> {};
template <typename T, std::size_t N, typename V, std::size_t M> struct is_floating_point_internal<simdarray<T, N, V, M>, false> : public std::is_floating_point<T> {};
template <typename T, std::size_t N, typename V, std::size_t M> struct is_signed_internal        <simdarray<T, N, V, M>, false> : public std::is_signed<T> {};
template <typename T, std::size_t N, typename V, std::size_t M> struct is_unsigned_internal      <simdarray<T, N, V, M>, false> : public std::is_unsigned<T> {};
}  // namespace Traits

/* TODO: add type for half-float, something along these lines:
class half_float
{
    uint16_t data;
public:
    constexpr half_float() : data(0) {}
    constexpr half_float(const half_float &) = default;
    constexpr half_float(half_float &&) = default;
    constexpr half_float &operator=(const half_float &) = default;

    constexpr explicit half_float(float);
    constexpr explicit half_float(double);
    constexpr explicit half_float(int);
    constexpr explicit half_float(unsigned int);

    explicit operator float       () const;
    explicit operator double      () const;
    explicit operator int         () const;
    explicit operator unsigned int() const;

    bool operator==(half_float rhs) const;
    bool operator!=(half_float rhs) const;
    bool operator>=(half_float rhs) const;
    bool operator<=(half_float rhs) const;
    bool operator> (half_float rhs) const;
    bool operator< (half_float rhs) const;

    half_float operator+(half_float rhs) const;
    half_float operator-(half_float rhs) const;
    half_float operator*(half_float rhs) const;
    half_float operator/(half_float rhs) const;
};
*/

// TODO: all of the following doesn't really belong into the toplevel Vc namespace. An anonymous
// namespace might be enough:

template<typename T> struct DetermineEntryType { typedef T Type; };

namespace VectorSpecialInitializerZero { enum ZEnum { Zero = 0 }; }
namespace VectorSpecialInitializerOne { enum OEnum { One = 1 }; }
namespace VectorSpecialInitializerIndexesFromZero { enum IEnum { IndexesFromZero }; }

#ifndef VC_ICC
// ICC ICEs if the traits below are in an unnamed namespace
namespace
{
#endif
    template<typename T> struct CanConvertToInt : public std::is_convertible<T, int> {
        static constexpr bool Value = std::is_convertible<T, int>::value;
    };
    template<> struct CanConvertToInt<bool>     { enum { Value = 0 }; };
    //template<> struct CanConvertToInt<float>    { enum { Value = 0 }; };
    //template<> struct CanConvertToInt<double>   { enum { Value = 0 }; };

    enum TestEnum {};
    static_assert(CanConvertToInt<int>::Value == 1, "CanConvertToInt_is_broken");
    static_assert(CanConvertToInt<unsigned char>::Value == 1, "CanConvertToInt_is_broken");
    static_assert(CanConvertToInt<bool>::Value == 0, "CanConvertToInt_is_broken");
    static_assert(CanConvertToInt<float>::Value == 1, "CanConvertToInt_is_broken");
    static_assert(CanConvertToInt<double>::Value == 1, "CanConvertToInt_is_broken");
    static_assert(CanConvertToInt<float*>::Value == 0, "CanConvertToInt_is_broken");
    static_assert(CanConvertToInt<TestEnum>::Value == 1, "CanConvertToInt_is_broken");

    static_assert(std::is_convertible<TestEnum, short>          ::value ==  true, "HasImplicitCast0_is_broken");
    static_assert(std::is_convertible<int *, void *>            ::value ==  true, "HasImplicitCast1_is_broken");
    static_assert(std::is_convertible<int *, const void *>      ::value ==  true, "HasImplicitCast2_is_broken");
    static_assert(std::is_convertible<const int *, const void *>::value ==  true, "HasImplicitCast3_is_broken");
    static_assert(std::is_convertible<const int *, int *>       ::value == false, "HasImplicitCast4_is_broken");

    template<typename From, typename To> struct is_implicit_cast_allowed : public std::false_type {};
    template<typename T> struct is_implicit_cast_allowed<T, T> : public std::true_type {};
    template<> struct is_implicit_cast_allowed< int32_t, uint32_t> : public std::true_type {};
    template<> struct is_implicit_cast_allowed<uint32_t,  int32_t> : public std::true_type {};
    template<> struct is_implicit_cast_allowed< int16_t, uint16_t> : public std::true_type {};
    template<> struct is_implicit_cast_allowed<uint16_t,  int16_t> : public std::true_type {};

    template<typename From, typename To> struct is_implicit_cast_allowed_mask : public is_implicit_cast_allowed<From, To> {};

    // TODO: issue #605 (make int_v <-> float_v conversion explicit only and drop the int_v::size()
    // == float_v::size() guarantee)
    template<> struct is_implicit_cast_allowed< int32_t,    float> : public std::true_type {};
    template<> struct is_implicit_cast_allowed<uint32_t,    float> : public std::true_type {};
    template<> struct is_implicit_cast_allowed_mask< float,  int32_t> : public std::true_type {};
    template<> struct is_implicit_cast_allowed_mask< float, uint32_t> : public std::true_type {};

    template<typename T> struct IsLikeInteger { enum { Value = !std::is_floating_point<T>::value && CanConvertToInt<T>::Value }; };
    template<typename T> struct IsLikeSignedInteger { enum { Value = IsLikeInteger<T>::Value && !std::is_unsigned<T>::value }; };
#ifndef VC_ICC
} // anonymous namespace
#endif

#ifndef VC_CHECK_ALIGNMENT
template<typename _T> static Vc_ALWAYS_INLINE void assertCorrectAlignment(const _T *){}
#else
template<typename _T> static Vc_ALWAYS_INLINE void assertCorrectAlignment(const _T *ptr)
{
    const size_t s = alignof(_T);
    if((reinterpret_cast<size_t>(ptr) & ((s ^ (s & (s - 1))) - 1)) != 0) {
        fprintf(stderr, "A vector with incorrect alignment has just been created. Look at the stacktrace to find the guilty object.\n");
        abort();
    }
}
#endif

}

namespace Vc_VERSIONED_NAMESPACE
{
namespace Common
{

template <typename T> using WidthT = std::integral_constant<std::size_t, sizeof(T)>;

template<std::size_t Bytes> class MaskBool;

template <typename T, typename IndexVector, typename Scale> class SubscriptOperation;

/**
 * \internal
 * Helper type to pass along the two arguments for a gather operation.
 *
 * \tparam IndexVector  Normally an integer SIMD vector, but an array or std::vector also works
 *                      (though often not as efficient).
 */
template <typename T, typename IndexVector> struct GatherArguments
{
    const IndexVector indexes;
    const T *const address;
};

/**
 * \internal
 * Helper type to pass along the two arguments for a scatter operation.
 *
 * \tparam IndexVector  Normally an integer SIMD vector, but an array or std::vector also works
 *                      (though often not as efficient).
 */
template <typename T, typename IndexVector> struct ScatterArguments
{
    const IndexVector indexes;
    T *const address;
};

template <typename I, I Begin, I End, typename F>
Vc_INTRINSIC enable_if<(Begin == End), void> unrolled_loop(F &&)
{
}

template <typename I, I Begin, I End, typename F>
Vc_INTRINSIC enable_if<(Begin < End), void> unrolled_loop(F &&f)
{
    f(Begin);
    unrolled_loop<I, Begin + 1, End>(f);
}

}  // namespace Common
}  // namespace Vc

#include "memoryfwd.h"
#include "undomacros.h"

#endif // VC_COMMON_TYPES_H

// vim: foldmethod=marker
