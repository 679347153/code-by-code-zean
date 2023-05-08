#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma execution_character_set("utf-8") // qt支持显示中文
//添加头文件
#include<QMessageBox>
#include <QMainWindow>
#include "mybutton.h"
#include <QButtonGroup>
#include<QDebug>
#include<QTime>
#include<QCoreApplication>
//文件相关
#include <QDateTime>
#include <QImage>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDir>
#include <QDialogButtonBox>
#include <QMetaObject>
#include <QEventLoop>

#include <stdlib.h>
#include <QDialog>//对话框
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QGridLayout>  //布局
//音乐头文件
#include<QSound>
//网络协议相关
#include<QHttpMultiPart>
#include <QNetworkAccessManager>
#include<QNetworkReply>
//数据库相关
#include <QSqlDatabase> //数据库
#include "database.h"
//Tcp协议
#include<QTcpSocket>  //通讯套接字
#include<QHostAddress>
//#include <QtGlobal>
//类似c++创建namespace

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QNetworkAccessManager;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void Init();
    void Gameinit();//初始化游戏
    void initGame();//初始化游戏界面
    QButtonGroup *modelGroup;          // 用于存放模板元素，
    QButtonGroup *levelGroup;          // 用于存放关卡中涉及的元素
    QList<MyButton*> *deleteBtnList;   // 用于存放消除区的元素

    //添加到下面消除窗口
    void addToDeleteWidget(MyButton *btn);
    //消除栏窗口链接
    void disconnetbutton(MyButton *btn);
    void connectbutton(MyButton *btn);
    // 销毁卡片
    void removeSame();

    // 加载模版图案
    void load_element();

    // 将关卡元素随机分配到空间中
    void distribution_element(int level = 0); // level代表关卡数量

    //判断是否可以点击
    bool isClickable(MyButton *btn);

    //判断按钮状态并设置图片

    void setPictureByStatus(MyButton *btn);
    //设置邻近的按钮的状态

    void setSideBtn(QPoint current_btn_point);
/**/
    ~MainWindow();
/**/
    void Get();//获取服务器数据库数据
    void Post(QString username,int score);//向服务器推送数据
//自定义槽函数
public slots:
    //添加到消除栏槽函数
    void addToDeleteSlot(QAbstractButton *);
    //从服务器读取数据库中的信息
    void slotReadyRead();
    //将数据推送到服务器
    void slotgetpost();
    //连接错误报错
    void slotError(QNetworkReply::NetworkError code);
    //int receiveHttpPostReply(QNetworkReply* reply);
    //void highlightit(MyButton*);
private:
    QString username;//用户名
    int score;//分数
    Ui::MainWindow *ui;
    QSound *p_button;
    QSound *c_button;
    QTcpSocket *tcpSocket;        //Tcp协议通讯套接字
    QNetworkAccessManager *m_pNetworkAccessManager =nullptr;     //HTTP协议
};
#endif // MAINWINDOW_H
