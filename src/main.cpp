#include <iostream>
#include "ros/ros.h"
#include "ros/package.h"

#include "GripperQuickChange.h"
#include "Hsc3apiInstance.h"
#include "hirop_msgs/quickChange_set4.h"
#include "hirop_msgs/quickChange_run.h"
#include "hirop_msgs/shelfStatus.h"
#include "hirop_msgs/quickChange_getStatus.h"
#include "hirop_msgs/quickChange_robStepMove.h"
#include "std_srvs/Empty.h"
using namespace std;


GripperQuickChange gripperQuickChange;

ros::Publisher pub_shelfStatus;

bool QuickChange_setCB(hirop_msgs::quickChange_set4::Request &req, hirop_msgs::quickChange_set4::Response &res)
{
    if(gripperQuickChange.init_GripperSelf()!=0){
        res.is_success= false;
        cout<<"server failed"<<endl;
        return true;
    }

    gripperQuickChange.gripperSelf->RobToolSelf.gripper_name=req.Setrobottool.gripper_name;
    gripperQuickChange.gripperSelf->RobToolSelf.is_hasGripper=req.Setrobottool.is_hasGripper;
    gripperQuickChange.gripperSelf->RobToolSelf.grippersort=gripperQuickChange.gripperSelf->map_GripperSort[req.Setrobottool.gripper_sort];

    gripperQuickChange.gripperSelf->map_GripperSelf["fourfingerGripper"].is_hasGripper=req.SetGripperShelf.shelf_p0_hasgripper;
    gripperQuickChange.gripperSelf->map_GripperSelf["noPowerGripper"].is_hasGripper=req.SetGripperShelf.shelf_p1_hasgripper;
    gripperQuickChange.gripperSelf->map_GripperSelf["twofingerGripper"].is_hasGripper=req.SetGripperShelf.shelf_p2_hasgripper;
    gripperQuickChange.gripperSelf->map_GripperSelf["fivefingerGripper"].is_hasGripper=req.SetGripperShelf.shelf_p3_hasgripper;
    gripperQuickChange.printInfo();
    hirop_msgs::shelfStatus msg;
    msg.shelf_p0_hasgripper=gripperQuickChange.gripperSelf->map_GripperSelf["fourfingerGripper"].is_hasGripper;
    msg.shelf_p1_hasgripper=gripperQuickChange.gripperSelf->map_GripperSelf["noPowerGripper"].is_hasGripper;
    msg.shelf_p2_hasgripper=gripperQuickChange.gripperSelf->map_GripperSelf["twofingerGripper"].is_hasGripper;
    msg.shelf_p3_hasgripper=gripperQuickChange.gripperSelf->map_GripperSelf["fivefingerGripper"].is_hasGripper;
    msg.robotTool_hasGripper=gripperQuickChange.gripperSelf->RobToolSelf.is_hasGripper;
    msg.robotTool_gripper_name=gripperQuickChange.gripperSelf->RobToolSelf.gripper_name;
    pub_shelfStatus.publish(msg);
    res.is_success= true;
    return true;
}

bool QuickChange_runCB(hirop_msgs::quickChange_run::Request &req, hirop_msgs::quickChange_run::Response &res)
{
    //启动机器人程序
    if(gripperQuickChange.runOrStopRbProg("GQC.PRG",true)!=0){
        res.is_success= false;
        cout<<"server failed"<<endl;
        return true;
    }
    if(gripperQuickChange.quickChangeGripper(req.switchGripperName)!=0){
        cout<<"夹具更换失败"<<endl;
        res.is_success= false;
        res.info=gripperQuickChange.err_message;
    } else
    {
        res.is_success= true;
        gripperQuickChange.printInfo();
        hirop_msgs::shelfStatus msg;
        msg.shelf_p0_hasgripper=gripperQuickChange.gripperSelf->map_GripperSelf["fourfingerGripper"].is_hasGripper;
        msg.shelf_p1_hasgripper=gripperQuickChange.gripperSelf->map_GripperSelf["noPowerGripper"].is_hasGripper;
        msg.shelf_p2_hasgripper=gripperQuickChange.gripperSelf->map_GripperSelf["twofingerGripper"].is_hasGripper;
        msg.shelf_p3_hasgripper=gripperQuickChange.gripperSelf->map_GripperSelf["fivefingerGripper"].is_hasGripper;
        msg.robotTool_hasGripper=gripperQuickChange.gripperSelf->RobToolSelf.is_hasGripper;
        msg.robotTool_gripper_name=gripperQuickChange.gripperSelf->RobToolSelf.gripper_name;
        pub_shelfStatus.publish(msg);
    }
    //关闭机器人程序
    if(gripperQuickChange.runOrStopRbProg("GQC.PRG",false)!=0){
        res.is_success= false;
        cout<<"server failed"<<endl;
        return true;
    }
    return true;
}

bool QuickChange_stopCB(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res){
    //关闭机器人程序
    gripperQuickChange.runOrStopRbProg("GQC.PRG",false);
    return true ;
}

bool QuickChange_getCB(hirop_msgs::quickChange_getStatus::Request &req, hirop_msgs::quickChange_getStatus::Response &res){
    res.robotTool_hasGripper=gripperQuickChange.gripperSelf->RobToolSelf.is_hasGripper;
    res.robotTool_gripper_name=gripperQuickChange.gripperSelf->RobToolSelf.gripper_name;
    res.is_success= true;
    return true ;
}

bool QuickChange_robStepMoveCB(hirop_msgs::quickChange_robStepMove::Request &req, hirop_msgs::quickChange_robStepMove::Response &res){
    string program_name=req.tp_progName;//"STEPP.PRG"
    //启动机器人程序
    if(gripperQuickChange.runOrStopRbProg(program_name,true)!=0){
        res.is_success= false;
        cout<<"server failed"<<endl;
        return true;
    }
    //等待机器人动作完成
    if(gripperQuickChange.hsc3RobotMove.waitRSignal(51,1.0)!=0){
        res.is_success= false;
        cout<<"wait failed"<<endl;
        return true;
    }
    if(Hsc3apiInstance::getInstance()->setR(52,1.0)!=0){
        res.is_success= false;
        cout<<"setR 设置失败"<<endl;
        return true;
    }
    if(gripperQuickChange.hsc3RobotMove.waitRSignal(51,0.0)!=0){
        res.is_success= false;
        cout<<"wait failed"<<endl;
        return true;
    }

    //关闭机器人程序
    if(gripperQuickChange.runOrStopRbProg(program_name,false)!=0){
        res.is_success= false;
        cout<<"server failed"<<endl;
        return true;
    }
    res.is_success= true;
    return true ;
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
    pub_shelfStatus=node.advertise<hirop_msgs::shelfStatus>("gripperquickchange/status", 1);
    ros::ServiceServer quickChange_start_server = node.advertiseService("gripperquickchange/set", QuickChange_setCB);
    ros::ServiceServer quickChange_run_server = node.advertiseService("gripperquickchange/run", QuickChange_runCB);
    ros::ServiceServer quickChange_stop_server = node.advertiseService("gripperquickchange/stop", QuickChange_stopCB);
    ros::ServiceServer quickChange_get_server = node.advertiseService("gripperquickchange/getStatus", QuickChange_getCB);
    ros::ServiceServer quickChange_robStepMove_server = node.advertiseService("gripperquickchange/robStepMove", QuickChange_robStepMoveCB);
    cout<<"快换桥服务开启完毕"<<endl;
    ros::MultiThreadedSpinner spinner1(1);
    spinner1.spin();
    return 0;
}

