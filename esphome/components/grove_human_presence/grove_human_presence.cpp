#include "esphome/core/log.h"
#include "grove_human_presence.h"
#include "smoother.h"  // Include the header for Smoother

namespace esphome {
namespace grove_human_presence {

#define MAX_RETRIES 3
#define TIMEOUT_MS 1000

void GroveHumanPresenceComponent::setup() {
  sensitivity_presence = 1.0;  // 6.0
  sensitivity_movement = 2.0;  // 10.0
  detect_interval = 30;
  m_last_time = millis();
  m_movement = MOVEMENT_NONE;

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

void GroveHumanPresenceComponent::loop() {
  if (disabled || !continous_reading || this->status_has_error()) {
    return;
  }

  uint32_t now = millis();

  if (!dataReady()) {
    ESP_LOGW(TAG, "Data not ready");
    this->status_set_warning();
    return;
  }

  read_sensors();

  if (now - m_last_time > (uint32_t) detect_interval) {
    calc_values();
    m_last_time = now;
  }
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

    if (d > sensitivity_presence) {
      m_presences[i] = true;
    } else if (d < (-sensitivity_presence)) {
      m_presences[i] = false;
    }
  }

  d = m_der13 = m_smoothers[4].getDerivative();

  if (d > sensitivity_movement) {
    m_movement &= 0b11111100;
    m_movement |= MOVEMENT_FROM_3_TO_1;
  } else if (d < (-sensitivity_movement)) {
    m_movement &= 0b11111100;
    m_movement |= MOVEMENT_FROM_1_TO_3;
  }

  d = m_der24 = m_smoothers[5].getDerivative();

  if (d > sensitivity_movement) {
    m_movement &= 0b11110011;
    m_movement |= MOVEMENT_FROM_4_TO_2;
  } else if (d < (-sensitivity_movement)) {
    m_movement &= 0b11110011;
    m_movement |= MOVEMENT_FROM_2_TO_4;
  }
}

void GroveHumanPresenceComponent::update() {
  if (disabled || this->status_has_error()) {
    return;
  }

  if (!continous_reading) {
    if (!dataReady()) {
      ESP_LOGW(TAG, "Data not ready");
      this->status_set_warning();
      return;
    }

    read_sensors();
    calc_values();
  }

  bool occupancy = m_presences[0] || m_presences[1] || m_presences[2] || m_presences[3];

  for (int i = 0; i < 4; i++) {
    m_presences[i] = false;
  }

  if (occupancy_binary_sensor != nullptr) {
    ESP_LOGD(TAG, "Presence: %d ", occupancy ? 1 : 0);
    occupancy_binary_sensor->publish_state(occupancy);
  }

  if (motion_sensor != nullptr) {
    uint8_t moving = getMovement();
    ESP_LOGD(TAG, "Moving: %d ", moving);
    motion_sensor->publish_state(moving);
  }

  if (temperature_sensor != nullptr) {
    float temp = getTemperature();
    ESP_LOGD(TAG, "Temperature: %.2f", temp);
    temperature_sensor->publish_state(temp);
  }
}

void GroveHumanPresenceComponent::set_sensitivity_presence(float value) {
  ESP_LOGD(TAG, "Set sensitivity_presence value: %f", value);
  sensitivity_presence = value;
}

void GroveHumanPresenceComponent::set_sensitivity_movement(float value) {
  ESP_LOGD(TAG, "Set sensitivity_movement value: %f", value);
  sensitivity_movement = value;
}

void GroveHumanPresenceComponent::set_detect_interval(int value) {
  ESP_LOGD(TAG, "Set detect_interval value: %f", value);
  detect_interval = value;
}

void GroveHumanPresenceComponent::set_continous_reading(bool value) {
  ESP_LOGD(TAG, "Set continous_reading value: %s", value ? "true" : "false");
  continous_reading = value;
}

void GroveHumanPresenceComponent::dump_config() {}

}  // namespace grove_human_presence
}  // namespace esphome