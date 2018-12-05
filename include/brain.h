
#ifndef BRAIN_H
#define BRAIN_H
#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <nxt_control/SensorData.h>
#include <visualization_msgs/Marker.h>
#include <sensor_msgs/Range.h>

class SubPub{
private:
  ros::NodeHandle n_;
  ros::Publisher pubVel_;
  ros::Publisher pubRange_;
  ros::Subscriber subOdom_;
  ros::Subscriber subSensor_;

  int los_;
  float d_;
//  bool obstacle_;

  struct Pose {
    double x, y, th;
  };
  Pose _goal;
  Pose _pose;

public:
  SubPub(int los);

//  void getPose(const nav_msgs::Odometry& msg);
  void QuadratFahren(const nav_msgs::Odometry& msg);
  void getSensor(const nxt_control::SensorData& msg);

  void run(){
    geometry_msgs::Twist vel;
    vel.linear.x = 1;
    vel.linear.y = 0;
    vel.linear.z = 0;
    vel.angular.x = 0;
    vel.angular.y = 0;
    vel.angular.z = 0;
    pubVel_.publish(vel);
  }

  void stop(){
    geometry_msgs::Twist vel;
    vel.linear.x = 0;
    vel.linear.y = 0;
    vel.linear.z = 0;
    vel.angular.x = 0;
    vel.angular.y = 0;
    vel.angular.z = 0;
    pubVel_.publish(vel);
  }

  void rotate(float z){
    geometry_msgs::Twist vel;
    vel.linear.x = 0;
    vel.linear.y = 0;
    vel.linear.z = 0;
    vel.angular.x = 0;
    vel.angular.y = 0;
    vel.angular.z = z;
    pubVel_.publish(vel);
  }
};

#endif /*BRAIN_H*/
