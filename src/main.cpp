#include <iostream>
#include "ros/ros.h"
#include "GripperQuickChange.h"
#include "Hsc3apiInstance.h"
#include "gripperquickchange_bridge/quickChange_set4.h"
#include "gripperquickchange_bridge/quickChange_set.h"
#include "gripperquickchange_bridge/quickChange_run.h"
using namespace std;



GripperQuickChange gripperQuickChange;

bool QuickChange_setCB(gripperquickchange_bridge::quickChange_set4::Request &req, gripperquickchange_bridge::quickChange_set4::Response &res)
{
    gripperQuickChange.init_GripperSelf();
    gripperQuickChange.gripperSelf->RobToolSelf.gripper_name=req.Setrobottool.gripper_name;
    gripperQuickChange.gripperSelf->RobToolSelf.is_hasGripper=req.Setrobottool.is_hasGripper;
    gripperQuickChange.gripperSelf->RobToolSelf.grippersort=gripperQuickChange.gripperSelf->map_GripperSort[req.Setrobottool.gripper_sort];

    gripperQuickChange.gripperSelf->map_GripperSelf["fourfingerGripper"].is_hasGripper=req.SetGripperShelf.shelf_p0_hasgripper;
    gripperQuickChange.gripperSelf->map_GripperSelf["noPowerGripper"].is_hasGripper=req.SetGripperShelf.shelf_p1_hasgripper;
    gripperQuickChange.gripperSelf->map_GripperSelf["twofingerGripper"].is_hasGripper=req.SetGripperShelf.shelf_p2_hasgripper;
    gripperQuickChange.gripperSelf->map_GripperSelf["fivefingerGripper"].is_hasGripper=req.SetGripperShelf.shelf_p3_hasgripper;
//    for (int i = 0; i <4; ++i) {
//        for (auto & iter : gripperQuickChange.gripperSelf->map_GripperSelf) {
//            if (iter.second.index_SelfPose == i)
//            {
////                iter.second.is_hasGripper=req.SetGripperShelf.;
//            }
//            continue;
//        }
//    }
    gripperQuickChange.printInfo();
    return true;
}

bool QuickChange_runCB(gripperquickchange_bridge::quickChange_run::Request &req, gripperquickchange_bridge::quickChange_run::Response &res)
{
    if(gripperQuickChange.quickChangeGripper(req.switchGripperName)!=0){
        cout<<"夹具更换失败"<<endl;
        res.is_success= false;
        return true;
    }
    res.is_success= true;
    gripperQuickChange.printInfo();
    return true;
}

int main(int argc,char** argv){

    //ros节点
    string nodeName = "gripperquickchange_bridge";
    ros::init(argc, argv, nodeName);
    ros::NodeHandle node;

    //创建节点
    ros::AsyncSpinner spinner(2);
    spinner.start();
//
    ros::ServiceServer quickChange_start_server = node.advertiseService("gripperquickchange/set", QuickChange_setCB);
    ros::ServiceServer quickChange_run_server = node.advertiseService("gripperquickchange/run", QuickChange_runCB);

//    cout<<"更换夹具fourfingerGripper"<<endl;
//    if(gripperQuickChange.quickChangeGripper("fourfingerGripper")!=0){
//        cout<<"夹具更换失败"<<endl;
//        return 0;
//    }
//    gripperQuickChange.printInfo();
    ros::MultiThreadedSpinner spinner1(1);
    spinner1.spin();
    return 0;
}

