#ifndef _BLUEFRUIT_STATE_H_
#define _BLUEFRUIT_STATE_H_

class BluefruitState {
private:
  enum Data { NONE, BUTTONS, ACCELEROMETER, GYRO };
  uint8_t m_isPressed;
  bool m_isDirty;
  float m_accelX;
  float m_accelY;
  float m_accelZ;
  float m_gyroX;
  float m_gyroY;
  float m_gyroZ;

  Data m_data;
  uint8_t m_replyByte;
  uint8_t m_replyIndex;
  uint8_t m_checksum;
  uint8_t m_buf[12];

  void resetParsing();
  void finalizeParsing();

public:
  bool isDirty();
  bool isButtonPressed(uint8_t num);
  uint8_t getButtons();
  uint8_t read(Adafruit_BluefruitLE_SPI &ble);
  void printControlPad();
  void printAccel();
  void printGyro();
  float getAccelX();
  float getAccelY();
  float getAccelZ();
  float getGyroX();
  float getGyroY();
  float getGyroZ();
};

#endif
