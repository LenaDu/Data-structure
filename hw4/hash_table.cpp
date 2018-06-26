#include<string>
#include<iostream>

using namespace std;

int get_code_num(int num, int len)
{
    return num % len;
}

int get_code_str(string &str, int len)
{
    unsigned int temp=0;
    int i;
    for(i=0; i<str.length(); i++)
    {
        unsigned int ascii =(unsigned char) str[i];
        temp = (temp*256 + ascii)%len;
    }
    return temp % len;
}

bool all_number(string &s)
{
    for(char item:s)
    { if(!isdigit(item)){return false;} }
    return true;
}

int get_code(string &str, int len)
{
    if(all_number(str))
    { return get_code_num(strtol(str.c_str(),NULL,10),len);}
    else
    { return get_code_str(str, len);}
}

class Node
{
public:
    int frequency;
    string key;
    Node();
    void record(string &s);
    void rerecord(int &f, string &s);
};
Node::Node()
{
    frequency = 0;
    key = "";
}
void Node::record(string &s)
{
    frequency = 1;
    key = move(s);
}
void Node::rerecord(int &f, string &s)
{
    frequency = f;
    key = move(s);
}

class List
{
public:
    unsigned int length;
    Node *element;
    List();
    ~List() { delete[] element; };
    void push(Node &n);
    void remove(string &k, int breakpoint);
    int exist(string &key);
    void add_freq(int &index);
};
List::List()
{
    length = 0;
    element = nullptr;
}
void List::push(Node &n)
{
    auto temp = new Node[length+1];
    for(int i = 0; i<length; i++)
    {
        temp[i+1] = element[i];
    }
    delete[] element;
    element = temp;
    element[0] = n;
    length += 1;
}
void List::remove(string &k, int breakpoint)
{
    int index = breakpoint;
    if(index==-1){return;}
    else
    {
        auto temp = new Node[length-1];
        for(int i = 0; i<index; i++)
        {
            temp[i] = element[i];
        }
        if(index<length-1)
        {
            for(int i = index; i<length-1; i++)
            {
                temp[i] = element[i+1];
            }
        }
        delete[] element;
        element = temp;
        length -= 1;
    }
}
int List::exist(string &key)
{
    for(int i=0;i<length; i++)
    {
        if(element[i].key == key)
        {return i;}
    }
    return -1;
}
void List::add_freq(int &index)
{ element[index].frequency += 1; }

class Hash
{
public:
    int length;
    int load_factor;
    int size;
    List *list;
    Hash(int &len, int &lf);
    bool not_enough();
    void fill(int &code, string &k);
    void refill();
    void remove(string &k,int code);
    ~Hash() { delete[] list; }
};
Hash::Hash(int &len, int &lf)
{
    length = len;
    load_factor = lf;
    size = 0;
    list = new List[length];
};

bool Hash::not_enough()
{
    return size * 100 > length * load_factor;
}

void Hash::fill(int &code, string &k)
{
    int index = list[code].exist(k);
    if(index==-1)
    {
        Node tmp; tmp.record(k);
        list[code].push(tmp);
        size += 1;
    }
    else
    {
        list[code].add_freq(index);
    }
}

void Hash::refill()
{
    int new_len = length*2+1;
    auto temp_l = new List[new_len];
    for(int i = 0; i<length; i++)
    {
        for(int j=0; j<list[i].length; j++)
        {
            int new_code = get_code(list[i].element[j].key, new_len);
            Node temp_n; temp_n.rerecord(list[i].element[j].frequency, list[i].element[j].key);
            temp_l[new_code].push(temp_n);
        }
    }
    delete[] list;
    list = temp_l;
    length = new_len;
}

void Hash::remove(string &k,int code)
{
    int index = list[code].exist(k);
    if(index != -1)
        { list[code].remove(k,index); }
    else{return;}
//    for(int i=0; i<length; i++)
//    {
//        if(list[i].length==0){ continue;}
//        int index = list[i].exist(k);
//        if(index != -1)
//        {
//            list[i].remove(k,index);
//            break;
//        }
//    }
}


bool is_end(string &s)
{
    int counter = 0;
    if (s.length()>=24)
    {
        for(int i=0; i<24; i++)
        {
            if(s[i]=='*'){counter+=1;}
        }
        if(counter == 24){return true;}
    }
    return false;
}




int main() {
    string s;
    int lf,len; // load factor & length
    cin>>lf>>len;
//    scanf("%d %d",&lf, &len);
    Hash table(len,lf);
    char* temp_s;
    while (cin>>s)
    {
        if(s[0]=='*'){if(is_end(s))break;}
        // Get code
        int code = get_code(s,table.length);
        // Deal with str
        table.fill(code,s);
        while(table.not_enough()){table.refill();}
    }
    // Remove
//    int counter = 0;
    while (cin>>s)
    {
        if (s[0] == '*') { if (is_end(s))break; }
        int code = get_code(s,table.length);
//        counter += 1;
        table.remove(s,code);
//        if(counter >= 1000){break;}
    }
    // Output

    cout<<"["<<endl;
    for(int i=0; i<table.length-1; i++)
    {
        if(table.list[i].length==0)
            cout<<"{},"<<endl;
        else
        {
            cout<<"{";
            for(int j=0; j<table.list[i].length-1; j++)
            {
                Node temp = table.list[i].element[j];
                cout<<"\""<<temp.key<<"\""<<":"<<temp.frequency<<",";  //"key":freqeuncy,
            }
            Node temp = table.list[i].element[table.list[i].length-1];
            cout<<"\""<<temp.key<<"\""<<":"<<temp.frequency;    //"key":freqeuncy
            cout<<"},"<<endl;
        }
    }
    if(table.length>0)
    {

        if(table.list[table.length-1].length==0)
        {
            cout<<"{}"<<endl;
        }
        else
        {
            cout<<"{";
            for(int i=0; i<table.list[table.length-1].length-1; i++)
            {
                Node temp = table.list[table.length-1].element[i];
                cout<<"\""<<temp.key<<"\""<<":"<<temp.frequency<<",";
            }
            Node temp = table.list[table.length-1].element[table.list[table.length-1].length-1];
            cout<<"\""<<temp.key<<"\""<<":"<<temp.frequency;    //"key":freqeuncy
            cout<<"}\n";
        }
    }
    cout<<"]";
    return 0;
}