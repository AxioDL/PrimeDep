#pragma once

#include <functional>
#include <map>
#include <memory>
#include <utility>

namespace axdl::primedep {
class IResource;
class ResourceFactory {
public:
  /**
   * Registered factories are given a buffer object that they own, they are responsible for properly cleaning it up
   */
  using FactoryFunc = std::function<std::shared_ptr<IResource>(const char* ptr, std::size_t size)>;

  void registerFactory(const uint32_t type, FactoryFunc func) {
    if (m_factories.contains(type)) {
      return;
    }
    m_factories[type] = std::move(func);
  }

  [[nodiscard]] FactoryFunc factory(const uint32_t type) const {
    if (m_factories.contains(type)) {
      return m_factories.at(type);
    }
    return nullptr;
  }

private:
  std::map<uint32_t, FactoryFunc> m_factories;
};
} // namespace axdl::primedep
