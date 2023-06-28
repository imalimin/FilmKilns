#ifndef __dng_RawEnvironment__
#define __dng_RawEnvironment__

// Define preprocessor constants that control platform-specific conditional
// compilation. The constants qMacOS and qWinOS must be defined on all
// platforms. Other constants, such as qLinux, only need to be defined if we're
// actually compiling for that platform.
#if defined(__linux__)
#define qMacOS 0
#define qWinOS 0
#define qLinux 1
#elif defined(__APPLE__)
#define qMacOS 1
#define qWinOS 0
#elif defined(_WIN32)
#define qMacOS 0
#define qWinOS 1
#endif

#endif  // __dng_RawEnvironment__
