#include "mybutton.h"
MyButton::MyButton(QWidget *parent)
{
    this->setParent(parent);
    this->setMaximumSize(105,105);//修改按钮大小
    this->setMinimumSize(105,105);
    // 初始化上面和下面按钮的链表
}

MyButton *MyButton::copyMyButton()
{
   MyButton * res = new MyButton();
   //在初始化中，仅仅增加了Text，
   res->setText(this->text());
   res->myId1=this->myId1;
   res->myId2=this->myId2;
   res->Id=this->Id;
   //待修改
   res->setStyleSheet(this->styleSheet());
   return res;
}
