#include <ros/ros.h>
#include <nxt_control/SensorData.h>
#include "geometry_msgs/PoseWithCovarianceStamped.h"	// F眉r Pose Parameter

//#include <iostream>

#include <geometry_msgs/Twist.h>

bool bdriving;
bool brotate;
bool bgoal;

ros::Publisher pubDrive;

void rotate(float rot)
{
  geometry_msgs::Twist msg;

  msg.angular.x = 0;
  msg.angular.y = 0;
  msg.angular.z = rot;

  msg.linear.x = 0;
  msg.linear.y = 0;
  msg.linear.z = 0;

  pubDrive.publish(msg);
}

void drive (float drive)
{
  geometry_msgs::Twist msg;

  msg.angular.x = 0;
  msg.angular.y = 0;
  msg.angular.z = 0;

  msg.linear.x = drive;
  msg.linear.y = 0;
  msg.linear.z = 0;
  pubDrive.publish(msg);
}

void goal (const geometry_msgs::PoseWithCovarianceStamped odom)
{

//	if (odom.pose.pose.position.x >0.5 && odom.pose.pose.position.x<1.5 && odom.pose.pose.position.y <-0.5 && odom.pose.pose.position.y>-1.5)
  if ( odom.pose.pose.position.y <-0.5 && odom.pose.pose.position.y>-1.5)

  {
    bgoal = true;
  }else
  {
    bgoal = false;
  }
}

void count(const nxt_control::SensorData msg) {
  static int countA = 0;
  static int countB = 0;
  countA += msg.tickCountA;
  countB += msg.tickCountB;
  double dist = msg.distance;
  if (dist < 30)
  {
    brotate = true;
  }else
  {
    brotate = false;
  }

  ROS_INFO("%d , %d , %f",countA,countB, dist);
//	std::cout<<countA<<","<<countB<<std::endl;
}





int main(int argc, char **argv) {
  ros::init(argc, argv, "brain2");
  ros::NodeHandle n;
  ros::Subscriber sub;
  ros::Subscriber subGoal;
  sub = n.subscribe("sensor_data", 1, count);
  subGoal = n.subscribe("odom",1, goal);
  pubDrive = n.advertise<geometry_msgs::Twist>("cmd_vel", 10);

  ros::Rate loop_rate(30);


    /*while (true){
      geometry_msgs::Twist msg;

          msg.angular.x = 0;
          msg.angular.y = 0;
          msg.angular.z = 0;

          msg.linear.x = 1;
          msg.linear.y = 0;
          msg.linear.z = 0;
          pubDrive.publish(msg);
    }*/

  while(ros::ok())
  {
    //drive(1);
    ros::spinOnce();
    loop_rate.sleep();

    if (bgoal)
    {
      drive(0);
    }else if (brotate)
    {
      rotate(1.57);
    }else
    {
      drive(1);
    }

    //Algorithmus

  }

  return 1;
}
