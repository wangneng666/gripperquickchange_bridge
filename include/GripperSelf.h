
#ifndef GRIPPERQUICKCHANGE_BRIDGE_GRIPPERSELF_H
#define GRIPPERQUICKCHANGE_BRIDGE_GRIPPERSELF_H
#include <iostream>
#include <map>
#include <utility>
#include <malloc.h>
#include "Hsc3RobotMove.h"
using namespace std;
//夹具类型
enum GripperSort{
    noPower=0,
    robotIO_twofinger,
    robotIO_fourfinger,
    serial_gripper_fivefinger,
};
static const char *GripperSortStrings[] = {"noPower","serial_gripper_twofigner","serial_gripper_fourfigner","serial_gripper_fivefigner","robotIO_gripper"};
//夹具点位信息
struct GripperPose{
    vector<vector<double >> UnloadMovePose_1;
    vector<vector<double >> UnloadMovePose_2;
    vector<vector<double >> loadMovePose_1;
    vector<vector<double >> loadMovePose_2;
};

//夹具工位信息
struct Gripperinfo{
    int index_SelfPose;//工位
    GripperSort grippersort;//夹具类型
    bool is_hasGripper;//是否有夹具
    string gripper_name;
    GripperPose gripperPose;
};

struct ProgSelect{
    RValueProg unload;
    RValueProg load;
};

class GripperSelf{
public:
    //设置夹具仓位数量
    explicit GripperSelf();
    ~GripperSelf();

public:
    map<string,Gripperinfo> map_GripperSelf;//夹爪对应的工位信息
    map<string,ProgSelect> map_GripperProgSelect;//夹爪对应的程序位
    map<string,GripperSort> map_GripperSort;

    Gripperinfo RobToolSelf;   //机器人工具架信息
public:
    //设置机器人工具架信息
    int setRobToolSelf(Gripperinfo gripperinfo);
    //设置夹具架各工位信息
    int setGripperSelf(string gripper_name,Gripperinfo gripperinfo);
    //设置夹爪对应的运动点位程序信息
    int setGripperProgSelect(string gripper_name,ProgSelect progSelect);

};





#endif //GRIPPERQUICKCHANGE_BRIDGE_GRIPPERSELF_H
