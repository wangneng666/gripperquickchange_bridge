#ifndef CPP_WORK_HSC3ROBOTMOVE_H
#define CPP_WORK_HSC3ROBOTMOVE_H

#include "Hsc3apiInstance.h"

enum RValueProg{
    shelfP0_UnloadGripper=10,
    shelfP1_UnloadGripper,
    shelfP2_UnloadGripper,
    shelfP3_UnloadGripper,
    shelfP0_loadGripper=15,
    shelfP1_loadGripper,
    shelfP2_loadGripper,
    shelfP3_loadGripper,
};

enum RValue{
    UnloadMove1Finish=20,
    UnloadActionFinish,
    UnloadMove2Finish,
    LoadMove1Finish,
    LoadActionFinish,
    LoadMove2Finish,
};

class Hsc3RobotMove {
public:
    Hsc3RobotMove();
    ~Hsc3RobotMove();

private:
    int clear_Rvalue();

public:

    /***
     * 初始化配置
     * @return
     */
    int init();

    /***
     * 退出正在运行的程序
     * @return
     */
    int programRunQuit();

    /***
     * 机器人卸载夹具
     * @return
     */
    int RobMove_unloadGripper(RValueProg setSignal);

    /***
     * 机器人安装夹具
     * @return
     */
    int RobMove_loadGripper(RValueProg setSignal);

    /***
     * 夹具连接器动作完成通知
     * @return
     */
    int notify_GripperConnector();

    bool isNeed_GripperConnector();

    int comm_robActionSignal(int32_t setSignal,int32_t revSignal);

    int waitRSignal(int32_t rValue,double value);

    int setJR_jointpose(int8_t gpId, int32_t index, const vector<double > & data);

};


#endif //CPP_WORK_HSC3ROBOTMOVE_H
