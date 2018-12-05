#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <nxt_control/SensorData.h>

class SubAndPub{
public:
  SubAndPub(double trans, double rot){
    trans_ = trans;
    rot_ = rot;
//    pub_ = n_.advertise<geometry_msgs::Twist>("cmd_vel", 10);
//    subOdom_ = n_.subscribe("odom", 10, &SubAndPub::callback, this);
    subSensor_ = n_.subscribe("sensor_data", 10, &SubAndPub::callbackSensor, this);
  }

//  void callback(const nav_msgs::Odometry& odom_msg){
//    static double dis=0;
//    static double theta=0;
//    int t_trans = 0;
//    int t_rot = 0;

//    geometry_msgs::Twist vel_msg;
//    vel_msg.linear.x=trans_;
//    vel_msg.angular.z=rot_;
//    double x=odom_msg.pose.pose.position.x;
//    double y=odom_msg.pose.pose.position.y;
//    theta+=odom_msg.pose.pose.orientation.z;
//    dis+=sqrt(x*x + y*y);
//    ROS_INFO_STREAM("pose=[" <<x <<", " <<y <<"] " << "d=" <<dis << " a=" <<theta);

//    if(dis>1){
//      vel_msg.linear.x = 0;
//      vel_msg.angular.z = 0;
//      ROS_INFO_STREAM("=============== 1 m ! ===============");
////      t_trans += 1;
//    }
//    if(theta>M_PI/2){
//      vel_msg.linear.x = 0;
//      vel_msg.angular.z = 0;
//      ROS_INFO_STREAM("===============  90 grad ! ===============");
////      t_rot += 1;
//    }
//    if (t_trans==4 && t_rot==4){
//      vel_msg.linear.x = 0;
//      vel_msg.angular.z = 0;
//      ROS_INFO_STREAM("===============  arrived!! ===============");
//    }

//    pub_.publish(vel_msg);
//  }

  void callbackSensor(const nxt_control::SensorData& sensor_data){
    //.... do something with the input and generate the output...
    int l = sensor_data.tickCountA;
    int r = sensor_data.tickCountB;
    int d = sensor_data.distance;
    ROS_INFO_STREAM("tickCountA, B, distance= (" <<l <<", " <<r <<", " <<d  <<") ");
  }

private:
  double trans_;
  double rot_;
  ros::NodeHandle n_;
  ros::Publisher pub_;
  ros::Subscriber subOdom_;
  ros::Subscriber subSensor_;
};

int main(int argc, char **argv){
  ros::init(argc, argv, "pub_sub");

//  double m=0;
//  std::cout << ("how far ? dis= \n");
//  std::cin >> m;
  double trans, rot;
  std::cout << "moving x= ";
  std::cin >> trans;
  std::cout << "turning z= ";
  std::cin >> rot;
  SubAndPub Object(trans, rot);

  ros::spin();

  return 0;
}
