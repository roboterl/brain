#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_datatypes.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <nxt_control/SensorData.h>

void callbackOdom(const nav_msgs::Odometry& odom_msg){
  static double x = 0;
  static double y = 0;
  x+=odom_msg.pose.pose.position.x;
  y+=odom_msg.pose.pose.position.y;
  ROS_INFO_STREAM("pose=[" <<x <<", " <<y <<"] ");
//  theta+=odom_msg.pose.pose.orientation.z;
  static tf::TransformBroadcaster br;
//  tf::Transform transform;
//  transform.setOrigin(tf::Vector3(3, 0, 0));
//  tf::Quaternion q;
//  q.setRPY(0, 0, 0);
//  transform.setRotation(q);
  br.sendTransform(
        tf::StampedTransform(
          tf::Transform(tf::Quaternion(0,0,0,1), tf::Vector3(0.09, 0, 0)),
          ros::Time::now(),
          "odom",
          "Ultrasonic"));

}


int main(int argc, char **argv){
  ros::init(argc, argv, "tf_pub");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("odom", 10, &callbackOdom);

  ros::spin();
  return 0;
  }
