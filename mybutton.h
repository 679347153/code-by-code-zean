#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QObject>
#include <QPushButton>

//自定义按钮格式
class MyButton:public QPushButton
{
    Q_OBJECT
public:
    MyButton(QWidget *parent = nullptr);
    MyButton * copyMyButton();

public:
    int Id;//总类型编号
    int indeId;  //独立唯一编号
    QString myId1;//指征它是那种色块
    QString myId2;//指征他是该种色块的第几个
};
#endif // MYBUTTON_H
