#include "PrimeDep/Resources/MetroidPrime/ScannableObjectInfo.hpp"

namespace axdl::primedep::MetroidPrime {
ScannableObjectInfo::ScannableObjectInfo(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc)
: ITypedResource(desc) {
  delete[] ptr;
}

} // namespace axdl::primedep