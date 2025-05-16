#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/preferences.h"
#include "ak975x.h"
#include "smoother.h"

namespace esphome {
namespace grove_human_presence {

class GroveHumanPresenceComponent : public PollingComponent, public AK975X {
 private:
  Smoother m_smoothers[NUM_SMOOTHER] = {Smoother(0.05), Smoother(0.05), Smoother(0.05),
                                        Smoother(0.05), Smoother(0.05), Smoother(0.05)};

  bool m_presences[4] = {false, false, false, false};
  uint8_t m_movement;

  float sensitivity_presence, sensitivity_movement;
  int detect_interval;
  uint32_t m_last_time;
  float m_ders[4];
  float m_der13, m_der24;
  bool continous_reading = false;
  bool disabled = false;

  /**
   Read the movement flags, clear after read
   @return - one/OR of the MOVEMENT_FROM_X_TO_X macro
   */
  uint8_t getMovement();

  void read_sensors();
  void calc_values();

 protected:
  binary_sensor::BinarySensor *occupancy_binary_sensor{nullptr};
  sensor::Sensor *motion_sensor{nullptr};
  sensor::Sensor *temperature_sensor{nullptr};

 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  void loop() override;

  void set_occupancy_binary_sensor(binary_sensor::BinarySensor *bs) { occupancy_binary_sensor = bs; }
  void set_motion_sensor(sensor::Sensor *s) { motion_sensor = s; }
  void set_temperature_sensor(sensor::Sensor *s) { temperature_sensor = s; }

  /**
   * @brief Set Sensitivity Presence
   *
   * @param value - compares with the derivative of the readings of a specific IR sensor (1/2/3/4)
   */
  void set_sensitivity_presence(float value);

  /**
   * @brief Set Sensitivity Movement
   *
   * @param value - compares with the derivative of the difference value between IR sensor 1-3 or 2-4
   */
  void set_sensitivity_movement(float value);

  /**
   * @brief Set interval of the presence detection, unit: millisecond
   *
   * @param value - the interval of the presence detection, unit: millisecond
   */
  void set_detect_interval(int value);

  /**
   * @brief Set the continous reading object
   *
   * @param value
   */
  void set_continous_reading(bool value);

  /**
   * @brief Set the disabled object
   *
   * This is used to disable the sensor in the configuration
   *
   * @param value
   */
  void set_disabled(bool value) { disabled = value; }
};

}  // namespace grove_human_presence
}  // namespace esphome