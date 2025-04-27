#pragma once
#include <cstdint>

namespace axdl::primedep {
enum class EPrimitiveType : uint8_t {
  Quads = 0x80,
  Triangles = 0x90,
  TriangleStrip = 0x98,
  TriangleFan = 0xA0,
  Lines = 0xA8,
  LineStrip = 0xB0,
  Points = 0xB8
};
}