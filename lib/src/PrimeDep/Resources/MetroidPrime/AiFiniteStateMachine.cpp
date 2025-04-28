#include "PrimeDep/Resources/MetroidPrime/AiFiniteStateMachine.hpp"

#include "athena/MemoryReader.hpp"

namespace axdl::primedep::MetroidPrime {
AiFiniteStateMachine::Trigger::Trigger(athena::io::IStreamReader& in)
: m_name(in.readString()), m_parameter(in.readFloatBig()) {}
void AiFiniteStateMachine::Trigger::PutTo(athena::io::IStreamWriter& out) const {
  out.writeString(m_name);
  out.writeFloatBig(m_parameter);
}

void AiFiniteStateMachine::Trigger::PutTo(nlohmann::ordered_json& out) const {
  out["Name"] = m_name;
  out["Parameter"] = m_parameter;
}

AiFiniteStateMachine::Transition::Transition(athena::io::IStreamReader& in) {
  auto triggerCount = in.readUint32Big();
  bool first = true;
  while (triggerCount--) {
    m_triggers.emplace_back(in);
    if (first) {
      m_targetState = in.readUint32Big();
      first = false;
    }
  }
}

void AiFiniteStateMachine::Transition::PutTo(athena::io::IStreamWriter& out) const {
  out.writeUint32Big(m_triggers.size());
  bool first = true;
  for (const auto& trigger : m_triggers) {
    trigger.PutTo(out);
    if (first) {
      out.writeUint32Big(m_targetState);
      first = false;
    }
  }
}

void AiFiniteStateMachine::Transition::PutTo(nlohmann::ordered_json& out) const {
  out["TargetState"] = m_targetState;
  auto& triggers = out["Triggers"];
  for (const auto& trigger : m_triggers) {
    trigger.PutTo(triggers.emplace_back());
  }
}

AiFiniteStateMachine::State::State(athena::io::IStreamReader& in) {
  auto transitionCount = in.readUint32Big();
  while (transitionCount--) {
    m_transitions.emplace_back(in);
  }
}

void AiFiniteStateMachine::State::PutTo(athena::io::IStreamWriter& out) const {
  out.writeUint32Big(m_transitions.size());
  for (const auto& transition : m_transitions) {
    transition.PutTo(out);
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

  in.readUint32Big(); // Total number of triggers in this state machine, we can calculate this at cook time, no need to
                      // store it

  for (int i = 0; i < stateCount; i++) {
    auto& state = m_states.emplace_back(in);
    state.setName(names[i]);
  }
}

bool AiFiniteStateMachine::writeCooked(const std::string_view path) const {
  const auto p = cookedPath(path);
  athena::io::FileWriter out(p.generic_string());
  out.writeUint32Big(m_states.size());

  uint32_t triggerCount = 0;
  for (const auto& state : m_states) {
    std::string name = state.name().substr(0, std::min<size_t>(state.name().length(), 32));
    out.writeString(name);
    // Calculate the total number of transitions per state
    for (const auto& transition : state.transitions()) {
      triggerCount += transition.triggers().size();
    }
  }
  out.writeUint32Big(triggerCount);

  for (const auto& state : m_states) {
    state.PutTo(out);
  }

  return !out.hasError();
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