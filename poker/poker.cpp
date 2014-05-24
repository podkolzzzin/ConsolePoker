#include <iostream>
#include <Windows.h>
#include <time.h>
#include <string>
#include <fstream>
#include <vector>
#include <exception>
#include "graphics.h"
#include <deque>
#include <algorithm>
using namespace std;
const int numDecks=1;
struct Card
{
    int value;
    int suit;
    bool operator <(const Card& p)
    {
        return value<p.value;
    }
};
struct Combo
{
    int value;
    Card kiker;
};
string toString(Card a)
{
    string ret;
    switch(a.value)
    {
    case 11:ret="Валет";break;
    case 12:ret="Дама";break;
    case 13:ret="Король";break;
    case 14:ret="Туз";break;
    default: ret=itoa(a.value,new char[20],10);
    }
    ret+=" ";
    switch(a.suit)
    {
    case 1:ret+="чирва";break;
    case 2:ret+="буба";break;
    case 3:ret+="трефа";break;
    case 4:ret+="пика";break;
    }
    return ret;

}
string stringCombo(int combo)
{
    if(combo==913) return "Роял флеш";
    else if(combo<913 && combo>900)
    {
        Card a={combo-900,-1};
        return "Стрит флеш до "+toString(a);
    }
    else if(combo<=813 && combo>800)
    {
        Card a={combo-800,-1};
        return "Каре из "+toString(a);
    }
    else if(combo<=713 && combo>700)
    {
        Card a={combo-700,-1};
        return "Фул хаус со старшей картой "+toString(a);
    }
    else if(combo<=613 && combo>600)
    {
        Card a={combo-600,-1};
        return "Флеш со старшей картой "+toString(a);
    }
    else if(combo<=513 && combo>500)
    {
        Card a={combo-500,-1};
        return "Стрит до "+toString(a);
    }
    else if(combo<=413 && combo>400)
    {
        Card a={combo-400,-1};
        return "Сет из "+toString(a);
    }
    else if(combo<=313 && combo>300)
    {
        Card a={combo-300,-1};
        return "Две пары со старшей картой "+toString(a);
    }
    else if(combo<=213 && combo>200)
    {
        Card a={combo-200,-1};
        return "Пара "+toString(a);
    }
    else 
    { 
        Card a={combo-100,-1};
        return "Кикер "+toString(a);
    }
}
vector<Card> createDeck()
{
    vector<Card> deck;
    for(int k=0;k<numDecks;k++)
    {
        for(int i=0;i<13;i++)
        {
            for(int j=0;j<4;j++)
            {
                Card a={i+2,j+1};
                deck.push_back(a);
            }
        }
    }
    return deck;
}
void shakeDeck(vector<Card> &deck)
{
    Card a;
    srand(time(0));
    int r;
    for(int k=0;k<numDecks;k++)
    {
        for(int i=0;i<deck.size();i++)
        {
            r=rand()%deck.size();
            a=deck[i];
            deck[i]=deck[r];
            deck[r]=a;
        }
    }
}
int kikirInSuit(vector<Card> &c,int suit)
{
    int maxI=-1;

    for(int i=0;i<c.size();i++)
    {
        if(c[i].suit==suit)
        {
            if(maxI==-1)
            {
                maxI=i;
            }
            if(c[i].value>c[maxI].value)
            {
                maxI=i;
            }
        }
    }

    return maxI;
}
int kikirInDiap(vector<Card> &c,int begin,int end)
{
    int maxI=begin;
    if(end>c.size()) end=c.size();
    for(int i=begin;i<end;i++)
    { 
        if(c[i].value>c[maxI].value)
        {
            maxI=i;
        }
    }
    return maxI;
}
int findFlash(vector<Card> &c)
{
    int mas[4]={};
    for(int j=0;j<4;j++)
    {
        for(int i=0;i<c.size();i++)
        {
            if(c[i].suit==(j+1))
            {
                mas[j]++;
            }
        }
    }
    for(int i=0;i<4;i++)
    {
        if(mas[i]==5)
        {
            int t=kikirInSuit(c,i+1);
            return 600+c[t].value;
        }
    }
    return 0;
}
int findStreet(vector<Card> &c)
{
    int counter=0;
    int start=0;

    for(int i=1;i<c.size();i++)
    {
        if(c[i-1].value+1==c[i].value)
        {
            counter++;
        }
        else
        {
            counter=0;
            start=i-1;
        }
    }
    if(counter==4)
    {
        int t=kikirInDiap(c,start,start+6);
        return 500+c[t].value;
    }
    return 0;
}
int findQuads(vector<Card> &c)
{
    int counter=0;
    int r=0;
    for(int i=0;i<c.size();i++)
    {
        for(int j=0;j<c.size();j++)
        {
            if(c[i].value==c[j].value && i!=j)
            {
                counter++;
            }
        }
        if(counter==3)
        {
            r=c[i].value;
        }
        counter=0;
    }
    if(r)
    {
        return 800+r;
    }
    return false;
}
int findSet(vector<Card> &c)
{
    int counter=0;
    int r=0;
    for(int i=0;i<c.size();i++)
    {
        for(int j=0;j<c.size();j++)
        {
            if(c[i].value==c[j].value && i!=j)
            {
                counter++;
            }
        }
        if(counter==2)
        {
            r=c[i].value;
        }
        counter=0;
    }
    if(r)
    {
        return 400+r;
    }
    return false;
}
int findPair(vector<Card> &c)
{
    int counter=0;
    int r=0;

    for(int i=0;i<c.size();i++)
    {
        for(int j=0;j<c.size();j++)
        {
            if(c[i].value==c[j].value && i!=j)
            {
                counter++;
            }
        }
        if(counter==1)
        {
            r=c[i].value;
        }
        counter=0;
    }
    if(r)
    {
        return 200+r;
    }
    return false;
}
int findStreetFlash(vector<Card> &c)
{
    int counter=0;
    int start=0;
    for(int i=1;i<c.size();i++)
    {
        if(c[i-1].suit==c[i].suit)
        {
            if(c[i-1].value+1==c[i].value)
            {
                counter++;
            }
            else
            {
                counter=0;
                start=i-1;
            }
        }
    }

    if(counter==4)
    {
        int t=kikirInDiap(c,start,start+6);

        return 900+c[t].value;
    }
    return 0;

}
int findTwoPair(vector<Card> &c)
{
    int x=findPair(c);
    int r=0;
    int counter=0;
    if(!x)
    {
        return false;
    }
    else
    {
        for(int i=0;i<c.size();i++)
        {
            for(int j=0;j<c.size();j++)
            {
                if((c[i].value==c[j].value && i!=j) && c[i].value!=(x-200))
                {
                    counter++;
                }
            }
            if(counter==1)
            {
                r=c[i].value;
            }
            counter=0;
        }
        if(r)
        {
            //cout<<x<<" "<<r<<endl;
            if((x-200)>r) return 100+x;
            else 300+r;
        }
        return false;
    }

}
int findFulHouse(vector<Card> &c)
{
    int x=findSet(c);
    int counter=0;
    int r=0;
    if(!x) return false;
    for(int i=0;i<c.size();i++)
    {
        for(int j=0;j<c.size();j++)
        {
            if((c[i].value==c[j].value && i!=j) && c[j].value!=x-400)
            {
                counter++;
            }
        }
        if(counter==1)
        {
            r=c[i].value;
        }
        counter=0;
    }
    if(r)
    {
        if(x-400>r) return 700+x-400;
        else return 700+r;
    }
    return false;
}
int findCombo(vector<Card> &c)
{
    sort(c.begin(),c.end());
    int x=0;
    if(x=findStreetFlash(c))
    {
        return x;
    }
    else if(x=findQuads(c))
    {
        return x;
    }
    else if(x=findFulHouse(c))
    {
        return x;
    }
    else if(x=findFlash(c))
    {
        return x;
    }
    else if(x=findStreet(c))
    {
        return x;
    }
    else if(x=findSet(c))
    {
        return x;
    }
    else if(x=findTwoPair(c))
    {
        return x;
    }
    else if(x=findPair(c))
    {
        return x;
    }
    else
    {
        return 100+c[kikirInDiap(c,0,c.size())].value;
    }
    return 0;
}
Card takeFirst(vector<Card> &c)
{
    Card ret=c.back();
    c.pop_back();
    if(c.size()==0)
    {
        c=createDeck();
        shakeDeck(c);
    }
    return ret;
}
class Player
{
    int combo;
    int rate;
    bool isPlay;
    vector<Card> c;
public:
    int money;
    Player()
    {
        combo=-1;
        money=1000;
        isPlay=true;
        rate=0;
    }
    void fold()
    {
        rate=0;
        isPlay=false;
    }
    int call(int sum)
    {
        if(sum<money)
        {
            money-=sum;
            rate+=sum;
            return sum;
        }
        else
        {
            money=0;
            rate+=money;
            return money;
        }
    }
    int getMoney()
    {
        return money;
    }
    bool rise(int sum)
    {
       
        return call(sum);
    }
    void addCard(Card a)
    {
        c.push_back(a);
    };
    void printCards()
    {
        for(int i=0;i<c.size();i++)
        {
            cout<<toString(c[i])<<endl;
        }
    }
    string getCombo()
    {
        return stringCombo(findCombo(c));;
    }
    int getIntCombo()
    {
        return findCombo(c);
    }
    int numCards()
    {
        return c.size(); 
    }
    virtual int ask(int sum=0,int blefCombo=0)=0;
};
class PlayerMan: public Player
{
public:
    int ask(int sum=0,int blefCombo=0)
    {
        int des=0;
        if(sum)
        {
            cout<<"Уровнять "<<sum<<"(1) или сбросить(0)";
            if(sum<money) cout<<" или поднять(2)"<<endl;
            cin>>des;
            if(des==1)
            {
                return call(sum);
            }
            else if(des==2 && money>sum)
            {
                call(sum);
                cout<<"Введите сумму на которую вы хотите поднять она должна быть меньше или равна "<<money<<endl;
                int r=0;
                cin>>r;
                return rise(r);
            }
            else if(des>1 && money<=sum)
            {
                return call(sum);
            }
            else
            {
                fold();
                return -2;
            }
        }
        else
        {
            if(money)
            {
                cout<<"Пропустить(0) или поднять(1)";
                cin>>des;
                if(des)
                {
                    cout<<"Введите сумму на которую вы хотите поднять она должна быть меньше или равна "<<money<<endl;
                    int r=0;
                    cin>>r;
                    return rise(r);
                }
                else
                {
                    return -1;
                }
            }
            else
            {
                return -1;
            }
        }
    }
};

