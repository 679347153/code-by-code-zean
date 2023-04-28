#include "mainwindow.h"
#include "ui_mainwindow.h"
void sleep(int sectime)
{
    QTime dieTime = QTime::currentTime().addMSecs(sectime);

    while (QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    username=nullptr;
//    levelGroup=nullptr;// 用于存放关卡中涉及的元素
//    deleteBtnList=nullptr;// 用于存放消除区的元素
//    modelGroup=nullptr; // 用于存放模板元素，
    Gameinit();

}
QString name;
int num;
void MainWindow::Gameinit()
{
    ui->stackedWidget->setCurrentIndex(0);//设定为第0页
    ui->lineEdit->setPlaceholderText("2 - 10 个 字");//输入名字栏
    //QString *name =new *QString(this);
    connect(ui->submit1,&QPushButton::clicked,this,[=,&name]()
    {
        name=ui->lineEdit->text();

        if(name.length()<=1||name.length()>10)
        {
            QMessageBox::critical(this,"警告","您输入的名字不符合要求，请输入2-10个字");
       }
        else ui->stackedWidget->setCurrentIndex(1);//设定为第1页
        //qDebug()<<name;
    });//确认按键

    connect(ui->one,&QPushButton::clicked,this,[=,&num](){
       //ui->one->setVisible(false);
       ui->two->setVisible(false);
       ui->there->setVisible(false);
       sleep(100);sleep(100);sleep(100);sleep(100);sleep(100);sleep(100);
       ui->stackedWidget->setCurrentIndex(2);//设定为第2页
       num=1;
       initGame();
       //ui->widget_2->setVisible(false);
       //ui->widget_3->setVisible(false);
       //QString style=btn->styleSheet();
       //btn->setStyleSheet(style+QString("image : url(:/icons/%1.png);").arg(btn->myId));//变元函数
       /*
       QString style1=ui->widget->styleSheet();
       QString style2=ui->widget_2->styleSheet();
       QString style4=ui->widget_4->styleSheet();
       qDebug()<<style1<<" "<<style2<<" "<<style4<<endl;
       for(int i=0;i<=250;i+=25)
       {
            //ui->widget_2->setStyleSheet(QString("#widget_2{background-image: url(:/res/middle1.png);background-color: rgba(255, 255, 255,%1);}").arg(i));
            //ui->widget_4->setStyleSheet(QString("#widget_4{background-image: url(:/res/front.png);background-color: rgba(255, 255, 255,%1);}").arg(i));
            // ui->widget_2->setStyleSheet(QString("#widget_2{background-image: url(:/res/middle1.png);background-color: rgba(255, 255, 255,%1);}").arg(i));
            //ui->widget->setStyleSheet(QString("#widget{")+style1+QString("background-color: rgba(255, 255, 255,%1);}").arg(i));
            //qDebug()<<ui->widget->styleSheet()<<" "<<ui->widget_2->styleSheet()<<" "<<ui->widget_3->styleSheet()<<endl;
            sleep(5);
       }
       ui->widget_2->setStyleSheet("");
       ui->widget_4->setStyleSheet("");
       ui->widget->setStyleSheet(style1);*/
    });
    connect(ui->two,&QPushButton::clicked,this,[=,&num](){
       ui->one->setVisible(false);
       //ui->two->setVisible(false);
       ui->there->setVisible(false);
       sleep(100);sleep(100);sleep(100);sleep(100);sleep(100);sleep(100);
       ui->stackedWidget->setCurrentIndex(2);//设定为第2页
       num=2;
       initGame();
    });
    connect(ui->there,&QPushButton::clicked,this,[=,&num](){
       ui->one->setVisible(false);
       ui->two->setVisible(false);
       sleep(100);sleep(100);sleep(100);sleep(100);sleep(100);sleep(100);
       ui->stackedWidget->setCurrentIndex(2);//设定为第2页
       num=3;
       initGame();
       //ui->there->setVisible(false);
    });
    /**/
}
void MainWindow::initGame()
{
    username=name;
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::addToDeleteWidget(MyButton *btn)
{
    //按照按钮的文字排序
    if(deleteBtnList->size()==0)
    {
        deleteBtnList->append(btn);
    }
    else{
        for (int i = 0; i < deleteBtnList->size(); i++)
        {
            MyButton *oldBtn = (MyButton *)deleteBtnList->at(i);
            if (oldBtn->myId == btn->myId)
            {
                deleteBtnList->insert(i,btn);
                break;
            }
            else if(i==deleteBtnList->size()-1)//截止到最后一个仍然没找到相同的块
            {
                deleteBtnList->append(btn);
                break;
            }
        }
    }

    //消除连续相同的三个按钮
    removeSame();
    //显现按钮
    int size=deleteBtnList->size();
    for(int i=0;i<size;i++){
        deleteBtnList->at(i)->setParent(ui->deleteWidget);
        deleteBtnList->at(i)->setGeometry(i*33+8,8,105,105);//待改
        deleteBtnList->at(i)->show();
    }
    //检验gameover
    if(deleteBtnList->size()==7)
    {
        //游戏失败对话框
        QDialog  dialog;
        dialog.setWindowTitle(tr("游戏失败"));
        QDialogButtonBox *button = new QDialogButtonBox(&dialog);
        //建立对话按钮
        button->addButton( "再试一次", QDialogButtonBox::YesRole);
        button->addButton( "退出游戏", QDialogButtonBox::NoRole);
        connect(button, SIGNAL(accepted()), &dialog, SLOT(accept()));
        connect(button, SIGNAL(rejected()), &dialog, SLOT(reject()));
        //信号和槽的链接
        QVBoxLayout *layout = new QVBoxLayout;//垂直布局
        layout->addWidget( button);//加入button
        dialog.setLayout(layout);//show

        //QString suffix ;
        if ( dialog.exec() == QDialog::Accepted)
        {
            initGame();
        }
        else
        {
            this->close();
        }
    }
}
//消除相同的块
void MainWindow::removeSame()
{
    int sum=1;
    for (int i = 0; i < deleteBtnList->size(); i++)
    {
        MyButton *oldBtn = (MyButton *)deleteBtnList->at(i);
        for (int j = i+1; j < deleteBtnList->size(); j++)
        {
            MyButton *nextBtn = (MyButton *)deleteBtnList->at(j);
            if(oldBtn->myId==nextBtn->myId){
                sum++;
                continue;
            }
        }
        if(sum==3)
        {
            //remove
            deleteBtnList->at(i)->setParent(NULL);
            deleteBtnList->removeAt(i);
            deleteBtnList->at(i)->setParent(NULL);
            deleteBtnList->removeAt(i);
            deleteBtnList->at(i)->setParent(NULL);
            deleteBtnList->removeAt(i);
            break;
        }
        else
        {
            sum=1;
        }
    }
}
