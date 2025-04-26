#include "PrimeDep/Resources/GuiFrame.hpp"

namespace axdl::primedep {
GuiFrame::GuiFrame(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc) : ITypedResource(desc) {
  delete[] ptr;
}

} // namespace axdl::primedep