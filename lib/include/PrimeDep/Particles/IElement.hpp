#pragma once
#include "PrimeDep/FourCC.hpp"
#include "nlohmann/json_fwd.hpp"

namespace athena::io {
class IStreamReader;
class IStreamWriter;
} // namespace athena::io

namespace axdl::primedep::particles {
class IElement {
public:
  explicit IElement(athena::io::IStreamReader& reader) {}
  explicit IElement(const nlohmann::ordered_json& reader) {}

  virtual ~IElement() = default;
  void PutTo(athena::io::IStreamWriter& out) const;
  void PutTo(nlohmann::ordered_json& out) const;

  virtual bool isValid() const = 0;
  virtual constexpr FourCC classId() const = 0;
  virtual constexpr std::string_view className() const = 0;

  void setPropertyId(const FourCC& classId) { m_propertyId = classId; }
  void setPropertyName(const std::string_view propertyName) { m_propertyName = propertyName; }

private:
  virtual void PutToInternal(athena::io::IStreamWriter& out) const = 0;
  virtual void PutToInternal(nlohmann::ordered_json& out) const = 0;
  FourCC m_propertyId;
  std::string m_propertyName;
};

class BoolElement final : public IElement {
public:
  explicit BoolElement(bool value, athena::io::IStreamReader& reader);
  explicit BoolElement(bool value, const nlohmann::ordered_json& reader);

  bool isValid() const override { return true; }

  FourCC classId() const override { return kInvalidFourCC; }
  std::string_view className() const override { return {}; }

private:
  void PutToInternal(athena::io::IStreamWriter& out) const override;
  void PutToInternal(nlohmann::ordered_json& out) const override;
  bool m_value{};
};

#define DEFINE_ELEMENT(clsId, clsName)                                                                                 \
  static constexpr FourCC ClassID() { return FOURCC(clsId); }                                                          \
  static constexpr std::string_view ClassName() { return clsName; }                                                    \
  constexpr FourCC classId() const override { return FOURCC(clsId); }                                                  \
  constexpr std::string_view className() const override { return clsName; }

} // namespace axdl::primedep::particles