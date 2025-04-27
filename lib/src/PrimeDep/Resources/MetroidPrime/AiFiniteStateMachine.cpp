#include "PrimeDep/Resources/MetroidPrime/AiFiniteStateMachine.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
AiFiniteStateMachine::AiFiniteStateMachine(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> AiFiniteStateMachine::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<AiFiniteStateMachine>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime