#include "esphome/core/log.h"
#include "grove_human_presence.h"
#include "smoother.h"  // Include the header for Smoother

namespace esphome {
namespace grove_human_presence {

static const char *TAG = "grove_human_presence.sensor";

void GroveHumanPresenceSensor::setup() {
  for (int i = 0; i < NUM_SMOOTHER; i++) {
    m_smoothers[i] = new Smoother(0.05);  // 0.3 very steep, 0.1 less steep, 0.05 less steep
  }

  sensitivity_presence = 6.0;
  sensitivity_movement = 10.0;

  memset(m_presences, 0, sizeof(m_presences));
  m_movement = MOVEMENT_NONE;
}

// void GroveHumanPresenceSensor::loop() {
//   float ir1, ir2, ir3, ir4, diff13, diff24;
//   uint32_t now = millis();

//   if (!dataReady()) {
//     return;
//   }

//   ir1 = getIR1();
//   ir2 = getIR2();
//   ir3 = getIR3();
//   ir4 = getIR4();

//   diff13 = ir1 - ir3;
//   diff24 = ir2 - ir4;

//   startNextSample();

//   m_smoothers[0]->addDataPoint(ir1);
//   m_smoothers[1]->addDataPoint(ir2);
//   m_smoothers[2]->addDataPoint(ir3);
//   m_smoothers[3]->addDataPoint(ir4);
//   m_smoothers[4]->addDataPoint(diff13);
//   m_smoothers[5]->addDataPoint(diff24);

//   if (now - m_last_time > (uint32_t) detect_interval) {
//     float d;
//     for (int i = 0; i < 4; i++) {
//       d = m_ders[i] = m_smoothers[i]->getDerivative();
//       // if (i == 0) Serial.println(d);
//       if (d > sensitivity_presence) {
//         m_presences[i] = true;
//       } else if (d < (-sensitivity_presence)) {
//         m_presences[i] = false;
//       }
//     }

//     d = m_der13 = m_smoothers[4]->getDerivative();
//     // Serial.println(d);
//     if (d > sensitivity_movement) {
//       m_movement &= 0b11111100;
//       m_movement |= MOVEMENT_FROM_3_TO_1;
//     } else if (d < (-sensitivity_movement)) {
//       m_movement &= 0b11111100;
//       m_movement |= MOVEMENT_FROM_1_TO_3;
//     }

//     d = m_der24 = m_smoothers[5]->getDerivative();
//     if (d > sensitivity_movement) {
//       m_movement &= 0b11110011;
//       m_movement |= MOVEMENT_FROM_4_TO_2;
//     } else if (d < (-sensitivity_movement)) {
//       m_movement &= 0b11110011;
//       m_movement |= MOVEMENT_FROM_2_TO_4;
//     }

//     m_last_time = now;
//   }
// }

uint8_t GroveHumanPresenceSensor::getMovement() {
  uint8_t r = m_movement;
  m_movement = MOVEMENT_NONE;
  return r;
}

void GroveHumanPresenceSensor::update() {
  float ir1, ir2, ir3, ir4, diff13, diff24;
  uint32_t now = millis();

  uint8_t data;
  read_register(REG_ST1, &data, 1);

  ESP_LOGD(TAG, "Read REG_ST1 data:  %d", data);

  // if (!dataReady()) {
  if ((data & 0x01) != 0x01) {
    ESP_LOGD(TAG, "data is not ready");
    return;
  }

  ESP_LOGI(TAG, "Read data");

  ir1 = getIR1();
  ir2 = getIR2();
  ir3 = getIR3();
  ir4 = getIR4();

  diff13 = ir1 - ir3;
  diff24 = ir2 - ir4;

  startNextSample();

  m_smoothers[0]->addDataPoint(ir1);
  m_smoothers[1]->addDataPoint(ir2);
  m_smoothers[2]->addDataPoint(ir3);
  m_smoothers[3]->addDataPoint(ir4);
  m_smoothers[4]->addDataPoint(diff13);
  m_smoothers[5]->addDataPoint(diff24);

  float d;
  for (int i = 0; i < 4; i++) {
    d = m_ders[i] = m_smoothers[i]->getDerivative();
    // if (i == 0) Serial.println(d);
    if (d > sensitivity_presence) {
      m_presences[i] = true;
    } else if (d < (-sensitivity_presence)) {
      m_presences[i] = false;
    }
  }

  d = m_der13 = m_smoothers[4]->getDerivative();
  // Serial.println(d);
  if (d > sensitivity_movement) {
    m_movement &= 0b11111100;
    m_movement |= MOVEMENT_FROM_3_TO_1;
  } else if (d < (-sensitivity_movement)) {
    m_movement &= 0b11111100;
    m_movement |= MOVEMENT_FROM_1_TO_3;
  }

  d = m_der24 = m_smoothers[5]->getDerivative();
  if (d > sensitivity_movement) {
    m_movement &= 0b11110011;
    m_movement |= MOVEMENT_FROM_4_TO_2;
  } else if (d < (-sensitivity_movement)) {
    m_movement &= 0b11110011;
    m_movement |= MOVEMENT_FROM_2_TO_4;
  }

  bool value = m_presences[0] || m_presences[1] || m_presences[2] || m_presences[3];

  ESP_LOGI(TAG, "Human present: %s", value);
  ESP_LOGI(TAG, "Moving: %d", getMovement());

  this->publish_state(value);
}

void GroveHumanPresenceSensor::set_sensitivity_presence(float value) {
  ESP_LOGD(TAG, "Set sensitivity_presence value: %f", value);
  sensitivity_presence = value;
}

void GroveHumanPresenceSensor::set_sensitivity_movement(float value) {
  ESP_LOGD(TAG, "Set sensitivity_movement value: %f", value);
  sensitivity_movement = value;
}

void GroveHumanPresenceSensor::dump_config() {}

}  // namespace grove_human_presence
}  // namespace esphome