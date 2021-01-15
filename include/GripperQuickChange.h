#ifndef GRIPPERMANAGER_BRIDGE_GRIPPERQUICKCHANGE_H
#define GRIPPERMANAGER_BRIDGE_GRIPPERQUICKCHANGE_H

#include "GripperManage.h"
#include "Hsc3RobotMove.h"
#include "GripperSelf.h"
#include "yaml-cpp/yaml.h"
#include <iostream>
#include <map>
#include "ros/ros.h"
#include "ros/package.h"
using namespace std;
using namespace hirop_gripper;


class GripperQuickChange {
public:
    explicit GripperQuickChange();
    ~GripperQuickChange();
    //加载配置文件
    void parseConfigYmal();
    //运行机器人示教程序
    int runOrStopRbProg(string programName,bool flag_run);
    //初始化夹具架信息
    int init_GripperSelf();
    //快换某夹具
    int quickChangeGripper(string gripper_name);
    //装载夹具
    int load_gripper(string gripper_name);
    //卸载夹具
    int unload_gripper(string gripper_name);
    //切换夹爪连接
    int reConnNewGripper(string gripper_name);
    //打印信息
    void printInfo();
    //打印点位信息
    void printPoseInfo(GripperPose & _vv);
    //刷新夹具架基本信息
    void updateShelfinfo();

public:
    GripperSelf* gripperSelf;
    Hsc3RobotMove hsc3RobotMove;
    GripperManage gripperManage;
    string err_message;
};


#endif //GRIPPERMANAGER_BRIDGE_GRIPPERQUICKCHANGE_H
