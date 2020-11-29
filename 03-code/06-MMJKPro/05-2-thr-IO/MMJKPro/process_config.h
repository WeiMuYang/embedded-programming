#ifndef PROCESSCONFIG_H
#define PROCESSCONFIG_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <QDir>

class ProcessConfig : public QObject
{
    Q_OBJECT
public:
    explicit ProcessConfig(QObject *parent = 0);
    int readConfig(QString filePath);
signals:

public slots:

private:
    QString m_str_MMJKIP;
    QString m_str_MMJKPort;
    QString m_str_QDIP;
    QString m_str_QDPort;
};

#endif // PROCESSCONFIG_H
