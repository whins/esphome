#include "esphome/core/log.h"
#include "grove_human_presence.h"
#include "smoother.h"  // Include the header for Smoother

namespace esphome {
namespace grove_human_presence {

#define MAX_RETRIES 3
#define TIMEOUT_MS 1000

void GroveHumanPresenceSensor::setup() {
  // ESP_LOGI(TAG, "Free heap: %d", ESP.getFreeHeap());

  sensitivity_presence = 6.0;
  sensitivity_movement = 10.0;
  detect_interval = 30;
  m_last_time = millis();
  m_movement = MOVEMENT_NONE;

  if (this->initialize()) {
    ESP_LOGI(TAG, "Grove Human Presence Sensor initialized successfully.");
  } else {
    ESP_LOGE(TAG, "Failed to initialize Grove Human Presence Sensor.");
  }

  // ESP_LOGI(TAG, "Free heap: %d", ESP.getFreeHeap());
}

uint8_t GroveHumanPresenceSensor::getMovement() {
  uint8_t r = m_movement;
  m_movement = MOVEMENT_NONE;
  return r;
}

void GroveHumanPresenceSensor::update() {
  ESP_LOGI(TAG, "Updating sensors data...");

  if (!dataReady()) {
    ESP_LOGE(TAG, "Data not ready");
    this->status_set_error();
    return;
  }

  float ir1 = getIR1(), ir2 = getIR2(), ir3 = getIR3(), ir4 = getIR4();
  float diff13 = ir1 - ir3;
  float diff24 = ir2 - ir4;

  ESP_LOGI(TAG, "Read IR data completed");
  ESP_LOGI(TAG, "Start next sample");
  startNextSample();
  ESP_LOGI(TAG, "Start next sample completed");

  // ESP_LOGI(TAG, "Add data points to smoothers");
  // // ESP_LOGI(TAG, "Free heap before adding data points: %d", ESP.getFreeHeap());

  m_smoothers[0].addDataPoint(ir1);
  m_smoothers[1].addDataPoint(ir2);
  m_smoothers[2].addDataPoint(ir3);
  m_smoothers[3].addDataPoint(ir4);
  m_smoothers[4].addDataPoint(diff13);
  m_smoothers[5].addDataPoint(diff24);

  ESP_LOGI(TAG, "Add data points to smoothers complete");

  ESP_LOGI(TAG, "Fill presences");

  float d;
  for (int i = 0; i < 4; i++) {
    d = m_ders[i] = m_smoothers[i].getDerivative();

    if (d > sensitivity_presence) {
      m_presences[i] = true;
    } else if (d < (-sensitivity_presence)) {
      m_presences[i] = false;
    }
  }

  ESP_LOGI(TAG, "Fill presences completed");

  ESP_LOGI(TAG, "Check movement");

  d = m_der13 = m_smoothers[4].getDerivative();
  // Serial.println(d);
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

  ESP_LOGI(TAG, "Check movement completed");

  bool value = m_presences[0] || m_presences[1] || m_presences[2] || m_presences[3];

  ESP_LOGI(TAG, "Human present: %s", value ? "true" : "false");

  ESP_LOGI(TAG, "Moving: %d", getMovement());

  this->publish_state(value ? 1.0 : 0);
}

void GroveHumanPresenceSensor::set_sensitivity_presence(float value) {
  ESP_LOGD(TAG, "Set sensitivity_presence value: %f", value);
  sensitivity_presence = value;
}

void GroveHumanPresenceSensor::set_sensitivity_movement(float value) {
  ESP_LOGD(TAG, "Set sensitivity_movement value: %f", value);
  sensitivity_movement = value;
}
void GroveHumanPresenceSensor::set_detect_interval(int value) {
  ESP_LOGD(TAG, "Set detect_interval value: %f", value);
  detect_interval = value;
}

void GroveHumanPresenceSensor::dump_config() {}

}  // namespace grove_human_presence
}  // namespace esphome