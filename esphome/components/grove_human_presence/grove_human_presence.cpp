#include "esphome/core/log.h"
#include "grove_human_presence.h"

namespace esphome {
namespace grove_human_presence {

static const char *TAG = "grove_human_presence.sensor";

void GroveHumanPresenceSensor::setup() {}

float GroveHumanPresenceSensor::get_sensor_value() { return random_float() * 50; }

void GroveHumanPresenceSensor::update() { this->publish_state(get_sensor_value()); }

void GroveHumanPresenceSensor::dump_config() {}

}  // namespace grove_human_presence
}  // namespace esphome