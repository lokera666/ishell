﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScreen>
#include <QFile>
#include <QPushButton>
#include "webconsole.h"
#include "db/dbutil.h"
#include <QDir>
#include <QLabel>

MainWindow* mainwindow=NULL;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("ishell");
    setWindowIcon(QIcon(":/logo.png"));
    QList<QScreen *> list_screen =  QGuiApplication::screens();  //多显示器
    QRect rect = list_screen.at(0)->geometry();
    setMinimumSize(800, 600);

    QDir dir;
    dir.mkdir(Common::workspacePath);

//    int desktop_width = rect.width();
//    int desktop_height = rect.height();
//    setMaximumSize(desktop_width-200, desktop_height-200);
//    rect=QRect(200,200,desktop_width-200,desktop_height-200);
//    setGeometry(rect);
//    qDebug() << desktop_width <<desktop_height;

    DBUtil::GetInstance()->init();

    QTimer::singleShot(100,this,[=](){
        initUI();
        initWebSocketServer();
    });
    connectInfo=new QLabel;
    connectInfo->setText("项目开源地址：https://github.com/yz-java/ishell    Email:yangzhaojava@gmail.com");
    connectInfo->setAlignment(Qt::AlignCenter);
    connectInfo->setTextInteractionFlags(Qt::TextSelectableByMouse);

    ui->statusbar->addWidget(connectInfo);
}

void MainWindow::initUI(){
    connectManagerUI=new ConnectManagerUI(this);

    ui->tabWidget->setTabPosition(QTabWidget::North);
    ui->tabWidget->insertTab(0,new QWidget(this), QIcon(":/icons/manager.png"),"连接管理");
    ui->tabWidget->setTabToolTip(0,"连接管理");
    webView = new QWebEngineView(this);
    QFile file(":/html/README.html");
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QString htmlData = file.readAll().constData();
    webView->setHtml(htmlData);
    webView->show();

    ui->tabWidget->setUsesScrollButtons(true);
    ui->tabWidget->insertTab(1,webView,QIcon(":/icons/welcome.png"),"欢迎页");
    ui->tabWidget->setTabToolTip(1,"欢迎页");
    this->ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->setCurrentIndex(1);
    mainwindow=this;
    STATUS_BAR_HIGHT=ui->statusbar->height();
    connect(connectManagerUI,SIGNAL(openSSHConnect(ConnectInfo)),this,SLOT(openSSHConnect(ConnectInfo)));
}

void MainWindow::initWebSocketServer(){
    webSocketServer=new WebSocketServer;
    connect(webSocketServer,&WebSocketServer::errorMsg,this,[=](const QString& msg){
        QMessageBox::warning(this,tr("警告"),msg);
        QApplication::quit();
    });
    webSocketServer->run();

}

void MainWindow::resizeEvent(QResizeEvent *)
{
    this->ui->tabWidget->resize(this->size());
    connectInfo->setMinimumWidth(this->width());
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_tabWidget_currentChanged(int index)
{

    qDebug() << index;
    if(index==0){
        this->ui->tabWidget->setCurrentIndex(currentIndex);
        if(!connectManagerUI->isActiveWindow()){
            connectManagerUI->show();
        }
        return;
    }
    currentIndex=index;

}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    qDebug() << "close ==> " << index;
    if(index==0){
        return;
    }
    WebConsole* console = (WebConsole*)ui->tabWidget->widget(index);
    console->close();
    ui->tabWidget->removeTab(index);

    if(ui->tabWidget->count()==1){
        ui->tabWidget->insertTab(1,webView,QIcon(":/icons/welcome.png"),"欢迎页");
        ui->tabWidget->setCurrentIndex(1);
    }

}

void MainWindow::openSSHConnect(ConnectInfo connectInfo){
    int count=ui->tabWidget->count();
    ui->tabWidget->insertTab(count,new WebConsole(this,&connectInfo),QIcon(":/icons/console.png"),connectInfo.name);
    ui->tabWidget->setTabToolTip(count,connectInfo.name);
    ui->tabWidget->setCurrentIndex(count);
}
