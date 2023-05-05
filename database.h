#ifndef DATABASE_H
#define DATABASE_H
#include <QString>
struct User
{
    QString username;
    int score;
};
class database
{
   private:
   User Top_ten[10];
   public:
   database();
   int add_user(User player);   //向排行榜数组中插入一个玩家,若玩家成功上榜,则返回玩家排名（整型）;若玩家未上榜,则返回整型0
   User* show_ranking();        //返回排行榜数组
};

#endif // DATABASE_H
