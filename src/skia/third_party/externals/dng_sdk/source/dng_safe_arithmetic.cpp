#include "dng_safe_arithmetic.h"

#include <cmath>
#include <limits>

#include "dng_exceptions.h"

// Implementation of safe integer arithmetic follows guidelines from
// https://www.securecoding.cert.org/confluence/display/c/INT30-C.+Ensure+that+unsigned+integer+operations+do+not+wrap
// and
// https://www.securecoding.cert.org/confluence/display/c/INT32-C.+Ensure+that+operations+on+signed+integers+do+not+result+in+overflow

namespace {

// Template functions for safe arithmetic. These functions are not exposed in
// the header for the time being to avoid having to add checks for the various
// constraints on the template argument (e.g. that it is integral and possibly
// signed or unsigned only). This should be done using a static_assert(), but
// we want to be portable to pre-C++11 compilers.

// Returns the result of adding arg1 and arg2 if it will fit in a T (where T is
// a signed or unsigned integer type). Otherwise, throws a dng_exception with
// error code dng_error_unknown.
template <class T>
T SafeAdd(T arg1, T arg2) {
  // The condition is reformulated relative to the version on
  // www.securecoding.cert.org to check for valid instead of invalid cases. It
  // seems safer to enumerate the valid cases (and potentially miss one) than
  // enumerate the invalid cases.
  // If T is an unsigned type, the second half of the condition always evaluates
  // to false and will presumably be compiled out by the compiler.
  if ((arg1 >= 0 && arg2 <= std::numeric_limits<T>::max() - arg1) ||
      (arg1 < 0 && arg2 >= std::numeric_limits<T>::min() - arg1)) {
    return arg1 + arg2;
  } else {
    ThrowProgramError("Arithmetic overflow");
    abort();  // Never reached.
  }
}

// Returns the result of multiplying arg1 and arg2 if it will fit in a T (where
// T is an unsigned integer type). Otherwise, throws a dng_exception with error
// code dng_error_unknown.
template <class T>
T SafeUnsignedMult(T arg1, T arg2) {
  if (arg1 == 0 || arg2 <= std::numeric_limits<T>::max() / arg1) {
    return arg1 * arg2;
  } else {
    ThrowProgramError("Arithmetic overflow");
    abort();  // Never reached.
  }
}

}  // namespace

bool SafeInt32Add(std::int32_t arg1, std::int32_t arg2, std::int32_t *result) {
  try {
    *result = SafeInt32Add(arg1, arg2);
    return true;
  } catch (const dng_exception &) {
    return false;
  }
}

std::int32_t SafeInt32Add(std::int32_t arg1, std::int32_t arg2) {
  return SafeAdd<std::int32_t>(arg1, arg2);
}

std::int64_t SafeInt64Add(std::int64_t arg1, std::int64_t arg2) {
  return SafeAdd<std::int64_t>(arg1, arg2);
}

bool SafeUint32Add(std::uint32_t arg1, std::uint32_t arg2,
                   std::uint32_t *result) {
  try {
    *result = SafeUint32Add(arg1, arg2);
    return true;
  } catch (const dng_exception &) {
    return false;
  }
}

std::uint32_t SafeUint32Add(std::uint32_t arg1, std::uint32_t arg2) {
  return SafeAdd<std::uint32_t>(arg1, arg2);
}

std::uint64_t SafeUint64Add(std::uint64_t arg1, std::uint64_t arg2) {
  return SafeAdd<std::uint64_t>(arg1, arg2);
}

bool SafeInt32Sub(std::int32_t arg1, std::int32_t arg2, std::int32_t *result) {
  if ((arg2 >= 0 && arg1 >= std::numeric_limits<int32_t>::min() + arg2) ||
      (arg2 < 0 && arg1 <= std::numeric_limits<int32_t>::max() + arg2)) {
    *result = arg1 - arg2;
    return true;
  } else {
    return false;
  }
}

std::int32_t SafeInt32Sub(std::int32_t arg1, std::int32_t arg2) {
  std::int32_t result = 0;

  if (!SafeInt32Sub(arg1, arg2, &result)) {
    ThrowProgramError("Arithmetic overflow");
  }

  return result;
}

std::uint32_t SafeUint32Sub(std::uint32_t arg1, std::uint32_t arg2) {
  if (arg1 >= arg2) {
    return arg1 - arg2;
  } else {
    ThrowProgramError("Arithmetic overflow");
    abort();  // Never reached.
  }
}

bool SafeUint32Mult(std::uint32_t arg1, std::uint32_t arg2,
                    std::uint32_t *result) {
  try {
    *result = SafeUint32Mult(arg1, arg2);
    return true;
  } catch (const dng_exception &) {
    return false;
  }
}

bool SafeUint32Mult(std::uint32_t arg1, std::uint32_t arg2, std::uint32_t arg3,
                    std::uint32_t *result) {
  try {
    *result = SafeUint32Mult(arg1, arg2, arg3);
    return true;
  } catch (const dng_exception &) {
    return false;
  }
}

bool SafeUint32Mult(std::uint32_t arg1, std::uint32_t arg2, std::uint32_t arg3,
                    std::uint32_t arg4, std::uint32_t *result) {
  try {
    *result = SafeUint32Mult(arg1, arg2, arg3, arg4);
    return true;
  } catch (const dng_exception &) {
    return false;
  }
}

std::uint32_t SafeUint32Mult(std::uint32_t arg1, std::uint32_t arg2) {
  return SafeUnsignedMult<std::uint32_t>(arg1, arg2);
}

