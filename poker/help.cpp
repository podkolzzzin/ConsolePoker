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
    default: ret=itoa(a.value,new char[20],8);
    }
    ret+=" ";
    switch(a.suit)
    {
    case 1:ret+="чирва";break;
    case 2:ret+="буба";break;
    case 3:ret+="трефа";break;
    case 4:ret+="пика";break;
    default:ret+=" фиг его";
    }

    return ret;
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
        return kikirInDiap(c,0,c.size());
    }
    return 0;
}