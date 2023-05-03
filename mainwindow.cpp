#include "mainwindow.h"
#include "ui_mainwindow.h"
void sleep(int sectime)//非阻塞延时函数
{
    QTime dieTime = QTime::currentTime().addMSecs(sectime);

    while (QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);//最大100ms
    }
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
//    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
//        db.setHostName("eat.mysql.database.azure.com");  //连接远程主机
//        db.setPort(3306);
//        db.setDatabaseName("0nlinetek-eat-database");
//        db.setUserName("suyrbmymqs");
//        db.setPassword("1QW6E7P7YUNRMVA5");
//        bool ok = db.open();
//        if (ok){
//            QMessageBox::information(this, "infor", "link success");
//        }
//        else {
//            QMessageBox::information(this, "infor", "link failed");
//            //qDebug()<<"error open database because"<<db.lastError().text();
//        }
    username=nullptr;
    score=0;
    levelGroup=nullptr;// 用于存放关卡中涉及的元素
    deleteBtnList=nullptr;// 用于存放消除区的元素
    modelGroup=nullptr; // 用于存放模板元素，
    //QString runpath=QCoreApplication::applicationDirPath();
    //QString musicpath=runpath;
    QSound *startSound=new QSound(":/res/backgroundmusic.wav",this);
    startSound->play();
    connect(ui->goback,&QPushButton::clicked,this,[=]()
    {
        ui->one->setVisible(true);
        ui->two->setVisible(true);
        ui->there->setVisible(true);
        ui->stackedWidget->setCurrentIndex(1);//设定为第1页
    });
    Gameinit();
}
QString name;
int num=0;
MyButton *highlight;
bool flag=0;
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
    });
    ui->label->setAlignment(Qt::AlignHCenter);

}
void MainWindow::initGame()
{
    username=name;
    score=0;
    flag=0;
    highlight=nullptr;
    ui->label->setText(QString::number(score));
    //预处理两组按钮组（链表形式储存）
    //清空函数链表
    if(levelGroup!=nullptr)
    {
        int listSize=levelGroup->buttons().size();
        for(int i=0;i<listSize;i++)
        {
            auto btn=levelGroup->buttons().at(0);
            levelGroup->buttons().at(0)->setParent(NULL);
            levelGroup->removeButton(btn);
        }
    }
    if(deleteBtnList!=nullptr)
    {
        int listSize=deleteBtnList->size();
        for(int i=0;i<listSize;i++)
        {
            deleteBtnList->at(0)->setParent(NULL);
            deleteBtnList->removeAt(0);
        }
    }
    //消除区列表
    deleteBtnList=new QList<MyButton *>();

    //初始化随机数生成器，设置种子
    //init_randomGenerator();

    //加载模版图案
    //创建10个模版按钮
    modelGroup = new QButtonGroup();

    this->load_element();

    // 随机生成关卡中需要的元素
    levelGroup = new QButtonGroup();
    //初始化第num关
    distribution_element(num);
    //根据是否可点击设置颜色
    for (int i = 0 ; i < levelGroup->buttons().size(); i ++)
    {

        MyButton *btn=(MyButton *)levelGroup->buttons().at(i);
        setPictureByStatus(btn);
    }

}
bool MainWindow::isClickable(MyButton *btn)
{
    QPoint left_below(btn->pos().rx(),btn->pos().ry() + 100);
    MyButton * tmp_btn = (MyButton *) ui->goodsWidget->childAt(left_below);
    if (tmp_btn != btn)
    {
        return false;
    }

    QPoint left_top(btn->pos().rx(),btn->pos().ry());
    tmp_btn = (MyButton *) ui->goodsWidget->childAt(left_top);
    if (tmp_btn != btn)
    {
        return false;
    }

    QPoint rigth_top(btn->pos().rx() + 100,btn->pos().ry());
    tmp_btn = (MyButton *) ui->goodsWidget->childAt(rigth_top);
    if (tmp_btn != btn)
    {
        return false;
    }
    QPoint rigth_below(btn->pos().rx() + 100,btn->pos().ry() + 100);
    tmp_btn = (MyButton *) ui->goodsWidget->childAt(rigth_below);
    if (tmp_btn != btn)
    {
        return false;
    }
    return true;
}


