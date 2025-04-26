#include "PrimeDep/Resources/RasterFont.hpp"

namespace axdl::primedep {
RasterFont::RasterFont(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc) : ITypedResource(desc) {
  delete[] ptr;
}

} // namespace axdl::primedep