#include "GripperSelf.h"

GripperSelf::GripperSelf() {
    map_GripperSort.insert(pair<string,GripperSort>("robotIO_fourfinger",robotIO_fourfinger));
    map_GripperSort.insert(pair<string,GripperSort>("noPower",noPower));
    map_GripperSort.insert(pair<string,GripperSort>("robotIO_twofinger",robotIO_twofinger));
    map_GripperSort.insert(pair<string,GripperSort>("serial_gripper_fivefinger",serial_gripper_fivefinger));
}

GripperSelf::~GripperSelf() {
    map<string,Gripperinfo>().swap(map_GripperSelf);
    malloc_trim(0);
}

int GripperSelf::setRobToolSelf(Gripperinfo gripperinfo){
    RobToolSelf=std::move(gripperinfo);
}

int GripperSelf::setGripperSelf(string gripper_name,Gripperinfo gripperinfo){
    map_GripperSelf.insert(pair<string,Gripperinfo>(gripper_name,gripperinfo));
    return 0;
}

int GripperSelf::setGripperProgSelect(string gripper_name,ProgSelect progSelect){
    map_GripperProgSelect[gripper_name]=progSelect;
    return 0;
}



