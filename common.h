#ifndef COMMON_H
#define COMMON_H
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <QtConcurrent>
#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

extern int STATUS_BAR_HIGHT;

class Common
{
public:
    Common();
    static QString workspacePath;
};

void test();

void test1();

#endif // COMMON_H
