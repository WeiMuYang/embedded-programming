#include "process_config.h"

ProcessConfig::ProcessConfig(QObject *parent) : QObject(parent)
{

}

int ProcessConfig::readConfig(QString filePath)
{
    QDir dir(filePath);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QStringList strings;//字符串列表类
    strings<<"*.conf";  //以*。*conf结尾的文件才被列出
    QFileInfoList list = dir.entryInfoList(strings,QDir::AllEntries,QDir::DirsFirst);
    QVector<QString>absolutefilename;
    for (int i = 0; i < list.size(); ++i)  {
        QFileInfo fileInfo = list.at(i);
        absolutefilename.push_back(fileInfo.filePath());//绝对路径名
    }
    int AMOUNT = absolutefilename.size();
    if(AMOUNT > 1){
        qDebug()<<"There are more than one configuration files. Please delete some files!"<<endl;
        exit(EXIT_FAILURE);
    }

    QFile readfile(absolutefilename[0]);
    if(!readfile.open(QIODevice::ReadOnly))  {
        qDebug()<<"can't open "<<endl;
        exit(EXIT_FAILURE);
    }

    QTextStream str(&readfile);
    QString line;
    line=str.readLine();
    do{
        line = str.readLine();
    }while(line.mid(60,13)!="END OF HEADER");

    while ((line = str.readLine())!="")  {

            line=str.readLine();

    }
    readfile.close();

}
