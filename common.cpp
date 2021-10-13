#include "common.h"
#include <QStandardPaths>
int STATUS_BAR_HIGHT=0;

QString homePath=QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
QString Common::workspacePath=homePath+"/.ishell/";
Common::Common()
{

}

void test(){
    qDebug() << "dll000 万能函数调用";
}

void test1(){
    qDebug() << "dll1111 万能函数调用";
}
