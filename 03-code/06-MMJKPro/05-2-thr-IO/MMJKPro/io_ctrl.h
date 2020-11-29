#ifndef IOCTRL_H
#define IOCTRL_H

#include <stdlib.h>
#include <stdio.h>
#include <QTime>
#include <QObject>
#include <QCoreApplication>
#include "compatibility.h"
#include "bdaqctrl.h"
#include "di_thr.h"
#include "bit_operator.h"
using namespace Automation::BDaq;
typedef unsigned char byte;

class IOCtrl : public QObject
{
    Q_OBJECT
public:
    explicit IOCtrl(QObject *parent = 0);
    ~IOCtrl();


    void MySleep(double sec);
    void dataOut(uint8 status, int port,int bit,double interval);

public slots:
    void ReceiveInputData(int port, unsigned char inData);

private:
    ErrorCode m_errCode_ret;            // 操作错误信号
    DeviceInformation m_devInfo;        // 设备描述
    InstantDoCtrl *m_pinstantDoCtrl;    // 输出设备对象
    InstantDiCtrl *m_pinstantDiCtrl;    // 输入设备对象
    DIThr m_DiThr;                      // 输入端口监控线程
    BitOperator* m_pBitOpera_fun;       // 位运算的函数
};

#endif // IOCTRL_H
