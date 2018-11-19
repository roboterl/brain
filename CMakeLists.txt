cmake_minimum_required(VERSION 2.8.3)
project(brain)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  nav_msgs
  nxt_control
  roscpp
  std_msgs
  tf
)

catkin_package()
include_directories( ${catkin_INCLUDE_DIRS})

add_executable(brain_node src/brain.cpp)
add_executable(pub_sub_node src/pub_sub.cpp)

target_link_libraries(brain_node ${catkin_LIBRARIES})
target_link_libraries(pub_sub_node ${catkin_LIBRARIES})
