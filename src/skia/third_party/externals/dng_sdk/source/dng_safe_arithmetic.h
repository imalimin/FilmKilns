/*
 *
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Functions for safe arithmetic (guarded against overflow) on integer types.

#ifndef __dng_safe_arithmetic__
#define __dng_safe_arithmetic__

#include <cstddef>
#include <cstdint>
#include <limits>

#include "dng_exceptions.h"

#ifndef __has_builtin
#define __has_builtin(x) 0  // Compatibility with non-Clang compilers.
#endif

#if !defined(DNG_HAS_INT128) && defined(__SIZEOF_INT128__)
#define DNG_HAS_INT128
#endif

// If the result of adding arg1 and arg2 will fit in an int32_t (without
// under-/overflow), stores this result in *result and returns true. Otherwise,
// returns false and leaves *result unchanged.
bool SafeInt32Add(std::int32_t arg1, std::int32_t arg2, std::int32_t *result);

// Returns the result of adding arg1 and arg2 if it will fit in the result type
// (without under-/overflow). Otherwise, throws a dng_exception with error code
// dng_error_unknown.
std::int32_t SafeInt32Add(std::int32_t arg1, std::int32_t arg2);
std::int64_t SafeInt64Add(std::int64_t arg1, std::int64_t arg2);

// If the result of adding arg1 and arg2 will fit in a uint32_t (without
// wraparound), stores this result in *result and returns true. Otherwise,
// returns false and leaves *result unchanged.
bool SafeUint32Add(std::uint32_t arg1, std::uint32_t arg2,
                   std::uint32_t *result);

// Returns the result of adding arg1 and arg2 if it will fit in the result type
// (without wraparound). Otherwise, throws a dng_exception with error code
// dng_error_unknown.
std::uint32_t SafeUint32Add(std::uint32_t arg1, std::uint32_t arg2);
std::uint64_t SafeUint64Add(std::uint64_t arg1, std::uint64_t arg2);

// If the subtraction of arg2 from arg1 will not result in an int32_t under- or
// overflow, stores this result in *result and returns true. Otherwise,
// returns false and leaves *result unchanged.
bool SafeInt32Sub(std::int32_t arg1, std::int32_t arg2, std::int32_t *result);

// Returns the result of subtracting arg2 from arg1 if this operation will not
// result in an int32_t under- or overflow. Otherwise, throws a dng_exception
// with error code dng_error_unknown.
std::int32_t SafeInt32Sub(std::int32_t arg1, std::int32_t arg2);

// Returns the result of subtracting arg2 from arg1 if this operation will not
// result in wraparound. Otherwise, throws a dng_exception with error code
// dng_error_unknown.
std::uint32_t SafeUint32Sub(std::uint32_t arg1, std::uint32_t arg2);

// Returns the result of multiplying arg1 and arg2 if it will fit in a int32_t
// (without overflow). Otherwise, throws a dng_exception with error code
// dng_error_unknown.
std::int32_t SafeInt32Mult(std::int32_t arg1, std::int32_t arg2);

// If the result of multiplying arg1, ..., argn will fit in a uint32_t (without
// wraparound), stores this result in *result and returns true. Otherwise,
// returns false and leaves *result unchanged.
bool SafeUint32Mult(std::uint32_t arg1, std::uint32_t arg2,
                    std::uint32_t *result);
bool SafeUint32Mult(std::uint32_t arg1, std::uint32_t arg2, std::uint32_t arg3,
                    std::uint32_t *result);
bool SafeUint32Mult(std::uint32_t arg1, std::uint32_t arg2, std::uint32_t arg3,
                    std::uint32_t arg4, std::uint32_t *result);

// Returns the result of multiplying arg1, ..., argn if it will fit in a
// uint32_t (without wraparound). Otherwise, throws a dng_exception with error
// code dng_error_unknown.
std::uint32_t SafeUint32Mult(std::uint32_t arg1, std::uint32_t arg2);
std::uint32_t SafeUint32Mult(std::uint32_t arg1, std::uint32_t arg2,
                             std::uint32_t arg3);
std::uint32_t SafeUint32Mult(std::uint32_t arg1, std::uint32_t arg2,
                             std::uint32_t arg3, std::uint32_t arg4);

// Returns the result of multiplying arg1 and arg2 if it will fit in a size_t
// (without overflow). Otherwise, throws a dng_exception with error code
// dng_error_unknown.
std::size_t SafeSizetMult(std::size_t arg1, std::size_t arg2);

namespace dng_internal {

// Internal function used as fallback for SafeInt64Mult() if other optimized
// computation is not supported. Don't call this function directly.
std::int64_t SafeInt64MultSlow(std::int64_t arg1, std::int64_t arg2);

// Internal function used as optimization for SafeInt64Mult() if Clang
// __builtin_smull_overflow is supported. Don't call this function directly.
#if __has_builtin(__builtin_smull_overflow)
inline std::int64_t SafeInt64MultByClang(std::int64_t arg1, std::int64_t arg2) {
  std::int64_t result;
#if (__WORDSIZE == 64) && !defined(__APPLE__)
  if (__builtin_smull_overflow(arg1, arg2, &result)) {
#else
  if (__builtin_smulll_overflow(arg1, arg2, &result)) {
#endif
    ThrowProgramError("Arithmetic overflow");
    abort();  // Never reached.
  }
  return result;
}
#endif

// Internal function used as optimization for SafeInt64Mult() if __int128 type
// is supported. Don't call this function directly.
#ifdef DNG_HAS_INT128
inline std::int64_t SafeInt64MultByInt128(std::int64_t arg1,
                                          std::int64_t arg2) {
  const __int128 kInt64Max =
      static_cast<__int128>(std::numeric_limits<std::int64_t>::max());
  const __int128 kInt64Min =
      static_cast<__int128>(std::numeric_limits<std::int64_t>::min());
  __int128 result = static_cast<__int128>(arg1) * static_cast<__int128>(arg2);
  if (result > kInt64Max || result < kInt64Min) {
    ThrowProgramError("Arithmetic overflow");
  }
  return static_cast<std::int64_t>(result);
}
#endif

}  // namespace dng_internal

// Returns the result of multiplying arg1 and arg2 if it will fit in an int64_t
// (without overflow). Otherwise, throws a dng_exception with error code
// dng_error_unknown.
inline std::int64_t SafeInt64Mult(std::int64_t arg1, std::int64_t arg2) {
#if __has_builtin(__builtin_smull_overflow)
  return dng_internal::SafeInt64MultByClang(arg1, arg2);
#elif defined(DNG_HAS_INT128)
  return dng_internal::SafeInt64MultByInt128(arg1, arg2);
#else
  return dng_internal::SafeInt64MultSlow(arg1, arg2);
#endif
}

// Returns the result of dividing arg1 by arg2; if the result is not an integer,
// rounds up to the next integer. If arg2 is zero, throws a dng_exception with
// error code dng_error_unknown.
// The function is safe against wraparound and will return the correct result
// for all combinations of arg1 and arg2.
std::uint32_t SafeUint32DivideUp(std::uint32_t arg1, std::uint32_t arg2);

// Finds the smallest integer multiple of 'multiple_of' that is greater than or
// equal to 'val'. If this value will fit in a uint32_t, stores it in *result
// and returns true. Otherwise, or if 'multiple_of' is zero, returns false and
// leaves *result unchanged.
bool RoundUpUint32ToMultiple(std::uint32_t val, std::uint32_t multiple_of,
                             std::uint32_t *result);

// Returns the smallest integer multiple of 'multiple_of' that is greater than
// or equal to 'val'. If the result will not fit in a std::uint32_t or if
// 'multiple_of' is zero, throws a dng_exception with error code
// dng_error_unknown.
std::uint32_t RoundUpUint32ToMultiple(std::uint32_t val,
                                      std::uint32_t multiple_of);

// If the uint32_t value val will fit in a int32_t, converts it to a int32_t and
// stores it in *result. Otherwise, returns false and leaves *result unchanged.
bool ConvertUint32ToInt32(std::uint32_t val, std::int32_t *result);

// Returns the result of converting val to an int32_t if it can be converted
// without overflow. Otherwise, throws a dng_exception with error code
// dng_error_unknown.
std::int32_t ConvertUint32ToInt32(std::uint32_t val);

// Converts a value of the unsigned integer type TSrc to the unsigned integer
// type TDest. If the value in 'src' cannot be converted to the type TDest
// without truncation, throws a dng_exception with error code dng_error_unknown.
//
// Note: Though this function is typically used where TDest is a narrower type
// than TSrc, it is designed to work also if TDest is wider than from TSrc or
// identical to TSrc. This is useful in situations where the width of the types
// involved can change depending on the architecture -- for example, the
// conversion from size_t to uint32_t may either be narrowing, identical or even
// widening (though the latter admittedly happens only on architectures that
// aren't relevant to us).
template <class TSrc, class TDest>
static void ConvertUnsigned(TSrc src, TDest *dest) {
  static_assert(std::numeric_limits<TSrc>::is_integer &&
                    !std::numeric_limits<TSrc>::is_signed &&
                    std::numeric_limits<TDest>::is_integer &&
                    !std::numeric_limits<TDest>::is_signed,
                "TSrc and TDest must be unsigned integer types");

  const TDest converted = static_cast<TDest>(src);

  // Convert back to TSrc to check whether truncation occurred in the
  // conversion to TDest.
  if (static_cast<TSrc>(converted) != src) {
    ThrowProgramError("Overflow in unsigned integer conversion");
  }

  *dest = converted;
}

// Returns the result of converting val to the result type using truncation if
// val is in range of the result type values. Otherwise, throws a dng_exception
// with error code dng_error_unknown.
std::int32_t ConvertDoubleToInt32(double val);
std::uint32_t ConvertDoubleToUint32(double val);

// Returns the result of converting val to float. If val is outside of
// [-FLT_MAX, FLT_MAX], -infinity and infinity is returned respectively. NaN is
// returned as NaN.
float ConvertDoubleToFloat(double val);

#endif  // __dng_safe_arithmetic__
