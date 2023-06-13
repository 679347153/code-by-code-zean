#include "mainwindow.h"
#include "ui_mainwindow.h"
void sleep(int sectime)//非阻塞延时函数
{
    QTime dieTime = QTime::currentTime().addMSecs(sectime);

    while (QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 600);//最大600ms
    }
}
bool flag2=1;
database user;
User myuser,*userlist;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 采用http协议连接服务器
    Init();
    setWindowIcon(QIcon(":/res/icon.ico"));
    username=nullptr;
    score=0;
    levelGroup=nullptr;// 用于存放关卡中涉及的元素
    deleteBtnList=nullptr;// 用于存放消除区的元素
    modelGroup=nullptr; // 用于存放模板元素，
    //播放音乐
    c_button=new QSound(":/res/clear_010.wav",this);//消除声音
    p_button=new QSound(":/res/button.wav",this); //按下按钮声音
    QSound *startSound=new QSound(":/res/backgroundmusic.wav",this);
    startSound->setLoops(-1);
    startSound->play();
    //音乐开关
    connect(ui->music1,&QPushButton::clicked,this,[=,&flag2](){
        if(flag2)
        {
            startSound->stop();
            ui->music1->setStyleSheet("background-image: url(:/res/music_off.png);background-color: rgba(255, 255, 255,0);");
            ui->music2->setStyleSheet("background-image: url(:/res/music_off.png);background-color: rgba(255, 255, 255,0);");
        }
        else
        {
            startSound->setLoops(-1);
            startSound->play();
            ui->music1->setStyleSheet("background-image: url(:/res/music_on.png);background-color: rgba(255, 255, 255,0);");
            ui->music2->setStyleSheet("background-image: url(:/res/music_on.png);background-color: rgba(255, 255, 255,0);");

        }
        flag2=!flag2;
    });
    connect(ui->music2,&QPushButton::clicked,this,[=,&flag2](){
        if(flag2)
        {
            startSound->stop();
            ui->music1->setStyleSheet("background-image: url(:/res/music_off.png);background-color: rgba(255, 255, 255,0);");
            ui->music2->setStyleSheet("background-image: url(:/res/music_off.png);background-color: rgba(255, 255, 255,0);");
        }
        else
        {
            startSound->setLoops(-1);
            startSound->play();
            ui->music1->setStyleSheet("background-image: url(:/res/music_on.png);background-color: rgba(255, 255, 255,0);");
            ui->music2->setStyleSheet("background-image: url(:/res/music_on.png);background-color: rgba(255, 255, 255,0);");

        }
        flag2=!flag2;
    });
    /*首页排行按钮*/
    connect(ui->rank2,&QPushButton::clicked,this,[=](){
        Get();
        ui->goback2->setStyleSheet("background-image: url(:/res/go_back.png);"
                                   "background-color: rgba(255, 255, 255,0);");
        ui->stackedWidget->setCurrentIndex(3);
        ui->exit->setVisible(false);
        ui->addition->setVisible(false);
        disconnect(ui->goback2,0,0,0);
        connect(ui->goback2,&QPushButton::clicked,this,[=](){
            ui->stackedWidget->setCurrentIndex(4);
            ui->goback2->setStyleSheet("background-image: url(:/res/retry.png);"
                                       "background-color: rgba(255, 255, 255,0);");
            ui->exit->setVisible(true);
            ui->addition->setVisible(true);
            connect(ui->goback2,&QPushButton::clicked,this,[=]()
            {
                ui->stackedWidget->setCurrentIndex(1);//设定为第1页
            });
        });
    });
    connect(ui->start,&QPushButton::clicked,this,[=](){
        if(!flag2)p_button->play();
        ui->stackedWidget->setCurrentIndex(0);
    });
    //重玩游戏
    connect(ui->goback,&QPushButton::clicked,this,[=]()
    {
        if(!flag2)p_button->play();
        ui->stackedWidget->setCurrentIndex(1);//设定为第1页
    });
    connect(ui->goback2,&QPushButton::clicked,this,[=]()
    {
        if(!flag2)p_button->play();
        ui->stackedWidget->setCurrentIndex(1);//设定为第1页
    });
    connect(ui->exit,&QPushButton::clicked,this,[=]()//退出游戏
    {
        if(!flag2)p_button->play();
        exit(0);
    });
    //消除区列表
    deleteBtnList=new QList<MyButton *>();
    //加载模版图案
    //创建10个模版按钮
    modelGroup = new QButtonGroup();

    this->load_element();

    // 随机生成关卡中需要的元素
    levelGroup = new QButtonGroup();
    int nIndex = QFontDatabase::addApplicationFont(":/res/鸿雷拙书简体.ttf");
    if(nIndex != -1)
    {
        QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));
        if(strList.count()>0)
        {
            QFont font(strList.at(0));
            font.setPointSize(48);
            font.setItalic(1);
            ui->label->setFont(font);
        }
    }
    Gameinit();
}
QString name;
int num=0,len=10;
MyButton *highlight;
bool flag=0,flagx=0;
void MainWindow::slotReadyRead()
{
    int result;
    QNetworkReply *reply = (QNetworkReply *)sender();
    QString replyArray = reply->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(replyArray.toUtf8(), &jsonError));
    QJsonObject jsonObject=jsonDoc.object();//抓取json对象
    QJsonArray array=jsonObject["users"].toArray();//抓取并转化为数组
    user.delete_all();
    User my_user;
    for(int i=0; i<10; i++)
    {
        QJsonValue child =array.at(i);
        QJsonObject object=child.toObject();
        my_user.username =object.value("username").toString();
        my_user.score =object.value("score").toInt();
        user.add_user(my_user);
    }
    user.qsort1(0,9);//快速排序
    //写入排名
    userlist=user.show_ranking();
    for(int i=0;i<len;i++)
    {
        ui->rank->setItem(9-i,0,new QTableWidgetItem(userlist[i].username));
        ui->rank->setItem(9-i,1,new QTableWidgetItem(QString::number(userlist[i].score)));
    }
    //写入文件
    QFile json_file("./database.json");
    QFile file("./highscore.dat");
    file.open(QIODevice::Truncate|QIODevice::WriteOnly);
    bool ok=json_file.open(QIODevice::Truncate|QIODevice::WriteOnly);//设置为覆盖写入
    if(ok)
    {
        QTextStream stream(&json_file);
        QTextStream streamx(&file);
        stream.setCodec("UTF-8");		// 设置写入编码是UTF8
        streamx.setCodec("UTF-8");
        // 写入文件
        stream << jsonDoc.toJson();
        streamx<<jsonDoc.toJson();
        json_file.close();
        file.close();
    }
    else
    {
        qDebug()<<"write error!"<<endl;
    }
    if(jsonError.error != QJsonParseError::NoError)
    {
        result = -1;
    }
    QJsonObject rootObject = jsonDoc.object();
    QString codeResult = rootObject.value("code").toString();
    if (codeResult.compare("200") == 0)
    {
        //返回代码为200的时候证明请求成功
        if(rootObject.contains("result"))
        {

        }
        result = 0;
    }
    else
    {
        //请求失败对对应的处理
        result = codeResult.toInt();
    }
}

