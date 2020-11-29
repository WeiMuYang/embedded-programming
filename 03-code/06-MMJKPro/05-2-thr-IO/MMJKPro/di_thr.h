#ifndef DITHR_H
#define DITHR_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "compatibility.h"
#include "bdaqctrl.h"
#include "bit_operator.h"
using namespace Automation::BDaq;
typedef unsigned char byte;


class DIThr : public QThread
{
    Q_OBJECT
public:
    explicit DIThr(QThread *parent = 0);
    void setDiCtrl(InstantDiCtrl *p);
    void dataIsIn(int port);
    void ThreadPause();
    void ThreadContinue();

signals:
    void sendInputData(int port, unsigned char inData);

protected:
    void run();

private:
    int m_n_port;
    unsigned char m_uc_inData;
    ErrorCode m_errCode_ret;
    InstantDiCtrl *m_pinstantDiCtrl;
    QMutex m_pauseTex;
    BitOperator *m_pBitOpera_fun;

    // 如果有输入信号控制的设备，可以在里面继续添加其他设备对象
};

#endif // DITHR_H
