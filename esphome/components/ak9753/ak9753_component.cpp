#include "ak9753_component.h"

namespace esphome {
namespace ak9753 {

AK9753Component::~AK9753Component() {
  delete ak9753_;
  delete detector_;
}

void AK9753Component::setup() {
  ak9753_ = new AK9753(i2c_addr_);
  if (!ak9753_->initialize()) {
    ESP_LOGE("ak9753", "Failed to initialize AK9753 sensor");
    return;
  }

  detector_ = new PresenceDetector(*ak9753_, 50.0, 100.0, 100); // Параметри детектора
  ESP_LOGI("ak9753", "AK9753 sensor initialized successfully");
}

void AK9753Component::update() {
  if (ak9753_ == nullptr || !ak9753_->dataReady()) {
    ESP_LOGW("ak9753", "Sensor not ready or not initialized");
    return;
  }

  detector_->loop();

  if (presence_sensor_ != nullptr) {
    bool presence = detector_->presentFullField(false);
    ESP_LOGD("ak9753", "Presence detected: %s", presence ? "true" : "false");
    presence_sensor_->publish_state(presence);
  }

  for (int i = 0; i < 4; i++) {
    if (ir_sensors_[i] != nullptr) {
      float value = 0;
      switch (i) {
        case 0: value = ak9753_->getIR1(); break;
        case 1: value = ak9753_->getIR2(); break;
        case 2: value = ak9753_->getIR3(); break;
        case 3: value = ak9753_->getIR4(); break;
      }
      ESP_LOGD("ak9753", "IR Sensor %d value: %.2f", i, value);
      ir_sensors_[i]->publish_state(value);
    }
  }

  if (temperature_sensor_ != nullptr) {
    float temp = ak9753_->getTMP();
    ESP_LOGD("ak9753", "Temperature: %.2f", temp);
    temperature_sensor_->publish_state(temp);
  }

  ak9753_->startNextSample();
}

}  // namespace ak9753
}  // namespace esphome
