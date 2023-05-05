#include "database.h"
int database::add_user(User player)
{
    int i;
    //void insert;
    for(i=0;i<11;i++)
    {
        if(Top_ten[i].score<player.score)
        {
            for(int j=9;j>i;j--)
            {
                Top_ten[j]=Top_ten[j-1];
            }
            Top_ten[i]=player;
            break;
        }
    }
    if(i == 11)
    i = -1;
    return i;
}

User* database::show_ranking()
{
    User *list = &Top_ten[0];
    return list;
}

database::database()
{
    int i;
    for(i = 0;i < 10;i++)
    {
        Top_ten[i].username = "无";
        Top_ten[i].score = 0;
    }
}
