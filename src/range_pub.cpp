#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <nxt_control/SensorData.h>
#include <visualization_msgs/Marker.h>
#include <sensor_msgs/Range.h>

//sensor_msgs::Range Control::publishUltrasonicRange(
//		const nxt_control::SensorData& msg) {
//	sensor_msgs::Range mr;
//	mr.field_of_view = 1;
//	mr.header.frame_id = "ultrasonic";
//	mr.header.stamp = ros::Time::now();
//	mr.max_range = 2.55;
//	mr.min_range = 0;
//	mr.radiation_type = 1;
//	mr.range = static_cast<float>(msg.distance) / 100;
//	return mr;
//}
class SubAndPub{
public:
  SubAndPub(){
    pub_ = n_.advertise<sensor_msgs::Range>("ultra_range", 10);
    //    subSensor_ = n_.subscribe("sensor_data", 10, &SubAndPub::callback, this);
    subSensor_ = n_.subscribe("sensor_data", 10, &SubAndPub::callbackSensor, this);
  }

  void callbackSensor(const nxt_control::SensorData& sensor_data){
    sensor_msgs::Range range;
    //    int64 l = sensor_data.tickCountA;
    //    int64 r = sensor_data.tickCountB;
    int d = sensor_data.distance;
    ROS_INFO_STREAM("distance= (" <<d  <<") ");
    range.range = d;
    pub_.publish(range);
  }

private:
  ros::NodeHandle n_;
  ros::Publisher pub_;
  ros::Subscriber subSensor_;
};

int main(int argc, char **argv){
  ros::init(argc, argv, "range_pub");

  SubAndPub Object;

  ros::spin();

  return 0;
}


//#include <ros/ros.h>
//#include <nav_msgs/Odometry.h>
//#include <geometry_msgs/Twist.h>
//#include <nxt_control/SensorData.h>

//class SubAndPub{
//public:
//  SubAndPub(){
//    subSensor_ = n_.subscribe("sensor_data", 10, &SubAndPub::callbackSensor, this);
//  }

//  void callbackSensor(const nxt_control::SensorData& sensor_data){
//    //.... do something with the input and generate the output...
//    int l = sensor_data.tickCountA;
//    int r = sensor_data.tickCountB;
//    int d = sensor_data.distance;
//    ROS_INFO_STREAM("tickCountA, B, distance= (" <<l <<", " <<r <<", " <<d  <<") ");
//  }

//private:
//  ros::NodeHandle n_;
//  ros::Publisher pub_;
//  ros::Subscriber subOdom_;
//  ros::Subscriber subSensor_;
//};

//int main(int argc, char **argv){
//  ros::init(argc, argv, "range_pub");

//  double trans, rot;
//  std::cout << "moving x= ";
//  std::cin >> trans;
//  std::cout << "turning z= ";
//  std::cin >> rot;
//  SubAndPub Object();

//  ros::spin();

//  return 0;
//}
