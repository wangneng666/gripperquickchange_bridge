#include "GripperQuickChange.h"

GripperQuickChange::GripperQuickChange() {
    gripperSelf=new GripperSelf();
}

GripperQuickChange::~GripperQuickChange(){
    delete gripperSelf;
}

void GripperQuickChange::parseConfigYmal() {
    //参数解析
    string configFile_path=ros::package::getPath("gripperquickchange_bridge")+"/config/GripperSelf.yaml";
    YAML::Node yaml_node=YAML::LoadFile(configFile_path);
    //装填夹具架信息
    vector<string> shelf_pose{"index_SelfPose_0","index_SelfPose_1","index_SelfPose_2","index_SelfPose_3"};
    for (int i = 0; i <shelf_pose.size(); ++i) {
        string gripperName = yaml_node["gripperSelf"][shelf_pose[i]]["gripper_name"].as<string>();
        bool hasGripper = yaml_node["gripperSelf"][shelf_pose[i]]["is_hasGripper"].as<bool>();
        string gripper_sort = yaml_node["gripperSelf"][shelf_pose[i]]["gripper_sort"].as<string>();
        GripperPose gripperPose;
        gripperPose.UnloadMovePose_1=yaml_node["gripperSelf"][shelf_pose[i]]["UnloadMovePose_1"].as<vector<vector<double >>>();
        gripperPose.UnloadMovePose_2=yaml_node["gripperSelf"][shelf_pose[i]]["UnloadMovePose_2"].as<vector<vector<double >>>();
        gripperPose.loadMovePose_1=yaml_node["gripperSelf"][shelf_pose[i]]["loadMovePose_1"].as<vector<vector<double >>>();
        gripperPose.loadMovePose_2=yaml_node["gripperSelf"][shelf_pose[i]]["loadMovePose_2"].as<vector<vector<double >>>();
        gripperSelf->setGripperSelf(gripperName,{i,gripperSelf->map_GripperSort[gripper_sort], hasGripper,gripperName,gripperPose});
        ProgSelect progSelect;
        progSelect.unload=(RValueProg)yaml_node["gripperSelf"][shelf_pose[i]]["progSelect_unload"].as<int>();
        progSelect.load=(RValueProg)yaml_node["gripperSelf"][shelf_pose[i]]["progSelect_load"].as<int>();
        gripperSelf->setGripperProgSelect(gripperName,progSelect);
    }
    //装填机器人工具位信息
    Gripperinfo robtoolselfinfo;
    robtoolselfinfo.gripper_name=yaml_node["gripperSelf"]["RobToolSelf"]["gripper_name"].as<string>();
    robtoolselfinfo.is_hasGripper=yaml_node["gripperSelf"]["RobToolSelf"]["is_hasGripper"].as<bool>();
    robtoolselfinfo.grippersort=gripperSelf->map_GripperSort[yaml_node["gripperSelf"]["RobToolSelf"]["gripper_sort"].as<string>()];
    gripperSelf->setRobToolSelf(robtoolselfinfo);

}

int GripperQuickChange::init_GripperSelf() {
    parseConfigYmal();
    return 0;
}

int GripperQuickChange::quickChangeGripper(string gripper_name) {
    //检查此工具在夹具架中是否具有
    if(gripperSelf->map_GripperSelf.count(gripper_name)==0){
        cout<<"map_GripperSelf中不存在"<<gripper_name<<endl;
        err_message="夹具架中不存在此夹具!";
        return -1;
    }
    //拒绝自己更换自己
    if(gripper_name==gripperSelf->RobToolSelf.gripper_name){
        cout<<"此夹具已经在机器人工具位上，请不要重复更换"<<endl;
        err_message="此夹具已经在机器人工具位上，请不要重复更换!";
        return -2;
    }

    //机器人工具架有夹具则先行卸载夹具
    if(gripperSelf->RobToolSelf.is_hasGripper)
    {
        if(unload_gripper(gripperSelf->RobToolSelf.gripper_name)!=0){
            err_message="夹具卸载失败";
            cout<<"快换卸载失败"<<endl;
            return -3;
        }
    }
    if(load_gripper(gripper_name)!=0){
        err_message="夹具安装失败";
        cout<<"快换安装失败"<<endl;
        return -4;
    }

    return 0;
}

int GripperQuickChange::load_gripper(string gripper_name) {
    //检查机器人工具位是否有夹具
    if(gripperSelf->RobToolSelf.is_hasGripper){
        cout<<"机器人工具位有夹具，请先卸载当前夹具方可安装"<<endl;
        return -1;
    }
    //检查此工具在夹具架中是否具有
    if(gripperSelf->map_GripperSelf.count(gripper_name)==0){
        cout<<"map_GripperSelf中不存在"<<gripper_name<<endl;
        return -2;
    }
    if(gripperSelf->map_GripperSelf[gripper_name].is_hasGripper== false){
        cout<<"夹具架中没有这个夹具"<<gripper_name<<endl;
        return -3;
    }
    cout<<"安装夹具："<<gripper_name<<endl;
    //安装夹具
    RValueProg rValueProg_load=gripperSelf->map_GripperProgSelect[gripper_name].load;
    cout<<"prog R"<<rValueProg_load<<endl;
    if(hsc3RobotMove.RobMove_loadGripper(rValueProg_load)!=0)
    {
        return -4;
    }
    //更新机器人工具位信息
    gripperSelf->RobToolSelf.is_hasGripper= true;
    gripperSelf->RobToolSelf.gripper_name=gripper_name;
    gripperSelf->RobToolSelf.grippersort=gripperSelf->map_GripperSelf[gripper_name].grippersort;
    //更新夹具架信息
    gripperSelf->map_GripperSelf[gripper_name].is_hasGripper= false;
    //夹具重新通讯连接
    if(reConnNewGripper(gripper_name)!=0){
        return -5;
    }
    return 0;
}

