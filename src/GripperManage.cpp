#include "GripperManage.h"

GripperManage::GripperManage() {
    gripperPtr = new Gripper();
}

GripperManage::~GripperManage() {
    delete gripperPtr;
    gripperPtr = nullptr;
}

int GripperManage::switchGripper(string generatorName, std::string configFile) {
    if(gripperPtr->setGripper(generatorName, "0" , configFile)!=0){
        cout<<"switchGripper ret -1"<<endl;
        return -1;
    }
    if(gripperPtr->connectGripper()!=0){
        cout<<"switchGripper ret -2"<<endl;
        return -2;
    }
    return 0;
}

int GripperManage::connectGripper() {
    int ret = gripperPtr->connectGripper();
    return ret;
}

int GripperManage::openGripper() {
    int ret = gripperPtr->openGripper();
    return ret;
}

int GripperManage::closeGripper() {
    int ret = gripperPtr->closeGripper();
    return ret;
}
