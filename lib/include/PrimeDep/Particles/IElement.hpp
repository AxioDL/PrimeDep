#pragma once
#include "nlohmann/json_fwd.hpp"

namespace athena::io {
class IStreamWriter;
}

namespace axdl::primedep::particles {
class IElement {
public:
  virtual ~IElement() = default;
  virtual void PutTo(athena::io::IStreamWriter& out) = 0;
  virtual void PutTo(nlohmann::ordered_json& out) = 0;
};

}; // namespace axdl::primedep::particles