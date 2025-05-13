#include "esphome/core/log.h"
#include "empty_i2c_sensor.h"

namespace esphome {
namespace grove_human_presence2 {

static const char *TAG = "grove_human_presence2.sensor";

void EmptyI2CSensor::setup() {}

void EmptyI2CSensor::update() {}

void EmptyI2CSensor::dump_config() {}

}  // namespace grove_human_presence2
}  // namespace esphome