#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace grove_human_presence {

class GroveHumanPresenceSensor : public sensor::Sensor, public PollingComponent, public i2c::I2CDevice {
 protected:
  float get_sensor_value();

 public:
  void setup() override;
  void update() override;
  void dump_config() override;
};

}  // namespace grove_human_presence
}  // namespace esphome