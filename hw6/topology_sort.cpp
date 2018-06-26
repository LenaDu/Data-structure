#include <iostream>
#include <string>

using namespace std;


class S_node
{
public:
    int degree;
    S_node()
    {
        degree = 0;
    }
};

class Source
{
public:
    S_node *element;
    Source()
    {
        element = new S_node[26];
        for(int i=0; i<26; i++)
        {
            char ascii = 97+i;
        }
    }
    ~Source()
    {
        if(element!= nullptr)delete element;
    }
    void add(char c)
    {
        int index = c-97;
        element[index].degree+=1;
    }
};


class C_node
{
public:
    char *child;
    int num;
    C_node()
    {
        child = new char[26];
        num = 0;
    }
    void add(char c)
    {
        child[num] = c;
        num += 1;
    }
};

class Connect           // Recording the connection
{
public:
    C_node* element;
    Connect()
    {
        element = new C_node[26];
    }
    bool record(string &s1, string &s2, Source &table)
    {
        long limit = min(s1.length(),s2.length());
        if(limit == 0){return true;}
        for(int i=0; i<limit; i++)
        {
            if(s1[i]!=s2[i])
            {
                if(existed(s1[i],s2[i])){return false;}
                int index = s1[i]-97;
                element[index].add(s2[i]);
                table.element[s2[i]-97].degree += 1;
                return true;                // succeed
            }
        }
        return s1.length()<s2.length();
    }
    bool existed(char c1, char c2)  // c2 existed in c1
    {
        int index = c2-97;
        if(element[index].num==0)
        {
            return false;
        }
        else
        {
            for(int i=0; i< element[index].num; i++)
            {
                if(element[index].child[i]==c1) { return true; }
            }
            return false;
        }
    }
};

string process(string s)
{
    string temp;
    for(char item : s)
    {
        if(item==' '){continue;}   // strip space
        else
        {
            int ascii = item;
            if(ascii>90){temp+=item;} // lowercase, add directly
            else                      // uppercase, transfer by ascii
            {
                ascii += 32;
                temp += ascii;
            }
        }
    }
    return temp;
}



int main() {
    int name_num, counter = 0;
    cin>>name_num;
    string pre_s;
    Source source_table;
    Connect connection;
    for(int i=0; i<name_num; i++)
    {
        string n;
        getline(cin,n);
        if(n.empty()){i-=1;continue;}
        n = process(n);
        if(pre_s[0]!=' ')
        {
            bool succeed = connection.record(pre_s,n,source_table);
            if(!succeed)
            {
                cout<<"另请高明吧"<<endl;
                return 0;
            }
        }
        counter += 1;
        pre_s = n;
    }
    int count_out = 0;
    int i;

    for(i=0; i<26; i++)
    {
        if(source_table.element[i].degree==0)       // Found a source
            {
                char temp = i+97;
                cout<<temp;                              // Output source
                count_out += 1;                            // Count output
                source_table.element[i].degree -= 1;       // Sign the source
                if(connection.element[i].num==0){continue;} // No child
                else
                {
                    for(int j=0; j<connection.element[i].num; j++)  // Decrease degree of children
                    {
                        int index = connection.element[i].child[j] - 97;
                        source_table.element[index].degree -= 1;
                    }
                    i = -1;                                   //Reset
                }
        }
    }
//    cout<<output<<endl;
//    for(int i=0;i<26;i++)
//    {
//        char temp = i+97;
//        cout<<"Alphabet: "<<temp<<" Degree: "<<source_table.element[i].degree<<endl;
//    }
//    for(int i=0; i<26; i++)
//    {
//        if(connection.element[i].num>0)
//        {
//            char temp = i+97;
//            cout<<"Parent: "<<temp<<endl;
//            cout<<"Child: "<<endl;
//            for(int j=0; j<connection.element[i].num; j++)
//            {
//                cout<<connection.element[i].child[j]<<" "<<endl;
//            }
//        }
//    }
    return 0;
}