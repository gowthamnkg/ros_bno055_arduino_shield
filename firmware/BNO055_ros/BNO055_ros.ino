#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <ros.h>
#include <std_msgs/String.h>

#include <std_msgs/Float32.h>

#define BNO055_SAMPLERATE_DELAY_MS 100
Adafruit_BNO055 bno = Adafruit_BNO055(55);
ros::NodeHandle  nh;

std_msgs::Float32 orientation;

std_msgs::String imuMsg;
ros::Publisher imuPublisher("arduino/raw_imu", &imuMsg);

ros::Publisher orientationPublisher("orientation", &orientation);

void setup()
{
  if (!bno.begin())
  {
    while (1);
  }
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.advertise(imuPublisher);

  nh.advertise(orientationPublisher);
  
  delay(1000);
}

void loop()
{
  String buffer_;
  sensors_event_t angVelocityData , linearAccelData, orientationData;
  bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);

  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  
  imu::Quaternion quat = bno.getQuat();

  buffer_ += "qW:";
  buffer_ += String(quat.w(),6);
  buffer_ += "qX:";
  buffer_ += String(quat.x(),6);
  buffer_ += "qY:";
  buffer_ += String(quat.y(),6);
  buffer_ += "qZ:";
  buffer_ += String(quat.z(),6);

  buffer_ += "avX:";
  buffer_ += String(angVelocityData.gyro.x, 4);
  buffer_ += "avY:";
  buffer_ += String(angVelocityData.gyro.y, 4);
  buffer_ += "avZ:";
  buffer_ += String(angVelocityData.gyro.z, 4);

  buffer_ += "laX:";
  buffer_ += String(linearAccelData.acceleration.x, 4);
  buffer_ += "laY:";
  buffer_ += String(linearAccelData.acceleration.y, 4);
  buffer_ += "laZ:";
  buffer_ += String(linearAccelData.acceleration.z, 4);

  imuMsg.data = buffer_.c_str();

  orientation.data = orientationData.orientation.x * M_PI / 180;
  
  imuPublisher.publish( &imuMsg );

  orientationPublisher.publish( &orientation );
  
  delay(BNO055_SAMPLERATE_DELAY_MS);
  nh.spinOnce();
}
