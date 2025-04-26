#include "PrimeDep/Resources/AiFiniteStateMachine.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep {
AiFiniteStateMachine::AiFiniteStateMachine(const char* ptr, std::size_t size, const ResourceDescriptor32Big& desc)
: ITypedResource(desc) {
  athena::io::MemoryReader mr(ptr, size, true);
}
std::shared_ptr<IResource> AiFiniteStateMachine::loadCooked(const char* ptr, std::size_t size,
                                                            const ResourceDescriptor32Big& desc) {
  return std::make_shared<AiFiniteStateMachine>(ptr, size, desc);
}
} // namespace axdl::primedep