void MainWindow::setPictureByStatus(MyButton *btn)
{
    bool btnStatus=isClickable(btn);
    if(btnStatus)
    {
        QString style=btn->styleSheet();
        btn->setStyleSheet(style+QString("image : url(:/icon/%1-%2.png);").arg(btn->myId1).arg(btn->myId2));//变元函数
    }
    else
    {

        QString style=btn->styleSheet();
        btn->setStyleSheet(style+QString("image : url(:/icon/%1-%2grey.png);").arg(btn->myId1).arg(btn->myId2));
    }
}

void MainWindow::setSideBtn(QPoint current_btn_point)
{//重新计算是否被遮挡

    MyButton * tmp_btn = (MyButton *) ui->goodsWidget->childAt(current_btn_point);
    if(tmp_btn)
    {
        setPictureByStatus(tmp_btn);
    }

    QPoint left_below(current_btn_point.rx(),current_btn_point.ry() + 100);
    tmp_btn = (MyButton *) ui->goodsWidget->childAt(left_below);
    if(tmp_btn)
    {
        setPictureByStatus(tmp_btn);
    }

    QPoint rigth_top(current_btn_point.rx() + 100,current_btn_point.ry());
    tmp_btn = (MyButton *) ui->goodsWidget->childAt(rigth_top);
    if(tmp_btn)
    {
        setPictureByStatus(tmp_btn);
    }
    QPoint rigth_below(current_btn_point.rx() + 100,current_btn_point.ry() + 100);
    tmp_btn = (MyButton *) ui->goodsWidget->childAt(rigth_below);
    if(tmp_btn)
    {
        setPictureByStatus(tmp_btn);
    }
}
void MainWindow::load_element()
{
    //加载模版图案
    //创建20个模版按钮
    QFile json_file(":/json/object.json");  //选择关卡配置文件。
    json_file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString value = json_file.readAll();
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8(),&error);
    json_file.close();
    QJsonObject jsonObject=document.object();//抓取json对象
    QJsonArray array=jsonObject["node"].toArray();//抓取并转化为数组
    for(int i=0; i<20; i++)
    {
        MyButton *btn = new MyButton(NULL);
        QJsonValue child =array.at(i);
        QJsonObject object=child.toObject();
        QList<QString> tmp_list =object.value((QString::number(i))).toString().split(",");
        QString tmp_x = tmp_list.at(0);
        QString tmp_y = tmp_list.at(1);
        btn->Id=i;
        btn->myId1=tmp_x;
        btn->myId2=tmp_y;
        //qDebug()<<tmp_x<<" "<<tmp_y<<endl;
        /**/btn->setStyleSheet(QString("border-image: url(:/icon/%1-%2.png);"
                                    "background-image: url(:/res/none.png);"
                                   "border-radius:15px;").arg(tmp_x).arg(tmp_y));//圆角
       /* "border-top: 1px solid green;"
        "border-left: 1px solid green;"
        "border-right: 1px solid green;"
        "border-bottom: 5px solid green;" */
        //更改底层按钮样式
        this->modelGroup->addButton(btn);
    }
}
void MainWindow::distribution_element(int level)
{
    //将关卡元素随机分配到空间中
    //并显示到parent上

    // 读取每个元素的位置
    //QString path=QDir::currentPath();
    //打开json文件
    QFile json_file(QString(":/json/level%1.json").arg(level));  //选择关卡配置文件。
    json_file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString value = json_file.readAll();
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8(),&error);
    json_file.close();
    QJsonObject jsonObject=document.object();//抓取json对象
    QJsonArray array=jsonObject["node"].toArray();//抓取并转化为数组
    QJsonArray arrayobject=jsonObject["objectx"].toArray();//抓取并转化为数组
    int max_element = array.size();
    for(int i=0; i < max_element; i ++)
    {
        QJsonValue child =arrayobject.at(i);
        QJsonObject object=child.toObject();
        QString V =object.value((QString::number(i))).toString();
        //qDebug()<<V<<endl;
        MyButton *btn=((MyButton *) (this->modelGroup->buttons().at(V.toInt())))->copyMyButton();
        btn->indeId=i;
        this->levelGroup->addButton(btn);
    }

    //将游戏区按键信号与槽链接
    connect(levelGroup,SIGNAL(buttonClicked(QAbstractButton *)),this,SLOT(addToDeleteSlot(QAbstractButton *)));

    // 打乱levelGroup的顺序
    QVector<int> *placement_num = new QVector<int>();     //顺序编号
    QVector<int> *placement_state = new QVector<int>();   //levelGroup中每个button放置的位置
    for(int i = 0 ; i < max_element ; i ++){
        placement_num->append(i);
    }
    for(int i = 0; i < max_element ;i ++){
        int luck_index = rand()%placement_num->size();//generator.bounded(0,placement_num->size());
        placement_state->append(placement_num->at(luck_index));
        placement_num->removeAt(luck_index);
    }
    //qDebug()<<max_element<<endl;
    //  循环读取每个位置的图案
    for (int i = 0 ; i < max_element; i ++)
    {
        QJsonValue child =array.at(i);
        QJsonObject object=child.toObject();

        QList<QString> tmp_list =object.value((QString::number(i))).toString().split(",");
        //qDebug()<<tmp_list.at(0)<<" "<<tmp_list.at(1)<<endl;
        int tmp_x = tmp_list.at(0).toInt();//+53
        int tmp_y = tmp_list.at(1).toInt()+200;
        this->levelGroup->buttons().at(placement_state->at(i))->setParent(ui->goodsWidget);
        this->levelGroup->buttons().at(placement_state->at(i))->setGeometry(tmp_x,tmp_y,105,105);//以x,y为坐标展示一个w,h的控件
        this->levelGroup->buttons().at(placement_state->at(i))->show();
    }
}
void MainWindow::addToDeleteSlot(QAbstractButton *aBtn)
{
    MyButton *btn=(MyButton *)(aBtn);
    // 首先判断button是否为最外层的元素, 通过判断元素四个顶角是否为最表层元素
    bool ret=isClickable(btn);
    if(!ret)
        return;

    // 从levelGroup中移除btn，并加入到deleteWidget。
    QPoint current_btn_point = btn->pos();
    levelGroup->removeButton(btn);
    if(btn->Id==0&&flag==1)//万能卡消除单个卡
    {
        flag=0;
        highlight->setParent(NULL);
        btn->setParent(NULL);
        disconnect(highlight,0,0,0);
        deleteBtnList->removeOne(highlight);
        score+=2;
        ui->label->setText(QString::number(score));
    /*刷新delete栏*/
    int size=deleteBtnList->size();
    for(int i=0;i<size;i++){
        deleteBtnList->at(i)->setParent(ui->deleteWidget);
        deleteBtnList->at(i)->setGeometry(i*105+37,25,105,105);//规定色块位置
        deleteBtnList->at(i)->show();
    }

    }
    else
    {
        btn->setParent(ui->deleteWidget);
        ui->deleteWidget->show();
        addToDeleteWidget((MyButton*)btn);
    }
    //重新计算是否可以被点击及样式
    setSideBtn(current_btn_point);
}
void MainWindow::connectbutton(MyButton *btn)//设置高亮
{
    connect(btn,&MyButton::clicked,this,[=,&flag,&highlight](){
        if(flag==1)
        {
            highlight->setStyleSheet(QString("border-image: url(:/icon/%1-%2.png);"
                                             "background-image: url(:/res/none.png);"
                                            "border-radius:15px;").arg(highlight->myId1).arg(highlight->myId2));
            if(btn!=highlight)
            {

                btn->setStyleSheet(QString("border-image: url(:/icon/%1-%2.png);"
                                                            "background-image: url(:/res/none.png);"
                                                            "border-top: 5px solid yellow;"
                                                            "border-left: 5px solid yellow;"
                                                            "border-right: 5px solid yellow;"
                                                            "border-bottom: 10px solid yellow;"
                                                           "border-radius:15px;").arg(btn->myId1).arg(btn->myId2));//圆角
                highlight=btn;
            }
            else
            {
                flag=0;
                highlight=nullptr;
            }

        }
        else
        {
            flag=1;
            btn->setStyleSheet(QString("border-image: url(:/icon/%1-%2.png);"
                                                        "background-image: url(:/res/none.png);"
                                                        "border-top: 5px solid yellow;"
                                                        "border-left: 5px solid yellow;"
                                                        "border-right: 5px solid yellow;"
                                                        "border-bottom: 10px solid yellow;"
                                                       "border-radius:15px;").arg(btn->myId1).arg(btn->myId2));//圆角
            highlight=btn;
        }
    });
}
void MainWindow::disconnetbutton(MyButton *btn)
{
    disconnect(btn,0,0,0);
}
void MainWindow::addToDeleteWidget(MyButton *btn)
{
    //按照按钮的文字排序
    if(deleteBtnList->size()==0)
    {
        deleteBtnList->append(btn);
        connectbutton(deleteBtnList->at(0));
    }
    else{
        for (int i = 0; i < deleteBtnList->size(); i++)
        {
            MyButton *oldBtn = (MyButton *)deleteBtnList->at(i);
            if (oldBtn->Id >= btn->Id)
            {
                deleteBtnList->insert(i,btn);
                connectbutton(deleteBtnList->at(i));
                break;
            }
            else if(i==deleteBtnList->size()-1)//截止到最后一个仍然没找到相同的块
            {
                deleteBtnList->append(btn);
                int i=deleteBtnList->size()-1;
                connectbutton(deleteBtnList->at(i));
                break;
            }
        }
    }
    //消除
    removeSame();
    //显现按钮
    int size=deleteBtnList->size();
    for(int i=0;i<size;i++){
        deleteBtnList->at(i)->setParent(ui->deleteWidget);
        deleteBtnList->at(i)->setGeometry(i*105+37,25,105,105);//规定色块位置
        deleteBtnList->at(i)->show();
    }
    //检验gameover
    if(deleteBtnList->size()==7||(levelGroup->buttons().empty()&&deleteBtnList->size()!=0))
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
    else if(levelGroup->buttons().empty()&&deleteBtnList->size()==0)
    {
        ui->stackedWidget->setCurrentIndex(3);//设定为第3页
        //username score
    }
}/**/
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
            if(oldBtn->Id==nextBtn->Id){
                sum++;
                continue;
            }
        }
        if(sum==3)
        {
            //remove
            deleteBtnList->at(i)->setParent(NULL);
            disconnect(deleteBtnList->at(i),0,0,0);
            deleteBtnList->removeAt(i);
            deleteBtnList->at(i)->setParent(NULL);
            disconnect(deleteBtnList->at(i),0,0,0);
            deleteBtnList->removeAt(i);
            deleteBtnList->at(i)->setParent(NULL);
            disconnect(deleteBtnList->at(i),0,0,0);
            deleteBtnList->removeAt(i);
            score+=5;
            break;
        }
        else
        {
            sum=1;
        }
    }
    for (int i = 0; i < deleteBtnList->size()-1; i++)
    {
        MyButton *oldBtn = (MyButton *)deleteBtnList->at(i);
        MyButton *nextBtn = (MyButton *)deleteBtnList->at(i+1);
        //qDebug()<<oldBtn->myId2<<""<<nextBtn->myId2<<endl;
        if(oldBtn->Id==0&&nextBtn->Id==0)
        {
            int l=deleteBtnList->size();
            for(int j=0;j<l;j++)
            {
                deleteBtnList->at(i)->setParent(NULL);
                disconnect(deleteBtnList->at(i),0,0,0);
                deleteBtnList->removeAt(i);
            }
            score+=15;
            break;
        }
        else if(oldBtn->myId1.toInt()==nextBtn->myId1.toInt()&&oldBtn->myId2.toInt()!=nextBtn->myId2.toInt()){
            deleteBtnList->at(i)->setParent(NULL);
            disconnect(deleteBtnList->at(i),0,0,0);
            deleteBtnList->removeAt(i);
            deleteBtnList->at(i)->setParent(NULL);
            disconnect(deleteBtnList->at(i),0,0,0);
            deleteBtnList->removeAt(i);
            score+=2;
            break;
        }

    }
    ui->label->setText(QString::number(score));
}
MainWindow::~MainWindow()
{
    delete ui;
}
