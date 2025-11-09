#pragma once
#include "nlohmann/json_fwd.hpp"

namespace athena::io {
class IStreamReader;
class IStreamWriter;
} // namespace athena::io

namespace axdl::primedep::particles {
class IElement {
public:
  virtual ~IElement() = default;
  virtual void PutTo(athena::io::IStreamWriter& out) const = 0;
  virtual void PutTo(nlohmann::ordered_json& out) const = 0;
};

}; // namespace axdl::primedep::particles