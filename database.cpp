#include "database.h"
//快速排序（为未来大量数据做准备）
void database::qsort1(int l,int r)//b
{
    User swap;
    int mid=Top_ten[(l+r)/2].score;
    int i=l,j=r;
    while(i<=j)
    {
        while(Top_ten[i].score>mid)i++;
        while(Top_ten[j].score<mid)j--;
        if(i<=j)
        {
            swap=Top_ten[i];
            Top_ten[i]=Top_ten[j];
            Top_ten[j]=swap;
            i++;j--;
        }
    }
    if(i<r) qsort1(i,r);
    if(l<j) qsort1(l,j);
}
void database::delete_all()
{
    for(int i=0;i<10;i++)
    {
        Top_ten[i].score=0;
        Top_ten[i].username="";
    }
}
int database::add_user(User player)
{
    int i;
    for(i=0;i<10;i++)
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
    if(i == 10)
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