void MainWindow::slotgetpost()
{
    QNetworkReply *reply = (QNetworkReply *)sender();
    QString replyArray = reply->readAll();
    //qDebug()<< __FUNCTION__ <<replyArray;
}

void MainWindow::slotError(QNetworkReply::NetworkError code)
{
    QNetworkReply *reply = (QNetworkReply *)sender();
    QString s=reply->errorString();
    //qDebug()<< __FUNCTION__ <<s;
    if(flagx==0)
    {
         QMessageBox::information(this, "infor", "服务器连接失败,开启离线对战模式");
         flagx=1;
    }

}
void MainWindow::Get()
{
    QNetworkRequest request;
    request.setUrl(QUrl("http://code-by-code.emmmmm.tk/get-users"));//url
    request.setRawHeader("Connection", "keep-alive");//信息头
    request.setRawHeader("User-Agent", "Client");//信息头
     QNetworkReply *reply = m_pNetworkAccessManager->get(request);
     connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
     connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(slotError(QNetworkReply::NetworkError)));
     QEventLoop loop;  //事件循环，直到成功
     connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
     loop.exec();
     if(flagx==0)
     {
          QMessageBox::information(this, "infor", "服务器连接成功,开启在线对战模式");
          flagx=1;
     }
}

void MainWindow::Post(QString username, int score)
{
    QNetworkRequest request;
    request.setUrl(QUrl("http://code-by-code.emmmmm.tk/add-users"));//url
    request.setRawHeader("Connection", "keep-alive");//信息头
    request.setRawHeader("User-Agent", "Client");//信息头
    //请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));//设置成json格式！！
    QJsonObject interestObj;
    // 插入元素，对应键值对
    interestObj.insert("score", score);
    interestObj.insert("username", username);
    QByteArray bate=QJsonDocument(interestObj).toJson(QJsonDocument::Compact);
     QNetworkReply *reply = m_pNetworkAccessManager->post(request,bate);//post(str.toUtf8());
     connect(reply, SIGNAL(readyRead()), this, SLOT(slotgetpost()));
     QEventLoop loop;//事件循环，直到成功
     connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
     loop.exec();
}
void MainWindow::Init()
{
    m_pNetworkAccessManager=new QNetworkAccessManager(this);
}
void MainWindow::Gameinit()
{


    ui->stackedWidget->setCurrentIndex(4);//设定为第0页
    //  对表格进行初始化
    ui->rank->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color: rgba(255, 255, 255,0);background-image: url(:/res/none.png);}");
    ui->rank->verticalHeader()->setStyleSheet("QHeaderView::section {background-color: rgba(255, 255, 255,0);background-image: url(:/res/none.png);}");
    ui->rank->setColumnCount(2);
    ui->rank->setHorizontalHeaderLabels(QStringList()<<"  name  "<<"score");
    ui->rank->setRowCount(10);
    ui->rank->setVerticalHeaderLabels(QStringList()<<"10"<<"9"<<"8"<<"7"<<"6"<<"5"<<"4"<<"3"<<"2"<<"1");
    ui->rank->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->rank->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->lineEdit->setPlaceholderText("2-10个字");//输入名字栏
    ui->lineEdit->setAlignment(Qt::AlignCenter);
    connect(ui->submit1,&QPushButton::clicked,this,[=,&name]()
    {
        if(!flag2)p_button->play();
        name=ui->lineEdit->text();

        if(name.length()<=1||name.length()>10)
        {
            QMessageBox::critical(this,"警告","您输入的名字不符合要求，请输入2-10个字");
       }
        else ui->stackedWidget->setCurrentIndex(1);//设定为第1页
    });//确认按键

    connect(ui->one,&QPushButton::clicked,this,[=,&num](){
        if(!flag2)p_button->play();
       sleep(100);sleep(100);sleep(100);sleep(100);sleep(100);sleep(100);
       ui->stackedWidget->setCurrentIndex(2);//设定为第2页
       num=1;
       initGame();
       //特效
    });
    connect(ui->two,&QPushButton::clicked,this,[=,&num](){
        if(!flag2)p_button->play();
       sleep(100);sleep(100);sleep(100);sleep(100);sleep(100);sleep(100);
       ui->stackedWidget->setCurrentIndex(2);//设定为第2页
       num=2;
       initGame();
    });
    connect(ui->there,&QPushButton::clicked,this,[=,&num](){
        if(!flag2)p_button->play();
       sleep(100);sleep(100);sleep(100);sleep(100);sleep(100);sleep(100);
       ui->stackedWidget->setCurrentIndex(2);//设定为第2页
       num=3;
       initGame();
    });
    ui->label->setAlignment(Qt::AlignHCenter);
    ui->addition->setAlignment(Qt::AlignHCenter);
    ui->addition2->setAlignment(Qt::AlignHCenter);
    ui->addition2->setText("Programmed by zw and lcy. All rights reserved.");
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
    //初始化第num关
    distribution_element(num);
    //根据是否可点击设置颜色
    for (int i = 0 ; i < levelGroup->buttons().size(); i ++)
    {

        MyButton *btn=(MyButton *)levelGroup->buttons().at(i);
        setPictureByStatus(btn);
    }

}
bool MainWindow::isClickable(MyButton *btn)//判断方块是否能被点击
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


