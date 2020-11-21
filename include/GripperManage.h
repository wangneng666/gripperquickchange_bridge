#ifndef GRIPPERQUICKCHANGE_BRIDGE_GRIPPERMANAGE_H
#define GRIPPERQUICKCHANGE_BRIDGE_GRIPPERMANAGE_H
#include "hirop/gripper/execute.h"
using namespace hirop_gripper;

class GripperManage {
public:
    GripperManage();
    ~GripperManage();
private:
    Gripper* gripperPtr;
public:
    /***
     * 夹具切换
     * @param generatorName
     * @param configFile
     * @return
     */
    int switchGripper(string generatorName, std::string configFile);

    /***
     * 夹具连接
     * @return
     */
    int connectGripper();

    /**
     * @brief openGripper 打开夹爪
     * @return
     */
    int openGripper();

    /**
     * @brief CloseGripper 关闭夹爪
     * @return
     */
    int closeGripper();





};


#endif //GRIPPERQUICKCHANGE_BRIDGE_GRIPPERMANAGE_H
