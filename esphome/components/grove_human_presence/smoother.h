#pragma once

namespace esphome {
namespace grove_human_presence {

#define NUM_SMOOTHER 6

class Smoother {
 public:
  Smoother(float average_weight) : m_average_weight(average_weight), m_last_marked_value(0), m_average(0) {}

  void addDataPoint(float data) { m_average = m_average_weight * data + (1 - m_average_weight) * m_average; }

  float getSmoothValue() { return m_average; }

  float getDerivative() {
    float d = m_average - m_last_marked_value;
    m_last_marked_value = m_average;
    return d;
  }

 private:
  float m_average_weight;
  float m_last_marked_value;
  float m_average;
};

}  // namespace grove_human_presence
}  // namespace esphome