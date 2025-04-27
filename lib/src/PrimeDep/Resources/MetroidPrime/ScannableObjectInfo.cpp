#include "PrimeDep/Resources/MetroidPrime/ScannableObjectInfo.hpp"

namespace axdl::primedep::MetroidPrime {
ScannableObjectInfo::ScannableObjectInfo(const char* ptr, const std::size_t size) { delete[] ptr; }

} // namespace axdl::primedep::MetroidPrime