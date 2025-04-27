#pragma once
#include "PrimeDep/Math/Transform4f.hpp"
#include "PrimeDep/WorldCommon/MetroidPrime/EditorId.hpp"
#include "nlohmann/json_fwd.hpp"

namespace athena::io {
class IStreamReader;
}

namespace axdl::primedep::MetroidPrime {
class MappableObject {
public:
  enum class EMappableObjectType : uint32_t {
    BlueDoor = 0,
    ShieldDoor = 1,
    IceDoor = 2,
    WaveDoor = 3,
    PlasmaDoor = 4,
    BigDoor1 = 5,
    BigDoor2 = 6,
    IceDoorCeiling = 7,
    IceDoorFloor = 8,
    WaveDoorCeiling = 9,
    WaveDoorFloor = 10,
    IceDoorFloor2 = 13,
    WaveDoorFloor2 = 14,
    SaveStationMP2 = 17,
    DownArrowYellow = 27, /* Maintenance Tunnel */
    UpArrowYellow = 28,   /* Phazon Processing Center */
    DownArrowGreen = 29,  /* Elevator A */
    UpArrowGreen = 30,    /* Elite Control Access */
    DownArrowRed = 31,    /* Elevator B */
    UpArrowRed = 32,      /* Fungal Hall Access */
    TransportLift = 33,
    SaveStation = 34,
    MissileStation = 37
  };
  enum class EVisMode {
    Always,
    MapStationOrVisit,
    Visit,
    Never,
    MapStationOrVisit2,
  };

  explicit MappableObject(athena::io::IStreamReader& in);


  void PutTo(nlohmann::ordered_json& j) const;

private:
  EMappableObjectType m_type;
  EVisMode m_visMode;
  EditorId m_editorId;
  uint32_t m_unused;
  Transform4f m_transform;
};
} // namespace axdl::primedep::MetroidPrime