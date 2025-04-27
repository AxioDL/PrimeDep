#include "PrimeDep/Resources/MetroidPrime/RasterFont.hpp"

namespace axdl::primedep ::MetroidPrime {
RasterFont::RasterFont(const char* ptr, const std::size_t size) { delete[] ptr; }

} // namespace axdl::primedep::MetroidPrime