int GripperQuickChange::unload_gripper(string gripper_name) {
    //机器人工具架有夹具则先行卸载夹具
    if(gripperSelf->RobToolSelf.is_hasGripper)
    {
        RValueProg rValueProg_unload=gripperSelf->map_GripperProgSelect[gripperSelf->RobToolSelf.gripper_name].unload;
        cout<<"prog R"<<rValueProg_unload<<endl;
        cout<<"卸载夹具："<<gripper_name<<endl;
        if(hsc3RobotMove.RobMove_unloadGripper(rValueProg_unload)!=0)
        {
            return -1;
        }
        //更新机器人工具位信息
        gripperSelf->RobToolSelf.is_hasGripper= false;
        //更新夹具架信息
        gripperSelf->map_GripperSelf[gripperSelf->RobToolSelf.gripper_name].is_hasGripper= true;
    } else
    {
        cout<<"机器人当前工具位上无夹具，不需卸载操作"<<endl;
        return -2;
    }
    return 0;
}

void GripperQuickChange::printInfo() {
    for (int i = 0; i <gripperSelf->map_GripperSelf.size(); ++i) {
        for (auto & iter : gripperSelf->map_GripperSelf) {
            if (iter.second.index_SelfPose == i) {
                cout << "夹具架工位号:" << "<" << iter.second.index_SelfPose << ">";
                cout << " 夹具名称:" << "<" << iter.second.gripper_name << ">";
                if (iter.second.is_hasGripper) cout << " 是否有夹具:" << "<有>"; else cout << " 是否有夹具:" << "<无>";
                cout << " 夹具类型:" << GripperSortStrings[iter.second.grippersort];
                printPoseInfo(iter.second.gripperPose);//打印夹具点位信息
                cout << endl;
            }
            continue;
        }
    }

    cout<<"机器人工具位夹爪名称:"<<"<"<<gripperSelf->RobToolSelf.gripper_name<<">";
    if(gripperSelf->RobToolSelf.is_hasGripper) cout<<" 是否有夹具:"<<"<有>";else cout<<" 是否有夹具:"<<"<无>";
    cout<<" 夹具类型:"<<"<"<<GripperSortStrings[gripperSelf->RobToolSelf.grippersort]<<">";
    cout<<endl;
}

void GripperQuickChange::printPoseInfo(GripperPose & _vv)
{
    cout<<" 夹具点位信息: ";
    for (int i = 0; i <_vv.UnloadMovePose_1.size(); ++i)
    {
        cout<<"[";
        for (int j = 0; j <6; ++j)
        {
            cout<<_vv.UnloadMovePose_1[i][j];
            if(j<5) cout<<",";
        }
        cout<<"] ";
    }

    for (int i = 0; i <_vv.UnloadMovePose_2.size(); ++i)
    {
        cout<<"[";
        for (int j = 0; j <6; ++j)
        {
            cout<<_vv.UnloadMovePose_2[i][j];
            if(j<5) cout<<",";
        }
        cout<<"] ";
    }

    for (int i = 0; i <_vv.loadMovePose_1.size(); ++i)
    {
        cout<<"[";
        for (int j = 0; j <6; ++j)
        {
            cout<<_vv.loadMovePose_1[i][j];
            if(j<5) cout<<",";
        }
        cout<<"] ";
    }

    for (int i = 0; i <_vv.loadMovePose_2.size(); ++i)
    {
        cout<<"[";
        for (int j = 0; j <6; ++j)
        {
            cout<<_vv.loadMovePose_2[i][j];
            if(j<5) cout<<",";
        }
        cout<<"] ";
    }

}

int GripperQuickChange::reConnNewGripper(string gripper_name) {
    string configPath="/home/fshs/catkin_ws/src/gripperquickchange_bridge/config/";
    switch (gripperSelf->RobToolSelf.grippersort)
    {
        case noPower:
            break;
        case robotIO_twofinger:
            gripperManage.switchGripper("RobotIOGripper",configPath+"robotIO_twofinger.yaml");
            break;
        case robotIO_fourfinger:
            gripperManage.switchGripper("RobotIOGripper",configPath+"robotIO_fourfinger.yaml");
            break;
        case serial_gripper_fivefinger:
            gripperManage.switchGripper("fiveFingerSerialGripper",configPath+"serialGripper_fivefinger.yaml");
            break;
    }
    return 0;
}

int GripperQuickChange::runOrStopRbProg(string programName,bool flag_run) {
    int ret=0;
    if(flag_run){
        ret= hsc3RobotMove.RunProg(programName);
    } else{
        ret= hsc3RobotMove.programRunQuit(programName);
    }
    return ret;
}










