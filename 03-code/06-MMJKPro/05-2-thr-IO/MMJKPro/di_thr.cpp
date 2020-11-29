#include "di_thr.h"
#include  <QDebug>

/**
 * @file        di_thr.cpp
 * @brief       控制输入信号的线程
 * @authors     YangWeiBin
 * @copyright   Gmagii
 * @version     v0.0.1
 * @date        2020-11-06 12:43:48
 */

DIThr::DIThr(QThread *parent) : QThread(parent) {
    m_errCode_ret = Success;
    m_pBitOpera_fun = new BitOperator;
}

void DIThr::setDiCtrl(InstantDiCtrl *p)
{
    m_pinstantDiCtrl = p;
}

void DIThr::run()
{
    while(1){
        m_pauseTex.lock();      // 1. 给对象加锁
        dataIsIn(0);            // 2. 扫描0号端口是否有信号
        dataIsIn(1);            // 3. 扫描1号端口是否有信号
        m_pauseTex.unlock();    // 4. 给对象解锁
        msleep(100);            // 5. 暂停100毫秒，只能是整数
    }
}

/**
 * @brief 判断p端口的b位是否有数据输入（间隔0.5秒）
 * @param port 端口号，行号 ：  0 1
 * @param bit  位置号： 0 1 2 3 4 5 6 7
 * @return 0：未通电   1： 通电
 */
void DIThr::dataIsIn(int port){
    uint8 data = 0;
    m_uc_inData = 0;
    for(int i = 0; i < 8; ++i){
        m_errCode_ret = m_pinstantDiCtrl->ReadBit(int32(port), int32(i), &data);
        m_pBitOpera_fun->changeBytePosBit(&m_uc_inData, i, int(data));
    }
    m_pBitOpera_fun->separatePrintBit(m_uc_inData);
    if(m_uc_inData != 0x00)
        emit sendInputData(port, m_uc_inData);

   // CHK_RESULT(m_errCode_ret);  error 20201105
   //  qDebug() << data;
}



void DIThr::ThreadPause() {
    m_pauseTex.lock();      // 1. 给对象加锁
}
void DIThr::ThreadContinue() {
    m_pauseTex.unlock();    // 2. 给对象解锁
}


