#include "io_ctrl.h"



#define  deviceDescription  L"USB-5830,BID#0"
const wchar_t* profilePath = L"";

/**
 * @brief 实现设备的初始化和线程的初始化
 * @param parent：基类指针
 */
IOCtrl::IOCtrl(QObject *parent) : QObject(parent)
{
    // 1. 初始化设备和属性成员
    m_errCode_ret = Success;
    m_pinstantDoCtrl = InstantDoCtrl::Create();
    m_pinstantDiCtrl = InstantDiCtrl::Create();
    DeviceInformation devInfo(deviceDescription);
    m_errCode_ret = m_pinstantDoCtrl->setSelectedDevice(devInfo);
    if(BioFailed(m_errCode_ret))
        printf("ret error in IOCtrl() 001!\n");
    m_errCode_ret = m_pinstantDiCtrl->setSelectedDevice(devInfo);
    if(BioFailed(m_errCode_ret))
        printf("ret error in IOCtrl() 002!\n");
    m_errCode_ret = m_pinstantDoCtrl->LoadProfile(profilePath);//Loads a profile to initialize the device.
    if(BioFailed(m_errCode_ret))
        printf("ret error in IOCtrl() 003!\n");
    m_errCode_ret = m_pinstantDiCtrl->LoadProfile(profilePath);
    if(BioFailed(m_errCode_ret))
        printf("ret error in IOCtrl() 004!\n");
    m_pBitOpera_fun = new BitOperator;

    // 2. 信号槽建立连接
    connect(&m_DiThr, SIGNAL(sendInputData(int,unsigned char)), this,SLOT(ReceiveInputData(int,unsigned char)));

    // 3. 所有灯刷新，全部关闭
    byte  bufferForWriting[64] = {0};
    bufferForWriting[0] = 0x0;
    m_pinstantDoCtrl->Write(0, 1,bufferForWriting);
    m_pinstantDoCtrl->Write(1, 1,bufferForWriting);

    // 4. 对监控输入的线程添加输入设备对象
    m_DiThr.setDiCtrl(m_pinstantDiCtrl);

    // 5. 启动输入监控线程
    m_DiThr.start();
}


/**
 * @brief 接收输入信号的槽函数
 * @param index:信号输入的编号 [0,15]
 * @param stats:信号的状态    [0, 1] 无输入， 有输入
 */
void IOCtrl::ReceiveInputData(int port, unsigned char inData)
{
    qDebug() << port;
    m_pBitOpera_fun->separatePrintBit(inData);
}


/**
 * @brief 以interval秒的间隔对p端口b位进行信号的输入
 * @param port：端口号    行号 ： 0 1
 * @param bit： 位置号    列号 ： 0 1 2 3 4 5 6 7
 * @param status： 灯的需要变的状态
 * @param interval： 间隔时间
 */
void IOCtrl::dataOut(uint8 status, int port,int bit,double interval){
    if(0 == interval)   // 控制常亮，或者常灭
        m_errCode_ret = m_pinstantDoCtrl->WriteBit(port, (byte)bit, status);
    else{               // 控制闪烁一下，用时 interval * 2 秒
        m_errCode_ret = m_pinstantDoCtrl->WriteBit(port, (byte)bit, 1);
        MySleep(interval);
        m_errCode_ret = m_pinstantDoCtrl->WriteBit(port, (byte)bit, 0);
        MySleep(interval);
    }
    CHK_RESULT(m_errCode_ret);
}


/**
 * @brief 析构函数，释放空间和设备
 */
IOCtrl::~IOCtrl()
{
    // Close device and release any allocated resource.
    m_pinstantDoCtrl->Dispose();
    m_pinstantDiCtrl->Dispose();

    // If something wrong in this execution, print the error code on screen for tracking.
    if(BioFailed(m_errCode_ret)) {
        wchar_t enumString[256];
        AdxEnumToString(L"ErrorCode", (int32)m_errCode_ret, 256, enumString);
        printf("Some error occurred. And the last error code is 0x%X. [%ls]\n", m_errCode_ret, enumString);
    }
}

void IOCtrl::MySleep(double sec){
    int msec = sec * 1000;
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

