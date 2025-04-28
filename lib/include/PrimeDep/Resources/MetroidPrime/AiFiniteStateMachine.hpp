#pragma once

#include "PrimeDep/IResource.hpp"
namespace axdl::primedep::MetroidPrime {
class AiFiniteStateMachine final
: public TypedResource('AFSM', ".statemachine", ".afsm", DESCRIPTION("Ai Finite State Machine")) {
public:
  class Trigger {
  public:
    explicit Trigger(athena::io::IStreamReader& in, bool first);

    [[nodiscard]] const std::string& name() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }

    [[nodiscard]] float parameter() const { return m_parameter; }
    void setParameter(const float parameter) { m_parameter = parameter; }

    [[nodiscard]] uint32_t targetState() const { return m_targetState; }
    void setTargetState(const uint32_t state) { m_targetState = state; }

    void PutTo(nlohmann::ordered_json& out, bool first) const;

  private:
    std::string m_name;
    float m_parameter{};
    uint32_t m_targetState{};
  };

  class Transition {
  public:
    explicit Transition(athena::io::IStreamReader& in);

    [[nodiscard]] const std::vector<Trigger>& triggers() const { return m_triggers; }
    void setTransitions(const std::vector<Trigger>& triggers) { m_triggers = triggers; }

    void PutTo(nlohmann::ordered_json& out) const;

  private:
    std::vector<Trigger> m_triggers;
  };

  class State {
  public:
    explicit State(athena::io::IStreamReader& in);

    [[nodiscard]] const std::string& name() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }

    [[nodiscard]] const std::vector<Transition>& transitions() const { return m_transitions; }
    void setTransitions(const std::vector<Transition>& transitions) { m_transitions = transitions; }

    void PutTo(nlohmann::ordered_json& out) const;

  private:
    std::string m_name;
    std::vector<Transition> m_transitions;
  };

  AiFiniteStateMachine(const char* ptr, std::size_t size);

  bool writeUncooked(std::string_view path) const override;

  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);

  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view repPath) {
    return nullptr;
  }

private:
  std::vector<State> m_states;
};
} // namespace axdl::primedep::MetroidPrime