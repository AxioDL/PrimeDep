#pragma once

#include <endian.h>

#undef bswap16
#undef bswap32
#undef bswap64

/* Type-sensitive byte swappers */
template <typename T>
constexpr T bswap16(T val) noexcept {
#if __GNUC__
  return __builtin_bswap16(val);
#elif _WIN32
  return _byteswap_ushort(val);
#else
  return (val = (val << 8) | ((val >> 8) & 0xFF));
#endif
}

template <typename T>
constexpr T bswap32(T val) noexcept {
#if __GNUC__
  return __builtin_bswap32(val);
#elif _WIN32
  return _byteswap_ulong(val);
#else
  val = (val & 0x0000FFFF) << 16 | (val & 0xFFFF0000) >> 16;
  val = (val & 0x00FF00FF) << 8 | (val & 0xFF00FF00) >> 8;
  return val;
#endif
}

template <typename T>
constexpr T bswap64(T val) noexcept {
#if __GNUC__
  return __builtin_bswap64(val);
#elif _WIN32
  return _byteswap_uint64(val);
#else
  return ((val & 0xFF00000000000000ULL) >> 56) | ((val & 0x00FF000000000000ULL) >> 40) |
         ((val & 0x0000FF0000000000ULL) >> 24) | ((val & 0x000000FF00000000ULL) >> 8) |
         ((val & 0x00000000FF000000ULL) << 8) | ((val & 0x0000000000FF0000ULL) << 24) |
         ((val & 0x000000000000FF00ULL) << 40) | ((val & 0x00000000000000FFULL) << 56);
#endif
}

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
constexpr int16_t SBig(int16_t val) noexcept { return bswap16(val); }
constexpr uint16_t SBig(uint16_t val) noexcept { return bswap16(val); }
constexpr int32_t SBig(int32_t val) noexcept { return bswap32(val); }
constexpr uint32_t SBig(uint32_t val) noexcept { return bswap32(val); }
constexpr int64_t SBig(int64_t val) noexcept { return bswap64(val); }
constexpr uint64_t SBig(uint64_t val) noexcept { return bswap64(val); }
constexpr float SBig(float val) noexcept {
  union {
    float f;
    uint32_t i;
  } uval1 = {val};
  union {
    uint32_t i;
    float f;
  } uval2 = {bswap32(uval1.i)};
  return uval2.f;
}
constexpr double SBig(double val) noexcept {
  union {
    double f;
    uint64_t i;
  } uval1 = {val};
  union {
    uint64_t i;
    double f;
  } uval2 = {bswap64(uval1.i)};
  return uval2.f;
}
#ifndef SBIG
#define SBIG(q)                                                                                                        \
  (((q) & 0x000000FF) << 24 | ((q) & 0x0000FF00) << 8 | ((q) & 0x00FF0000) >> 8 | ((q) & 0xFF000000) >> 24)
#endif

constexpr int16_t SLittle(int16_t val) noexcept { return val; }
constexpr uint16_t SLittle(uint16_t val) noexcept { return val; }
constexpr int32_t SLittle(int32_t val) noexcept { return val; }
constexpr uint32_t SLittle(uint32_t val) noexcept { return val; }
constexpr int64_t SLittle(int64_t val) noexcept { return val; }
constexpr uint64_t SLittle(uint64_t val) noexcept { return val; }
constexpr float SLittle(float val) noexcept { return val; }
constexpr double SLittle(double val) noexcept { return val; }
#ifndef SLITTLE
#define SLITTLE(q) (q)
#endif
#else
constexpr int16_t SLittle(int16_t val) noexcept { return bswap16(val); }
constexpr uint16_t SLittle(uint16_t val) noexcept { return bswap16(val); }
constexpr int32_t SLittle(int32_t val) noexcept { return bswap32(val); }
constexpr uint32_t SLittle(uint32_t val) noexcept { return bswap32(val); }
constexpr int64_t SLittle(int64_t val) noexcept { return bswap64(val); }
constexpr uint64_t SLittle(uint64_t val) noexcept { return bswap64(val); }
constexpr float SLittle(float val) noexcept {
  int32_t ival = bswap32(*((int32_t*)(&val)));
  return *((float*)(&ival));
}
constexpr double SLittle(double val) noexcept {
  int64_t ival = bswap64(*((int64_t*)(&val)));
  return *((double*)(&ival));
}
#ifndef SLITTLE
#define SLITTLE(q)                                                                                                     \
  (((q) & 0x000000FF) << 24 | ((q) & 0x0000FF00) << 8 | ((q) & 0x00FF0000) >> 8 | ((q) & 0xFF000000) >> 24)
#endif

constexpr int16_t SBig(int16_t val) noexcept { return val; }
constexpr uint16_t SBig(uint16_t val) noexcept { return val; }
constexpr int32_t SBig(int32_t val) noexcept { return val; }
constexpr uint32_t SBig(uint32_t val) noexcept { return val; }
constexpr int64_t SBig(int64_t val) noexcept { return val; }
constexpr uint64_t SBig(uint64_t val) noexcept { return val; }
constexpr float SBig(float val) noexcept { return val; }
constexpr double SBig(double val) noexcept { return val; }
#ifndef SBIG
#define SBIG(q) (q)
#endif
#endif