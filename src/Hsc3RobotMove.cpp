#include "Hsc3RobotMove.h"

Hsc3RobotMove::Hsc3RobotMove() {
}

Hsc3RobotMove::~Hsc3RobotMove() {

}

int Hsc3RobotMove::init() {
    int ret=0;
    //设备连接
    ret=Hsc3apiInstance::getInstance()->connect("10.10.56.214",23234);
//    ret=0;
    if(ret!=0){
        cout<<"机器人连接失败 "<<endl;
        return -1;
    } else{
        cout<<"机器人连接成功"<<endl;
    }
    //清理R寄存器的值
//    clear_Rvalue();
    if(isEnable_robot()== false){
        cout<<"机器人未上使能，请先上使能"<<endl;
        return -1;
    }
    //加载一次机器人程序
    ret = Hsc3apiInstance::getInstance()->setStartUpProject("GQC.PRG");
    if(ret!=0){
        cout<<"机器人程序启动失败"<<endl;
        return -2;
    }
    this_thread::sleep_for(chrono::seconds(2));

    return 0;
}

int Hsc3RobotMove::programRunQuit() {
    Hsc3apiInstance::getInstance()->setStopProject("GQC.PRG");
    cout<<"机器人程序退出完成"<<endl;
    return 0;
}

int Hsc3RobotMove::RobMove_unloadGripper(RValueProg setSignal) {

    //启动夹具卸载程度
    if(Hsc3apiInstance::getInstance()->setR(setSignal,1.0)!=0){
        return -1;
    }

    //等待机器人动作完成
    if(waitRSignal(22,1.0)!=0){
        return -2;
    }
    if(Hsc3apiInstance::getInstance()->setR(23,1.0)!=0){
        cout<<"setR 设置失败"<<endl;
        return -3;
    }
    if(waitRSignal(22,0.0)!=0){
        return -3;
    }
    return 0;
}

int Hsc3RobotMove::RobMove_loadGripper(RValueProg setSignal) {
    //启动夹具卸载程度
    if(Hsc3apiInstance::getInstance()->setR(setSignal,1.0)!=0){
        cout<<"setR failed"<<endl;
    }
    //等待机器人动作完成
    if(waitRSignal(22,1.0)!=0){
        return -1;
    }
    //交互进行信号复位
    if(Hsc3apiInstance::getInstance()->setR(23,1.0)!=0){
        cout<<"setR 设置失败"<<endl;
        return -2;
    }
    if(waitRSignal(22,0.0)!=0){
        return -3;
    }
        return 0;
}

int Hsc3RobotMove::comm_robActionSignal(int32_t setSignal, int32_t revSignal) {
    //改变R寄存器的值与机器人示教程序进行交互
    Hsc3apiInstance::getInstance()->setR(setSignal,1.0);
    this_thread::sleep_for(chrono::seconds(2));
    //等待某信号完成
    if(waitRSignal(revSignal,1.0)!=0){
        return -1;
    }
    return 0;
}

int Hsc3RobotMove::waitRSignal(int32_t rValue,double value) {
    bool stop= false;
    double revSignalFinish=0;
    int time_count=0;
    while (!stop)
    {
        //获取寄存器的值，确保动作已经完成
        if(Hsc3apiInstance::getInstance()->getR(rValue, revSignalFinish)!=0){
            cout<<"Hsc3apiInstance::getInstance()->getR 获取失败"<<endl;
            return -1;
        }
//        cout<<"获取R["<<rValue<<"]的值"<<revSignalFinish<<endl;
        if(revSignalFinish==value)
        {
            Hsc3apiInstance::getInstance()->setR(rValue,0.0);
            cout<<"检测到动作完成"<<endl;
            stop= true;
            break;
        }
        //超时判断
        if(time_count>200)
        {
            cout<<"机器人去到夹具卸载点失败"<<endl;
            return -1;
        }
        time_count++;
        this_thread::sleep_for(chrono::seconds(1));
    }
    return 0;
}

int Hsc3RobotMove::clear_Rvalue() {

    for (int i = 10; i <15; ++i) {
        Hsc3apiInstance::getInstance()->setR(i,0.0);
    }

    for (int j = 20; j <25; ++j) {
        Hsc3apiInstance::getInstance()->setR(j,0.0);
    }
    return 0;
}

int Hsc3RobotMove::setJR_jointpose(int8_t gpId, int32_t index, const vector<double> &data) {
    JntPos jntPos;
    jntPos.vecPos=data;
    Hsc3apiInstance::getInstance()->setJR(gpId,index,jntPos);
    return 0;
}

bool Hsc3RobotMove::isEnable_robot() {
    bool en=false;
    if(Hsc3apiInstance::getInstance()->getGpEn(0,en)!=0){
        return false;
    }
    if(en){
        cout<<"enable true"<<endl;
    } else{
        cout<<"enable false"<<endl;
    }
    return en;
}








