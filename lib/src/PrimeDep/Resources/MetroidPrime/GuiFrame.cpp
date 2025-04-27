#include "PrimeDep/Resources/MetroidPrime/GuiFrame.hpp"

#include <athena/MemoryReader.hpp>

namespace axdl::primedep ::MetroidPrime {
GuiFrame::GuiFrame(const char* ptr, const std::size_t size) { athena::io::MemoryReader in(ptr, size, true); }

} // namespace axdl::primedep::MetroidPrime