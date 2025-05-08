#include "esphome/core/log.h"
#include "grove_human_presence.h"

namespace esphome {
namespace grove_human_presence {

static const char *TAG = "grove_human_presence.sensor";

void GroveHumanPresenceSensor::setup() {}

float GroveHumanPresenceSensor::get_sensor_value() {
  float value = random_float() * 50;  // Simulate a sensor value between 0 and 50
  ESP_LOGI(TAG, "Reading Grove presence sensor: %f", value);

  return value;
}

void GroveHumanPresenceSensor::update() { this->publish_state(get_sensor_value()); }

void GroveHumanPresenceSensor::dump_config() {}

}  // namespace grove_human_presence
}  // namespace esphome