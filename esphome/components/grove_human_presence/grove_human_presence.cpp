
#include "grove_human_presence.h"

namespace esphome {
namespace grove_human_presence {

void GroveHumanPresenceComponent::setup() {
  occupancy_sensitivity = 3.0;  // 6.0
  motion_sensitivity = 6.0;
  m_movement = MOVEMENT_NONE;
  last_read_time = millis();

  if (this->initialize()) {
    ESP_LOGE(TAG, "Failed to initialize Grove Human Presence Sensor.");
    this->status_set_error();
  }
}

uint8_t GroveHumanPresenceComponent::getMovement() {
  uint8_t r = m_movement;
  m_movement = MOVEMENT_NONE;
  return r;
}

void GroveHumanPresenceComponent::read_sensors() {
  float ir1 = getIR1(), ir2 = getIR2(), ir3 = getIR3(), ir4 = getIR4();
  float diff13 = ir1 - ir3;
  float diff24 = ir2 - ir4;

  startNextSample();

  m_smoothers[0].addDataPoint(ir1);
  m_smoothers[1].addDataPoint(ir2);
  m_smoothers[2].addDataPoint(ir3);
  m_smoothers[3].addDataPoint(ir4);
  m_smoothers[4].addDataPoint(diff13);
  m_smoothers[5].addDataPoint(diff24);
}

void GroveHumanPresenceComponent::calc_values() {
  float d;

  for (int i = 0; i < 4; i++) {
    d = m_ders[i] = m_smoothers[i].getDerivative();

    if (d > occupancy_sensitivity) {
      m_presences[i] = true;
    } else if (d < (-occupancy_sensitivity)) {
      m_presences[i] = false;
    }
  }

  d = m_der13 = m_smoothers[4].getDerivative();

  if (d > motion_sensitivity) {
    m_movement &= 0b11111100;
    m_movement |= MOVEMENT_FROM_3_TO_1;
  } else if (d < (-motion_sensitivity)) {
    m_movement &= 0b11111100;
    m_movement |= MOVEMENT_FROM_1_TO_3;
  }

  d = m_der24 = m_smoothers[5].getDerivative();

  if (d > motion_sensitivity) {
    m_movement &= 0b11110011;
    m_movement |= MOVEMENT_FROM_4_TO_2;
  } else if (d < (-motion_sensitivity)) {
    m_movement &= 0b11110011;
    m_movement |= MOVEMENT_FROM_2_TO_4;
  }
}

void GroveHumanPresenceComponent::loop() {
  uint32_t now = millis();
  if (now - last_read_time < 30) {
    return;
  }

  if (!dataReady()) {
    ESP_LOGW(TAG, "Data not ready");
    this->status_set_warning();
    return;
  }

  read_sensors();

  last_read_time = now;
}

void GroveHumanPresenceComponent::update() {
  calc_values();

#ifdef USE_BINARY_SENSOR
  bool occupancy = m_presences[0] || m_presences[1] || m_presences[2] || m_presences[3];

  if (occupancy_binary_sensor_ != nullptr) {
    ESP_LOGD(TAG, "Presence: %d ", occupancy ? 1 : 0);
    occupancy_binary_sensor_->publish_state(occupancy);
  }
#endif

#ifdef USE_SENSOR
  if (motion_sensor_ != nullptr) {
    uint8_t moving = getMovement();
    ESP_LOGD(TAG, "Moving: %d ", moving);
    motion_sensor_->publish_state(moving);
  }

  if (temperature_sensor_ != nullptr) {
    float temp = getTemperature();
    ESP_LOGD(TAG, "Temperature: %.2f", temp);
    temperature_sensor_->publish_state(temp);
  }
#endif

  // for (int i = 0; i < 4; i++) {
  //   m_presences[i] = false;
  // }
}

void GroveHumanPresenceComponent::set_occupancy_sensitivity(float value) {
  ESP_LOGD(TAG, "Set sensitivity_presence value: %f", value);
  occupancy_sensitivity = value;
}

void GroveHumanPresenceComponent::set_motion_sensitivity(float value) {
  ESP_LOGD(TAG, "Set sensitivity_movement value: %f", value);
  motion_sensitivity = value;
}

void GroveHumanPresenceComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "GroveHumanPresence:");
#ifdef USE_BINARY_SENSOR
  LOG_BINARY_SENSOR("  ", "OccupancyBinarySensor", this->occupancy_binary_sensor_);
#endif
#ifdef USE_SENSOR
  LOG_SENSOR("  ", "MotionSensor", this->motion_sensor_);
  LOG_SENSOR("  ", "TemperatureSensor", this->temperature_sensor_);
#endif

  ESP_LOGCONFIG(TAG, "  Occupancy sensitivity: %.2f", occupancy_sensitivity);
  ESP_LOGCONFIG(TAG, "  Motion sensitivity: %.2f", motion_sensitivity);
}

}  // namespace grove_human_presence
}  // namespace esphome