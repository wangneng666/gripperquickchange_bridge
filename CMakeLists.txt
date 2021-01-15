cmake_minimum_required(VERSION 3.0.2)
project(gripperquickchange_bridge)

## Compile as C++11, supported in ROS Kinetic and newer
 add_compile_options(-std=c++11)
add_definitions(-D__LINUX__)
#add_definitions(-D_COUT_)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rosmsg
  rospy
  hirop_msgs
  roslib
   #message_generation
)
#add_message_files(
#       FILES
#       robotToolInfo.msg
#       gripperShelfInfo.msg
#       gripperShelf_4p.msg
#)
### Generate services in the 'srv' folder
#add_service_files(
#       FILES
#       quickChange_run.srv
#       quickChange_set.srv
#       quickChange_set4.srv
#)

# Generate added messages and services with any dependencies listed here
#generate_messages(
#       DEPENDENCIES
#       std_msgs  # Or other packages containing msgs
#)


find_package(hirop_gripper REQUIRED)

file(GLOB_RECURSE EXTRA_FILES */*)
add_custom_target(${PROJECT_NAME}_OTHER_FILES ALL WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} SOURCES ${EXTRA_FILES})


catkin_package(
        INCLUDE_DIRS include
        CATKIN_DEPENDS
        message_runtime
)

link_directories(./3rd/lib/HsApi)
link_directories(./3rd/lib/boost_linux64)

include_directories(
  include
  3rd/include/
  3rd/include/proxy
  /home/fshs/catkin_ws/devel/include
  ${catkin_INCLUDE_DIRS}
  ${hirop_gripper_INCLUDE_DIRS}
)

#link_directories(/usr/lib/x86_64-linux-gnu/)


add_executable(gripperquickchange_bridge
        src/main.cpp
        src/GripperQuickChange.cpp
        src/Hsc3apiInstance.cpp
        src/Hsc3RobotMove.cpp
        src/GripperManage.cpp
        src/GripperSelf.cpp
        )

#add_dependencies(gripperquickchange_bridge ${hirop_msgs_EXPORTED_TARGETS}　${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(gripperquickchange_bridge
        ${catkin_LIBRARIES}
        ${hirop_gripper_LIBRARIES}
        dl
        yaml-cpp
        CommApi
        Hsc3Api
        LogApi
        boost_chrono
        boost_date_time
        boost_thread
        boost_system
        boost_filesystem
        )



