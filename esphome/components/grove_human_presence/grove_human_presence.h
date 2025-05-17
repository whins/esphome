#pragma once

#include "esphome/core/defines.h"
#include "esphome/core/component.h"
#ifdef USE_BINARY_SENSOR
#include "esphome/components/binary_sensor/binary_sensor.h"
#endif
#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/helpers.h"

#include "ak975x.h"
#include "smoother.h"

namespace esphome {
namespace grove_human_presence {

static const char *TAG = "grove_human_presence";
#define NUM_SMOOTHER 6

class GroveHumanPresenceComponent : public PollingComponent, public AK975X {
 private:
#ifdef USE_SENSOR
  SUB_SENSOR(motion)
  SUB_SENSOR(temperature)
#endif
#ifdef USE_BINARY_SENSOR
  SUB_BINARY_SENSOR(occupancy)
#endif

 protected:
  Smoother m_smoothers[NUM_SMOOTHER] = {Smoother(0.05), Smoother(0.05), Smoother(0.05),
                                        Smoother(0.05), Smoother(0.05), Smoother(0.05)};

  bool m_presences[4] = {false, false, false, false};
  uint8_t m_movement;

  float occupancy_sensitivity;
  float motion_sensitivity;

  float m_ders[4];
  float m_der13, m_der24;

  uint8_t read_data_failure_count = 0;

  /**
   Read the movement flags, clear after read
   @return - one/OR of the MOVEMENT_FROM_X_TO_X macro
   */
  uint8_t getMovement();

 public:
  void setup() override;
  void update() override;
  void dump_config() override;

  void set_occupancy_sensitivity(float value);
  void set_motion_sensitivity(float value);
};

}  // namespace grove_human_presence
}  // namespace esphome