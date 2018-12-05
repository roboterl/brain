cmake_minimum_required(VERSION 2.8.3)
project(brain)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  nav_msgs
  nxt_control
  roscpp
  std_msgs
  tf
  sensor_msgs
)

catkin_package()
include_directories(include
  ${catkin_INCLUDE_DIRS}
)

add_executable(brain_node src/brain.cpp)
add_executable(pub_sub_node src/pub_sub.cpp)
add_executable(tf_pub_node src/tf_broadcaster.cpp)
add_executable(range_pub_node src/range_pub.cpp)

target_link_libraries(brain_node ${catkin_LIBRARIES})
target_link_libraries(pub_sub_node ${catkin_LIBRARIES})
target_link_libraries(tf_pub_node ${catkin_LIBRARIES})
target_link_libraries(range_pub_node ${catkin_LIBRARIES})