void MainWindow::setPictureByStatus(MyButton *btn)//通过状态设置图片
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

    MyButton * tmp_btn = (MyButton *) ui->goodsWidget->childAt(current_btn_point);//正下（左上）
    if(tmp_btn)
    {
        setPictureByStatus(tmp_btn);
    }

    QPoint left_below(current_btn_point.rx(),current_btn_point.ry() + 100);//左下
    tmp_btn = (MyButton *) ui->goodsWidget->childAt(left_below);
    if(tmp_btn)
    {
        setPictureByStatus(tmp_btn);
    }

    QPoint rigth_top(current_btn_point.rx() + 100,current_btn_point.ry());//右上
    tmp_btn = (MyButton *) ui->goodsWidget->childAt(rigth_top);
    if(tmp_btn)
    {
        setPictureByStatus(tmp_btn);
    }
    QPoint rigth_below(current_btn_point.rx() + 100,current_btn_point.ry() + 100);//右下
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
        /**/btn->setStyleSheet(QString("border-image: url(:/icon/%1-%2.png);"
                                    "background-image: url(:/res/none.png);"
                                   "border-radius:15px;").arg(tmp_x).arg(tmp_y));//圆角
        //更改底层按钮样式
        this->modelGroup->addButton(btn);
    }
}
void MainWindow::distribution_element(int level)
{
    //将关卡元素随机分配到空间中
    //并显示到parent上

    // 读取每个元素的位置
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
    //  循环读取每个位置的图案
    for (int i = 0 ; i < max_element; i ++)
    {
        QJsonValue child =array.at(i);
        QJsonObject object=child.toObject();

        QList<QString> tmp_list =object.value((QString::number(i))).toString().split(",");
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
    //消除音效
     if(!flag2)p_button->play();
     QPoint current_btn_point = btn->pos();
     int x=0;
     //按照按钮的文字排序
     if(deleteBtnList->size()==0)
     {
         deleteBtnList->append(btn);
         connectbutton(deleteBtnList->at(0));
         x=0;
     }
     else{
         for (int i = 0; i < deleteBtnList->size(); i++)
         {
             MyButton *oldBtn = (MyButton *)deleteBtnList->at(i);
             if (oldBtn->Id >= btn->Id)
             {
                 deleteBtnList->insert(i,btn);
                 connectbutton(deleteBtnList->at(i));
                 x=i;
                 break;
             }
             else if(i==deleteBtnList->size()-1)//截止到最后一个仍然没找到相同的块
             {
                 deleteBtnList->append(btn);
                 int i=deleteBtnList->size()-1;
                 connectbutton(deleteBtnList->at(i));
                 x=i;
                 break;
             }
         }
     }
//xa
     //动画效果
     QPropertyAnimation *pPropertyAnimation = new QPropertyAnimation(btn, "",this);
     pPropertyAnimation->setDuration(500);//动画快慢
     pPropertyAnimation->setEasingCurve(QEasingCurve::Linear);//线性函数
     pPropertyAnimation->setStartValue(btn->pos());//初始点
     pPropertyAnimation->setEndValue(QPoint(0+105*x,1144));//结束点
     connect(pPropertyAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant &value) {
             //btn->move(value);
             btn->move(value.toPoint());
         });
     btn->raise();
     pPropertyAnimation->start();
     sleep(480);
     //刷新delete栏
     int size=deleteBtnList->size();
     for(int i=0;i<size;i++){
         deleteBtnList->at(i)->setParent(ui->deleteWidget);
         deleteBtnList->at(i)->setGeometry(i*105+37,25,105,105);//规定色块位置
         deleteBtnList->at(i)->show();
     }
     //重新计算是否可以被点击及样式
     setSideBtn(current_btn_point);/**/
     sleep(40);
     //将消除栏前置
     ui->deleteWidget->raise();
     // 从levelGroup中移除btn，并加入到deleteWidget。
    levelGroup->removeButton(btn);
    if(btn->Id==0&&flag==1)//万能卡消除单个卡
    {
        flag=0;
        highlight->setParent(NULL);
        btn->setParent(NULL);
        disconnect(highlight,0,0,0);
        disconnect(btn,0,0,0);
        deleteBtnList->removeOne(highlight);
        deleteBtnList->removeOne(btn);
        score+=2;
        ui->label->setText(QString::number(score));
        //刷新delete栏
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
        //刷新delete栏
        int size=deleteBtnList->size();
        for(int i=0;i<size;i++){
            deleteBtnList->at(i)->setParent(ui->deleteWidget);
            deleteBtnList->at(i)->setGeometry(i*105+37,25,105,105);//规定色块位置
            deleteBtnList->at(i)->show();
        }
        addToDeleteWidget((MyButton*)btn);
    }

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
    else if(levelGroup->buttons().empty()&&deleteBtnList->size()==0)//判断游戏成功
    {

        myuser.username=username;
        myuser.score=score;
        Get();
        int i=user.add_user(myuser)+1;
        Post(username,score);
        userlist=user.show_ranking();
        //输出表格
        for(int i=0;i<len;i++)
        {
            ui->rank->setItem(9-i,0,new QTableWidgetItem(userlist[i].username));
            ui->rank->setItem(9-i,1,new QTableWidgetItem(QString::number(userlist[i].score)));
        }/**/
        ui->stackedWidget->setCurrentIndex(3);//设定为第3页
        if(i!=0)
        {
            ui->addition->setText(QString("恭喜榜%1").arg(QString::number(i)));
        }
        else
        {
            ui->addition->setText("未上榜");
        }
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
            if(!flag2)
            c_button->play();
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
        if(oldBtn->Id==0&&nextBtn->Id==0)
        {
            int l=deleteBtnList->size();
            for(int j=0;j<l;j++)
            {
                if(!flag2)
                c_button->play();
                deleteBtnList->at(i)->setParent(NULL);
                disconnect(deleteBtnList->at(i),0,0,0);
                deleteBtnList->removeAt(i);
            }
            score+=15;
            break;
        }
        else if(oldBtn->myId1.toInt()==nextBtn->myId1.toInt()&&oldBtn->myId2.toInt()!=nextBtn->myId2.toInt()){
            if(!flag2)
            c_button->play();
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
