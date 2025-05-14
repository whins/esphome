
#include "ak975x.h"

namespace esphome {
namespace grove_human_presence {

bool AK975X::initialize(void) {
  delay(3);

  softReset();

  // check chip id
  if (getDeviceID() != 0x13) {
    return false;
  }

  // set mode and filter freq
  setECNTL1((AK975X_FREQ_8_8HZ << 3) | AK975X_MODE_0);

  // enable interrupt
  setEINTEN(0x1f);  // enable all interrupts

  return true;
}

/**
    getCompanyCode
    the code is expected to be 0x48
*/
uint8_t AK975X::getCompanyCode(void) {
  uint8_t data;
  readRegs(REG_WIA1, &data, 1);
  return (data);
}

/**
    getDeviceID
    the ID is expected to be 0x13
*/
uint8_t AK975X::getDeviceID(void) {
  uint8_t data;
  readRegs(REG_WIA2, &data, 1);
  return (data);
}

bool AK975X::dataReady(void) { /* returns ST1[0], read ST2 to clear */
  uint8_t data;
  readRegs(REG_ST1, &data, 1);
  return ((data & 0x01) == 0x01);
}

bool AK975X::dataOverRun(void) {
  uint8_t data;
  readRegs(REG_ST2, &data, 1);
  return ((data & 0x02) == 0x02);
}

uint8_t AK975X::getINTST(void) { /** return REG_INTST */
  uint8_t data;
  readRegs(REG_INTST, &data, 1);
  return (data);
}

uint8_t AK975X::getST1(void) {
  uint8_t data;
  readRegs(REG_ST1, &data, 1);
  return (data);
}

int16_t AK975X::getRawIR1(void) {
  uint8_t data[2];
  int16_t IR;
  readRegs(REG_IR1L, data, 2);
  IR = (data[1] << 8) | data[0];
  return (IR);
}

float AK975X::getIR1(void) {
  int16_t iValue;
  float fValue;
  iValue = getRawIR1();
  fValue = 14286.8 * iValue / 32768.0;
  return (fValue);
}

int16_t AK975X::getRawIR2(void) {
  uint8_t data[2];
  int16_t IR;
  readRegs(REG_IR2L, data, 2);
  IR = (data[1] << 8) | data[0];
  return (IR);
}

float AK975X::getIR2(void) {
  int16_t iValue;
  float fValue;
  iValue = getRawIR2();
  fValue = 14286.8 * iValue / 32768.0;
  return (fValue);
}

int16_t AK975X::getRawIR3(void) {
  uint8_t data[2];
  int16_t IR;
  readRegs(REG_IR3L, data, 2);
  IR = (data[1] << 8) | data[0];
  return (IR);
}

float AK975X::getIR3(void) {
  int16_t iValue;
  float fValue;
  iValue = getRawIR3();
  fValue = 14286.8 * iValue / 32768.0;
  return (fValue);
}

int16_t AK975X::getRawIR4(void) {
  uint8_t data[2];
  int16_t IR;
  readRegs(REG_IR4L, data, 2);
  IR = (data[1] << 8) | data[0];
  return (IR);
}

float AK975X::getIR4(void) {
  int16_t iValue;
  float fValue;
  iValue = getRawIR4();
  fValue = 14286.8 * iValue / 32768.0;
  return (fValue);
}

int16_t AK975X::getRawTMP(void) {
  uint8_t data[2];
  int16_t temp;
  readRegs(REG_TMPL, data, 2);
  temp = (data[1] << 8) | data[0];
  return (temp);
}

float AK975X::getTMP(void) {
  int16_t iValue;
  float temperature;

  iValue = getRawTMP();
  iValue >>= 6;  // Temp is 10-bit. TMPL0:5 fixed at 0

  temperature = 26.75 + (iValue * 0.125);

  return (temperature);
}

float AK975X::getTMP_F(void) {
  float temperature = getTMP();
  temperature = temperature * 1.8 + 32.0;
  return (temperature);
}

uint8_t AK975X::getST2(void) {
  uint8_t data;
  readRegs(REG_ST2, &data, 1);
  return (data);
}

int16_t AK975X::getETH13H(void) {
  int16_t value;
  uint8_t data[2];
  readRegs(REG_ETH13H_LSB, data, 2);
  value = (data[1] << 8) | data[0];
  return (value);
}

int16_t AK975X::getETH13L(void) {
  int16_t value;
  uint8_t data[2];
  readRegs(REG_ETH13L_LSB, data, 2);
  value = (data[1] << 8) | data[0];
  return (value);
}

int16_t AK975X::getETH24H(void) {
  int16_t value;
  uint8_t data[2];
  readRegs(REG_ETH24H_LSB, data, 2);
  value = (data[1] << 8) | data[0];
  return (value);
}

int16_t AK975X::getETH24L(void) {
  int16_t value;
  uint8_t data[2];
  readRegs(REG_ETH24L_LSB, data, 2);
  value = (data[1] << 8) | data[0];
  return (value);
}

uint8_t AK975X::getEHYS13(void) {
  uint8_t data;
  readRegs(REG_EHYS13, &data, 1);
  return (data);
}

uint8_t AK975X::getEHYS24(void) {
  uint8_t data;
  readRegs(REG_EHYS24, &data, 1);
  return (data);
}

uint8_t AK975X::getEINTEN(void) {
  uint8_t data;
  readRegs(REG_EINTEN, &data, 1);
  return (data);
}

uint8_t AK975X::getECNTL1(void) {
  uint8_t data;
  readRegs(REG_ECNTL1, &data, 1);
  return (data);
}

uint8_t AK975X::getCNTL2(void) {
  uint8_t data;
  readRegs(REG_CNTL2, &data, 1);
  return (data);
}

int16_t AK975X::ETHpAtoRaw(float pA) {
  int16_t raw = (int16_t) (pA / 3.4877);
  if (raw > 2047) {
    raw = 2047;
  }
  if (raw < -2048) {
    raw = -2048;
  }
  return raw;
}

void AK975X::setETH13H(int16_t value) {
  uint8_t data[3];
  data[0] = REG_ETH13H_LSB;
  value <<= 3;
  data[1] = value & 0xFF;
  data[2] = (value >> 8) & 0xFF;
  write(data, 3);
}

void AK975X::setETH13L(int16_t value) {
  uint8_t data[3];
  data[0] = REG_ETH13L_LSB;
  value <<= 3;
  data[1] = value & 0xFF;
  data[2] = (value >> 8) & 0xFF;
  write(data, 3);
}

void AK975X::setETH24H(int16_t value) {
  uint8_t data[3];
  data[0] = REG_ETH24H_LSB;
  value <<= 3;
  data[1] = value & 0xFF;
  data[2] = (value >> 8) & 0xFF;
  write(data, 3);
}

void AK975X::setETH24L(int16_t value) {
  uint8_t data[3];
  data[0] = REG_ETH24L_LSB;
  value <<= 3;
  data[1] = value & 0xFF;
  data[2] = (value >> 8) & 0xFF;
  write(data, 3);
}

uint8_t AK975X::EHYSpAtoRaw(float pA) {
  uint16_t raw = (uint16_t) (pA / 3.4877);
  if (raw > 31) {
    raw = 31;
  }
  return (uint8_t) raw;
}

void AK975X::setEHYS13(uint8_t value) {
  uint8_t data[2];
  data[0] = REG_EHYS13;
  data[1] = value;
  write(data, 2);
}

void AK975X::setEHYS24(uint8_t value) {
  uint8_t data[2];
  data[0] = REG_EHYS24;
  data[1] = value;
  write(data, 2);
}

void AK975X::setEINTEN(uint8_t value) {
  uint8_t data[2];
  data[0] = REG_EINTEN;
  data[1] = value;
  write(data, 2);
}

void AK975X::setECNTL1(uint8_t value) {
  uint8_t data[2];
  data[0] = REG_ECNTL1;
  data[1] = value;
  write(data, 2);
}

void AK975X::softReset(void) {
  uint8_t data[2] = {REG_CNTL2, 0xFF};
  write(data, 2);
}

void AK975X::startNextSample(void) { getST2(); }

int AK975X::readRegs(int addr, uint8_t *data, int len) {
  // Починаємо читання з регістра `reg` довжиною len байт

  uint8_t *p = (uint8_t *) &addr;

  if (!this->write(p, 1)) {
    ESP_LOGW(TAG, "Start registry reading failed!");
    return 1;
  }

  if (!this->read(data, len)) {
    // Помилка читання
    ESP_LOGW(TAG, "Reading registry data failed!");
    return 2;
  }

  return 0;  // OK
}

int AK975X::writeRegs(uint8_t *data, int len) {
  // Записуємо дані в регістр
  if (!this->write(data, len)) {
    // Помилка запису
    return 1;
  }

  return 0;  // OK
}

}  // namespace grove_human_presence
}  // namespace esphome