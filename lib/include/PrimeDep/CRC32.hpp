#pragma once

#include "PrimeDep/Types.hpp"

namespace axdl::primedep {
// Taken from metaforce
class CRC32 {
public:
  static uint32_t Calculate(const void* data, uint32_t length);
};

} // namespace axdl::primedep