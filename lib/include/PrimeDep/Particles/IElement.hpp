#pragma once

namespace athena::io {
class IStreamWriter;
}

namespace axdl::primedep::particles {
class IElement {
  virtual ~IElement() = default;
  virtual void PutTo(athena::io::IStreamWriter& out) = 0;
};
}; // namespace axdl::primedep::particles