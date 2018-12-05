#include <ros/ros.h>
#include "../include/brain.h"
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <nxt_control/SensorData.h>
#include <visualization_msgs/Marker.h>
#include <sensor_msgs/Range.h>
#include <tf/transform_broadcaster.h>
#include <fstream>
#include <iostream>
#include <time.h>
#include <ros/duration.h>

using namespace std;

SubPub::SubPub(int los){
  _goal.x = 0;
  _goal.y = 0;
  _goal.th = 0;
  _pose.x = 0;
  _pose.y = 0;
  _pose.th = 0;

  los_ = los;
  d_ = 0;
//  obstacle_ = true;
  pubVel_ = n_.advertise<geometry_msgs::Twist>("cmd_vel", 10);
  pubRange_ = n_.advertise<sensor_msgs::Range>("ultra_range", 10);
  subOdom_ = n_.subscribe("odom", 10, &SubPub::QuadratFahren, this);
  subSensor_ = n_.subscribe("sensor_data", 10, &SubPub::getSensor, this);
}

void SubPub::QuadratFahren(const nav_msgs::Odometry& odom_msg){
  static double dis=0;
  static double theta=0;

  double x = odom_msg.pose.pose.position.x;
  double y = odom_msg.pose.pose.position.y;
  double th = odom_msg.pose.pose.orientation.z;

  _pose.th = fmod(_pose.th + th, 2*M_PI);
  _pose.x += x*cos(_pose.th);
  _pose.y += x*sin(_pose.th);

  theta += th;
  dis += sqrt(x*x + y*y);
//  ROS_INFO("rotation:[%.0f]", theta/M_PI*180);

  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(_pose.x, _pose.y, 0.0) );
  tf::Quaternion q;
  q.setRPY(0, 0, _pose.th);
  transform.setRotation(q);
  br.sendTransform(
        tf::StampedTransform(transform,
                             ros::Time::now(),
                             "base_link",
                             "Roboter")
        );
  //  br.sendTransform(
  //        tf::StampedTransform(
  //          tf::Transform(tf::Quaternion(0,0,_pose.th,1), tf::Vector3(_pose.x, _pose.y, 0)),
  //          ros::Time::now(),
  //          //          "Roboter",
  //          //          "Ultrasonic"));
  //          "base_link",
  //          "Roboter"));

  //  ==================================       kalibieren      ================================== //
  //      if(los_!=0){    // los_=1: run 1m
  //        if(dis>=1){
  //          stop();
  //        }else{
  //          run();
  //        }
  //      }else{    // los_=0: rotate 90
  //        if(theta>=M_PI/2){
  //          stop();
  //        }else{
  //          rotate(1);
  //        }
  //      };

  //  ==================================     QuadratFahren      ================================== //
  //  ROS_INFO("cor:[%.2f, %.2f, %.2f]",_pose.x, _pose.y, _pose.th);
  //  fstream file("/home/liang/Documents/QuadratFahren.txt", ios::app);
  //  file << fixed << setprecision(3) << "[" <<_pose.x <<", " <<_pose.y <<", " <<_pose.th << "]" << endl;
  //  file.close();

  //  if(los_!=0){
  //    if(dis>=0.5){
  //      rotate(1);
  //      if(theta>=M_PI/2){
  //        run();
  //        dis=0;
  //      };
  //    }else{
  //      run();
  //      theta=0;
  //    }
  //  }else{
  //    stop();
  //  };

  //  ==================================  ultraschall fahren   ================================== //
  int FALL;
  if(theta>=0 && theta<M_PI/2){FALL=1;}         // orientierung: 0~90
  else if(theta>=M_PI/2){FALL= 2;}              // orientierung: >90
  else if(theta<0 && theta>-M_PI){FALL=3;}      // orientierung: 90~-90
  else{FALL=4;};
  ROS_INFO("obstacle: [%.0f]cm, rotate:[%.0f]deg", d_, theta/M_PI*180);

  if(los_!=0){
    if(d_<30){
      switch (FALL) {
      case 1: rotate(1); break;
      case 2: theta=0; rotate(-1); break;
      case 3: rotate(-1); break;
      case 4: stop(); ROS_INFO_STREAM("NO WAY TO GO!!!"); break;
      default: break;
      };
    }else{
      run();
      theta = 0;
    }
  }else{
    stop();
  }
  //  =========================================================================================== //

}



void SubPub::getSensor(const nxt_control::SensorData& sensor_data){
  sensor_msgs::Range r;
  //  int l = sensor_data.tickCountA;
  //  int r = sensor_data.tickCountB;
  d_ = sensor_data.distance;
  r.field_of_view = 1;
  r.header.frame_id = "Roboter";
  r.header.stamp = ros::Time::now();
  r.max_range = 2.55;
  r.min_range = 0;
  r.radiation_type = 1;
  r.range = d_ / 100;
  pubRange_.publish(r);

//  ROS_INFO_STREAM("distance= (" <<d  <<") ");
//  if(d<30){
//    obstacle_ = true;
//  }else{
//    obstacle_ = false;
//  }
}

int main(int argc, char **argv){
  ros::init(argc, argv, "brain");

  int los;
  std::cout << "los fahren(1) oder bleiben(0) ?? ";
  std::cin >> los;
  SubPub Object(los);

  ros::spin();

  return 0;
}
