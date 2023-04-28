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
    QList<MyButton> *buttomBtnList;
    QList<MyButton> *topBtnList;
    QString myId;//指征它是那种色块
};

#endif // MYBUTTON_H
