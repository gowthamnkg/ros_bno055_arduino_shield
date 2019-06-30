#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Imu.h>
#include <string>

class RosBno055
{
    private:
        ros::NodeHandle& imu_nh;
        ros::Publisher imu_publisher;
        ros::Subscriber arduino_subscriber;
        ros::Timer imu_timer;
        std::string frame_id_;
        sensor_msgs::Imu imuData;
    public:
        RosBno055(ros::NodeHandle& imu_nh_);
        ~RosBno055();
        void imuArduinoCb(const std_msgs::String& str);
        void startPublish();
};