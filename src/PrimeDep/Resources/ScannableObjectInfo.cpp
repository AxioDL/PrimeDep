#include "PrimeDep/Resources/ScannableObjectInfo.hpp"

namespace axdl::primedep {
ScannableObjectInfo::ScannableObjectInfo(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc)
: ITypedResource(desc) {
  delete[] ptr;
}

} // namespace axdl::primedep