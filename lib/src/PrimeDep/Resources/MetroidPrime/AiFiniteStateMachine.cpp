#include "PrimeDep/Resources/MetroidPrime/AiFiniteStateMachine.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
AiFiniteStateMachine::Trigger::Trigger(athena::io::IStreamReader& in, const bool first)
: m_name(in.readString()), m_parameter(in.readFloatBig()) {
  if (first) {
    m_targetState = in.readUint32Big();
  }
}
void AiFiniteStateMachine::Trigger::PutTo(nlohmann::ordered_json& out, const bool first) const {
  out["Name"] = m_name;
  out["Parameter"] = m_parameter;
  if (first) {
    out["TargetState"] = m_targetState;
  }
}

AiFiniteStateMachine::Transition::Transition(athena::io::IStreamReader& in) {
  auto triggerCount = in.readUint32Big();
  bool first = true;
  while (triggerCount--) {
    m_triggers.emplace_back(in, first);
    first = false;
  }
}

void AiFiniteStateMachine::Transition::PutTo(nlohmann::ordered_json& out) const {
  auto& triggers = out["Triggers"];
  bool first = true;
  for (const auto& trigger : m_triggers) {
    trigger.PutTo(triggers.emplace_back(), first);
    first = false;
  }
}

AiFiniteStateMachine::State::State(athena::io::IStreamReader& in) {
  auto transitionCount = in.readUint32Big();
  while (transitionCount--) {
    m_transitions.emplace_back(in);
  }
}

void AiFiniteStateMachine::State::PutTo(nlohmann::ordered_json& out) const {
  out["Name"] = m_name;
  auto& transitions = out["Transitions"];
  for (const auto& transition : m_transitions) {
    transition.PutTo(transitions.emplace_back());
  }
}

AiFiniteStateMachine::AiFiniteStateMachine(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader in(ptr, size, true);
  const auto stateCount = in.readUint32Big();
  std::vector<std::string> names;

  for (int i = 0; i < stateCount; i++) {
    names.push_back(in.readString());
  }

  in.readUint32Big(); // Total number of triggers in this state machine

  for (int i = 0; i < stateCount; i++) {
    auto& state = m_states.emplace_back(in);
    state.setName(names[i]);
  }
}

bool AiFiniteStateMachine::writeUncooked(const std::string_view path) const {
  const auto p = rawPath(path);

  nlohmann::ordered_json j;
  auto& states = j["States"];
  for (const auto& state : m_states) {
    state.PutTo(states.emplace_back());
  }

  athena::io::FileWriter writer(p.generic_string());
  const auto js = j.dump(4) + "\n";
  writer.writeString(js, js.length());
  return !writer.hasError();
}

std::shared_ptr<IResource> AiFiniteStateMachine::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<AiFiniteStateMachine>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime