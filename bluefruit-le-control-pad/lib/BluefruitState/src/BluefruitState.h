#ifndef _BLUEFRUIT_STATE_H_
#define _BLUEFRUIT_STATE_H_

class BluefruitState {
public:
  enum Data { NONE, BUTTONS, ACCELEROMETER, GYRO, QUATERNIONS, JOY };

private:
  uint8_t m_isPressed;
  uint8_t m_isDirty;
  float m_accelX;
  float m_accelY;
  float m_accelZ;
  float m_gyroX;
  float m_gyroY;
  float m_gyroZ;
  float m_quatX;
  float m_quatY;
  float m_quatZ;
  float m_quatW;
  float m_aJoyX;
  float m_aJoyY;
  float m_bJoyX;
  float m_bJoyY;

  Data m_data;
  uint8_t m_replyByte;
  uint8_t m_replyIndex;
  uint8_t m_checksum;
  uint8_t m_buf[16];

  void resetParsing();
  void finalizeParsing();

public:
  bool isDirty(Data data);
  bool isButtonPressed(uint8_t num);
  uint8_t getButtons();
  uint8_t read(Adafruit_BluefruitLE_SPI &ble);
  void printButtons();
  void printAccel();
  void printGyro();
  void printQuat();
  void printJoy();
  float getAccelX();
  float getAccelY();
  float getAccelZ();
  float getGyroX();
  float getGyroY();
  float getGyroZ();
  float getQuatX();
  float getQuatY();
  float getQuatZ();
  float getQuatW();
  float getAJoyX();
  float getAJoyY();
  float getBJoyX();
  float getBJoyY();
};

#endif
