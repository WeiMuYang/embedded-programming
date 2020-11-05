#ifndef IOCTRL_H
#define IOCTRL_H

#include <stdlib.h>
#include <stdio.h>
#include <QTime>
#include <QCoreApplication>
#include "compatibility.h"
#include "bdaqctrl.h"
#include "do_thr.h"
using namespace Automation::BDaq;
typedef unsigned char byte;

class IOCtrl
{
public:
    IOCtrl();
    ~IOCtrl();
    ErrorCode m_errCode_ret;
    InstantDoCtrl *m_pinstantDoCtrl;
    DeviceInformation m_devInfo;
    int32 m_startPort;
    DOThr m_DoThr_Arr[16];




    void dataOut(int port, int bit, uint8 status, double interval);
    void MySleep(double sec);
    int dataIsIn(int port, int bit);
};

#endif // IOCTRL_H
