
#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "Grove_Human_Presence_Sensor.h"

namespace esphome {
namespace ak9753 {

class AK9753Component : public PollingComponent {
 public:
  void set_i2c_address(uint8_t address) { i2c_addr_ = address; }

  void set_presence_binary_sensor(binary_sensor::BinarySensor *bs) { presence_sensor_ = bs; }
  void set_ir_sensor(sensor::Sensor *s, int index) { ir_sensors_[index] = s; }
  void set_temperature_sensor(sensor::Sensor *s) { temperature_sensor_ = s; }

  void setup() override;
  void update() override;

 protected:
  uint8_t i2c_addr_;
  AK9753 *ak9753_;
  PresenceDetector *detector_;

  binary_sensor::BinarySensor *presence_sensor_{nullptr};
  sensor::Sensor *ir_sensors_[4]{};
  sensor::Sensor *temperature_sensor_{nullptr};

  uint32_t last_update_{0};
};

}  // namespace ak9753
}  // namespace esphome
