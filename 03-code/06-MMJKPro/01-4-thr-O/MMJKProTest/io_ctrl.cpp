#include "io_ctrl.h"



#define  deviceDescription  L"USB-5830,BID#0"
const wchar_t* profilePath = L"";

IOCtrl::IOCtrl()
{
    m_errCode_ret = Success;
    m_pinstantDoCtrl = InstantDoCtrl::Create();
    DeviceInformation devInfo(deviceDescription);
    m_errCode_ret = m_pinstantDoCtrl->setSelectedDevice(devInfo);
    if(BioFailed(m_errCode_ret))
        printf("ret error in IOCtrl() 001!\n");
    m_errCode_ret = m_pinstantDoCtrl->LoadProfile(profilePath);//Loads a profile to initialize the device.
    if(BioFailed(m_errCode_ret))
        printf("ret error in IOCtrl() 002!\n");
    m_startPort = 0;

    for(int i = 0; i < 8; ++i){
        m_DoThr_Arr[i].setData(m_pinstantDoCtrl,0,i);
        m_DoThr_Arr[i+8].setData(m_pinstantDoCtrl,1,i);
    }
    m_DoThr_Arr[0].setType(0);
    m_DoThr_Arr[1].setType(1);
    m_DoThr_Arr[0].start();
    m_DoThr_Arr[0].ThreadContinue();
    m_DoThr_Arr[1].start();
    m_DoThr_Arr[1].ThreadContinue();
    m_DoThr_Arr[2].start();
    m_DoThr_Arr[2].ThreadContinue();
    m_DoThr_Arr[3].start();
    m_DoThr_Arr[3].ThreadContinue();


}

/**
 * @brief ��interval��ļ����p�˿�bλ�����źŵ�����
 * @param port���˿ں�    �к� �� 0 1
 * @param bit�� λ�ú�    �к� �� 0 1 2 3 4 5 6 7
 * @param status�� �Ƶ���Ҫ���״̬
 * @param interval�� ���ʱ��
 */
void IOCtrl::dataOut(int port, int bit, uint8 status, double interval){
    if(0 == interval)
        m_errCode_ret = m_pinstantDoCtrl->WriteBit(port, (byte)bit, status);
    else{
        do{
            m_errCode_ret = m_pinstantDoCtrl->WriteBit(port, (byte)bit, 1);
            MySleep(interval);
            m_errCode_ret = m_pinstantDoCtrl->WriteBit(port, (byte)bit, 0);
            MySleep(interval);
        }while(1);
    }
    CHK_RESULT(m_errCode_ret);
}

/**
 * @brief �ж�p�˿ڵ�bλ�Ƿ����������루���0.5�룩
 * @param port �˿ںţ��к� ��  0 1
 * @param bit  λ�úţ� 0 1 2 3 4 5 6 7
 * @return 0��δͨ��   1�� ͨ��
 */
int IOCtrl::dataIsIn(int port, int bit){
    uint8 data = 0;
    do{
        m_errCode_ret = m_pinstantDoCtrl->ReadBit(port, bit, &data);
        CHK_RESULT_R(m_errCode_ret);
        if(data == 0x00)
            return 0;
        else
            return 1;
        SLEEP(0.5);
    }while(!kbhit());
}



void IOCtrl::MySleep(double sec){
    int msec = sec * 1000;
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

IOCtrl::~IOCtrl()
{
    // Close device and release any allocated resource.
    m_pinstantDoCtrl->Dispose();

    // If something wrong in this execution, print the error code on screen for tracking.
    if(BioFailed(m_errCode_ret)) {
        wchar_t enumString[256];
        AdxEnumToString(L"ErrorCode", (int32)m_errCode_ret, 256, enumString);
        printf("Some error occurred. And the last error code is 0x%X. [%ls]\n", m_errCode_ret, enumString);
    }
}

