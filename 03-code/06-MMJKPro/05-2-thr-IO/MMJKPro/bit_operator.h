#ifndef BINARYBITOPERATOR_H
#define BINARYBITOPERATOR_H

/**
 * @file        binary_bit_operator.h
 * @brief       二进制位运算
 * @authors     YangWeiBin
 * @copyright   Gmagii
 * @version     v0.0.1
 * @date        2020-11-06 12:54:49
 */


#include <QObject>
#include <QDebug>

class BitOperator : public QObject
{
    Q_OBJECT
public:
    explicit BitOperator(QObject *parent = 0);
    void getBytePosBit(unsigned char byteData, int pos, int *pbit);
    void separatePrintBit(unsigned char byteData);
    void changeBytePosBit(unsigned char *pByteData, int pos, int bit);

signals:

public slots:
};

#endif // BINARYBITOPERATOR_H
