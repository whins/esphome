#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"
#include "ak975x.h"
#include "smoother.h"

namespace esphome {
namespace grove_human_presence {

class GroveHumanPresenceSensor : public sensor::Sensor, public PollingComponent, public AK975X {
 protected:
 private:
  Smoother *m_smoothers[NUM_SMOOTHER];
  int detect_interval;
  uint32_t m_last_time;

  bool m_presences[4];
  uint8_t m_movement;

  float sensitivity_presence, sensitivity_movement;
  float m_ders[4];
  float m_der13, m_der24;

  /**
      Read the movement flags, clear after read
      @return - one/OR of the MOVEMENT_FROM_X_TO_X macro
  */
  uint8_t getMovement();

  void set_sensitivity_presence(float value);
  void set_sensitivity_movement(float value);
  void set_detect_interval(int value);

 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  // void loop() override;
};

}  // namespace grove_human_presence
}  // namespace esphome