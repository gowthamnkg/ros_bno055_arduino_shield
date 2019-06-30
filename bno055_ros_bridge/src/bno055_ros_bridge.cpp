#include "bno055_ros_bridge.hpp"

RosBno055::RosBno055(ros::NodeHandle& imu_nh_):imu_nh(imu_nh_)
{
    frame_id_ = "imu_bno055";
    imuData.header.frame_id = frame_id_;
    imuData.header.seq = 0;
    arduino_subscriber = imu_nh.subscribe("arduino/raw_imu", 10, &RosBno055::imuArduinoCb, this);
    imu_publisher = imu_nh.advertise<sensor_msgs::Imu>("sensors/imu", 1000);
}

RosBno055::~RosBno055()
{

}

void RosBno055::imuArduinoCb(const std_msgs::String& str)
{
    imuData.header.stamp = ros::Time::now();
    // parsing start
    imuData.orientation.w = ::atof((str.data.substr(str.data.find("qW:")+3, str.data.find("qX:")-(str.data.find("qW:")+3)).c_str()));
    imuData.orientation.x = ::atof((str.data.substr(str.data.find("qX:")+3, str.data.find("qY:")-(str.data.find("qX:")+3)).c_str()));
    imuData.orientation.y = ::atof((str.data.substr(str.data.find("qY:")+3, str.data.find("qZ:")-(str.data.find("qY:")+3)).c_str()));
    imuData.orientation.z = ::atof((str.data.substr(str.data.find("qZ:")+3, str.data.find("avX:")-(str.data.find("qZ:")+4)).c_str()));
    imuData.angular_velocity.x = ::atof((str.data.substr(str.data.find("avX:")+4, str.data.find("avY:")-(str.data.find("avX:")+4)).c_str()));
    imuData.angular_velocity.y = ::atof((str.data.substr(str.data.find("avY:")+4, str.data.find("avZ:")-(str.data.find("avY:")+4)).c_str()));
    imuData.angular_velocity.z = ::atof((str.data.substr(str.data.find("avZ:")+4, str.data.find("laX:")-(str.data.find("avZ:")+4)).c_str()));
    imuData.linear_acceleration.x = ::atof((str.data.substr(str.data.find("laX:")+4, str.data.find("laY:")-(str.data.find("laX:")+4)).c_str()));
    imuData.linear_acceleration.y = ::atof((str.data.substr(str.data.find("laY:")+4, str.data.find("laZ:")-(str.data.find("laY:")+4)).c_str()));
    imuData.linear_acceleration.z = ::atof((str.data.substr(str.data.find("laZ:")+4).c_str()));
    // parsing end

    imu_publisher.publish(imuData);
    ++ imuData.header.seq;

}

void RosBno055::startPublish()
{
    ros::spin();
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "bno055_ros_bridge");
    ros::NodeHandle imu_nh;
    RosBno055 sensor(imu_nh);
    sensor.startPublish();
    return 0;
}