std::uint32_t SafeUint32Mult(std::uint32_t arg1, std::uint32_t arg2,
                             std::uint32_t arg3) {
  return SafeUint32Mult(SafeUint32Mult(arg1, arg2), arg3);
}

std::uint32_t SafeUint32Mult(std::uint32_t arg1, std::uint32_t arg2,
                             std::uint32_t arg3, std::uint32_t arg4) {
  return SafeUint32Mult(SafeUint32Mult(arg1, arg2, arg3), arg4);
}

std::int32_t SafeInt32Mult(std::int32_t arg1, std::int32_t arg2) {
  const std::int64_t tmp =
      static_cast<std::int64_t>(arg1) * static_cast<std::int64_t>(arg2);
  if (tmp >= std::numeric_limits<std::int32_t>::min() &&
      tmp <= std::numeric_limits<std::int32_t>::max()) {
    return static_cast<std::int32_t>(tmp);
  } else {
    ThrowProgramError("Arithmetic overflow");
    abort();
  }
}

std::size_t SafeSizetMult(std::size_t arg1, std::size_t arg2) {
  return SafeUnsignedMult<std::size_t>(arg1, arg2);
}

namespace dng_internal {

std::int64_t SafeInt64MultSlow(std::int64_t arg1, std::int64_t arg2) {
  bool overflow = true;

  if (arg1 > 0) {
    if (arg2 > 0) {
      overflow = (arg1 > std::numeric_limits<std::int64_t>::max() / arg2);
    } else {
      overflow = (arg2 < std::numeric_limits<std::int64_t>::min() / arg1);
    }
  } else {
    if (arg2 > 0) {
      overflow = (arg1 < std::numeric_limits<std::int64_t>::min() / arg2);
    } else {
      overflow = (arg1 != 0 &&
                  arg2 < std::numeric_limits<std::int64_t>::max() / arg1);
    }
  }

  if (overflow) {
    ThrowProgramError("Arithmetic overflow");
    abort();  // Never reached.
  } else {
    return arg1 * arg2;
  }
}

}  // namespace dng_internal

std::uint32_t SafeUint32DivideUp(std::uint32_t arg1, std::uint32_t arg2) {
  // It might seem more intuitive to implement this function simply as
  //
  //   return arg2 == 0 ? 0 : (arg1 + arg2 - 1) / arg2;
  //
  // but the expression "arg1 + arg2" can wrap around.

  if (arg2 == 0) {
    ThrowProgramError("Division by zero");
    abort();  // Never reached.
  } else if (arg1 == 0) {
    // If arg1 is zero, return zero to avoid wraparound in the expression
    //   "arg1 - 1" below.
    return 0;
  } else {
    return (arg1 - 1) / arg2 + 1;
  }
}

bool RoundUpUint32ToMultiple(std::uint32_t val, std::uint32_t multiple_of,
                             std::uint32_t *result) {
  try {
    *result = RoundUpUint32ToMultiple(val, multiple_of);
    return true;
  } catch (const dng_exception &) {
    return false;
  }
}

std::uint32_t RoundUpUint32ToMultiple(std::uint32_t val,
                                      std::uint32_t multiple_of) {
  if (multiple_of == 0) {
    ThrowProgramError("multiple_of is zero in RoundUpUint32ToMultiple");
  }

  const std::uint32_t remainder = val % multiple_of;
  if (remainder == 0) {
    return val;
  } else {
    return SafeUint32Add(val, multiple_of - remainder);
  }
}

bool ConvertUint32ToInt32(std::uint32_t val, std::int32_t *result) {
  try {
    *result = ConvertUint32ToInt32(val);
    return true;
  } catch (const dng_exception &) {
    return false;
  }
}

std::int32_t ConvertUint32ToInt32(std::uint32_t val) {
  const std::uint32_t kInt32MaxAsUint32 =
      static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max());

  if (val <= kInt32MaxAsUint32) {
    return static_cast<std::int32_t>(val);
  } else {
    ThrowProgramError("Arithmetic overflow");
    abort();  // Never reached.
  }
}

std::int32_t ConvertDoubleToInt32(double val) {
  const double kMin =
      static_cast<double>(std::numeric_limits<std::int32_t>::min());
  const double kMax =
      static_cast<double>(std::numeric_limits<std::int32_t>::max());
  // NaNs will fail this test; they always compare false.
  if (val > kMin - 1.0 && val < kMax + 1.0) {
    return static_cast<std::int32_t>(val);
  } else {
    ThrowProgramError("Argument not in range in ConvertDoubleToInt32");
    abort();  // Never reached.
  }
}

std::uint32_t ConvertDoubleToUint32(double val) {
  const double kMax =
      static_cast<double>(std::numeric_limits<std::uint32_t>::max());
  // NaNs will fail this test; they always compare false.
  if (val >= 0.0 && val < kMax + 1.0) {
    return static_cast<std::uint32_t>(val);
  } else {
    ThrowProgramError("Argument not in range in ConvertDoubleToUint32");
    abort();  // Never reached.
  }
}

float ConvertDoubleToFloat(double val) {
  const double kMax = std::numeric_limits<float>::max();
  if (val > kMax) {
    return std::numeric_limits<float>::infinity();
  } else if (val < -kMax) {
    return -std::numeric_limits<float>::infinity();
  } else {
    // The cases that end up here are:
    // - values in [-kMax, kMax]
    // - NaN (because it always compares false)
    return static_cast<float>(val);
  }
}