class PlayerBot:public Player
{
public:
    PlayerBot()
    {
    }
    int ask(int sum=0,int blefCombo=0)
    {
        //bool isfold=true;
        //bool call=true;
        //bool rise=true;
        //bool check=true;
        
        int combo;
        if(!blefCombo)
            combo=getIntCombo();
        else
            combo=blefCombo;
        
        if(numCards()>=5)
        {
            if(sum<money)
            {
                if(combo>110 && combo<200) 
                {
                    if(sum<money/5)
                    {
                        rise(sum+money/10);
                        return sum+money/10;
                    }
                    else if(rand()%2)
                    {
                        return ask(sum,110+rand()%700);
                    }
                    else 
                    {
                        fold();
                        return -1;
                    }
                }
                else if(combo>200 && combo<300)
                {
                    if(sum<money/4)
                    {
                        rise(sum+money/4);
                        return sum+money/4;
                    }
                    else if(rand()%2)
                    {
                        return ask(sum,110+rand()%700);
                    }
                    else 
                    {
                        fold();
                        return -1;
                    }
                }
                else if(combo>300 && combo<400)
                {
                    if(sum<money/3)
                    {
                        rise(sum+money/3);
                    }
                    else if(rand()%2)
                    {
                        return ask(sum,110+rand()%700);
                    }
                    else 
                    {
                        fold();
                        return -1;
                    }
                }
                else if(combo<110)
                {
                    srand(time(0));
                    bool isBlef=rand()%2;
                    if(isBlef)
                    {
                        return ask(sum,110+rand()%700);
                    }
                    else
                    {
                        fold();
                        return -1;
                    }
                }
                else
                {
                    srand(time(0));
                    int riser=rand()%5;
                    if(riser)
                    {
                        rise(money/riser);
                    }
                    else
                    {
                        call(sum);
                    }
                }
            }
            else
            {
                srand(time(0));
                bool isBlef=rand()%2;
                if(combo>210 || isBlef)
                {
                    return call(sum);
                }
                else
                {
                    fold();
                    return -1;
                }
            }
        }
        else
        {
            return call(sum);
        }
    }

};
bool gameIsGoing(vector<PlayerBot> &bots,PlayerMan &man)
{
    return true;
}
void botActivities(vector<PlayerBot>& bots)
{

}
void getCards(vector<PlayerBot> &bots,PlayerMan &man,vector<Card> &deck)
{
    for(int i=0;i<bots.size();i++)
    {
        bots[i].addCard(takeFirst(deck));
        bots[i].addCard(takeFirst(deck));
    }
    man.addCard(takeFirst(deck));
    man.addCard(takeFirst(deck));   
}
void main()
{
    SetConsoleOutputCP(1251);
    vector<Card> deck=createDeck();
    shakeDeck(deck);

    cout<<"Введите количество компьютеров"<<endl;
    int numPlayers;
    cin>>numPlayers;
    if(numPlayers<1) numPlayers=1;
    if(numPlayers>5) numPlayers=5;
    cout<<"Вы хотите играть с "<<numPlayers<<" компьютеров"<<endl;
    vector<PlayerBot> bots(numPlayers);
    PlayerMan man;
    const int start=10;
    int openCardNum=0;
    int con=0;
    int callNeed=0;
    int answer=0;
    //////////////////
    cout<<"Начальная ставка "<<start<<endl;
    for(int i=0;i<bots.size();i++)
    {
        if(bots[i].call(start)==start)
        {
            cout<<i+1<<" игрок в игре"<<endl;
        }
        else
        {
            cout<<i+1<<" отказался от учатсия потому что не смог сделать начальную ставку";
            bots.erase(bots.begin()+i);
        }
    }
    if(man.call(start)==start)
    {
        cout<<"Вы сделали начальную ставку"<<endl;
    }
    getCards(bots,man,deck);
    int numCards=0;
    /////////////
    while(numCards!=5)
    {
        for(int i=0;i<bots.size();i++)
        {
            answer=bots[i].ask(callNeed);
            if(answer==-1)
            {
                bots.erase(bots.begin()+i);
                i--;
                cout<<i+1<<" игрок сбросил"<<endl;
            }
            else if(answer>callNeed)
            {
                callNeed=answer;
                cout<<i+1<<" игрок поднял на "<<callNeed<<endl;
            }
            else
            {
                cout<<i+1<<" игрок уровнял"<<endl;
            }
        }
        system("Pause");
        callNeed=200;
        man.printCards();
        answer=man.ask(callNeed);
    
        if(answer==-2)
        {
            cout<<"Вы сбросили";
    //        break;
        }
        else if(answer==callNeed)
        {
            callNeed=0;
            cout<<"Вы уровняли"<<endl;
        }
        else if(answer>callNeed)
        {
            cout<<"Вы подняли на "<<callNeed<<endl;
            callNeed=answer-callNeed;
        }
        else
        {
            cout<<"Вы пропустили ход "<<endl;
        }
        //   cout<<endl<<p1.getCombo();
        system("Pause");
        if(numCards==0)
        {
            for(int i=0;i<bots.size();i++)
            {
                bots[i].addCard(takeFirst(deck));                bots[i].addCard(takeFirst(deck));                bots[i].addCard(takeFirst(deck));
            }
            man.addCard(takeFirst(deck)) ;           man.addCard(takeFirst(deck));            man.addCard(takeFirst(deck));numCards=3;
        }
        else if(numCards<5)
        {
            for(int i=0;i<bots.size();i++)
            {
                bots[i].addCard(takeFirst(deck)); 
            }
            man.addCard(takeFirst(deck)) ;numCards++;
        }

    }

}