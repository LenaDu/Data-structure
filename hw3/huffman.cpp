#include<string>
#include<iostream>
#include<istream>
#include<ostream>
#include<sstream>

using namespace std;

typedef struct element_node *node;
typedef struct element_list *list;
typedef struct dictionary *dict;
typedef struct dictionary_node *d_node;

typedef struct element_node     // Store node
{
    char character;
    uint8_t ascii;
    unsigned int weight;
    node left;
    node right;
}element_node;
typedef struct element_list     // A list that store node
{
    node *element;
    int length;
}element_list;
typedef struct dictionary_node      // Store the name and its corresponding code
{
    char name;
    string code;
//    unsigned int frequency;
}dictionary_node;
typedef struct dictionary
{
    d_node *element;
    int length;
}dictionary;

list create_list()
{
//    auto init_list = (list)(malloc(sizeof(element_list)));
//    list init_list = reinterpret_cast<list>(new list);
    auto init_list = new element_list;
     init_list->element = new node;
    init_list->element[0] = new element_node;
//    init_list->element = (node *)(malloc(sizeof(element_node)));
//    init_list->element[0] = (node)(malloc(sizeof(element_node)));
    init_list->length = 0;
    return init_list;
}

void puzzle_list(list s, int point)
{
    for(int i = point; i<s->length-1; i++)
    {
        s->element[i] = s->element[i+1];
    }
    s->length -= 1;
//    s->element = (node *)(realloc(s->element,(s->length)*sizeof(element_node)));
    node *temp = new node[s->length];
    for(int i=0; i<s->length; i++)
    {
        temp[i] = s->element[i];
    }
    delete[] s->element;
    s->element = temp;
}

int search_dict(dict d, char c)
{
    for(int index=0; index<d->length; index++)
    {
        if(d->element[index]->name == c)
        {
            return index;
        }
    }
    return -1;
}

node get_heap(list s)
{
//    auto new_root = (node)(malloc(sizeof(element_node)));
    auto new_root = new element_node;
    node min_node_1;
    node min_node_2;
    int index = 0;
    min_node_1 = s->element[0];
    for (int current=0; current<s->length; current++)
    {
        if(s->element[current]->weight>0)       /* Seach the smallsest one */
        {
            if(s->element[current]->weight < min_node_1->weight)
            {
                min_node_1 = s->element[current];
                index = current;
            }
            else if(s->element[current]->weight == min_node_1->weight)
            {
                if(s->element[current]->ascii <= min_node_1->ascii)
                {
                    min_node_1 = s->element[current];
                    index = current;
                }
            }
        }
    }
    puzzle_list(s,index);
    min_node_2 = s->element[0];
    for (int current=0; current<s->length; current++)
    {
        if(index == current){continue;}
        if(s->element[current]->weight>0)
        {
            if(s->element[current]->weight < min_node_2->weight)
            {
                min_node_2 = s->element[current];
                index = current;
            }
            else if(s->element[current]->weight == min_node_2->weight)
            {
                if(s->element[current]->ascii <= min_node_2->ascii)
                {
                    min_node_2 = s->element[current];
                    index = current;
                }
            }

        }
    }
    puzzle_list(s,index);
    new_root->left = min_node_2;
    new_root->right = min_node_1;
    new_root->ascii =  min_node_1->ascii > min_node_2->ascii? min_node_2->ascii: min_node_1->ascii; /* Set the less ascii */
    new_root->weight = min_node_1->weight+min_node_2->weight;

    return new_root;
}

void insert_first_list(list s, char c)
{
    bool found = false;
    //bool jumpout = false;
    for(int index=0; index<s->length; index++)      // Already existed this char
    {
        if(s->element[index]->character == c)
        {
            s->element[index]->weight+=1;
            found = true;
        }
    }
    if(!found)          // Not yet exist
    {
        int new_index = s->length;
        if(s->length>0)
        {
            node *temp = new node[s->length+1];
            for(int i=0; i<s->length; i++)
            {
                temp[i] = s->element[i];
            }
            delete[] s->element;
            s->element = temp;
            s->element[new_index] = new element_node;
        }
        s->element[new_index]->weight = 1;
        s->element[new_index]->character = c;
        s->element[new_index]->ascii = static_cast<uint8_t>(c);
        s->element[new_index]->left = nullptr;
        s->element[new_index]->right = nullptr;
        s->length += 1;
    }
}

void insert_list(list s, node n)
{
    node *temp = new node[s->length+1];
    for(int i=0; i<s->length; i++)
    {
        temp[i] = s->element[i];
    }
    delete[] s->element;
    s->element = temp;
    s->element[s->length] = n;
    s->length+=1;
}

void free_s(list s)
{
    for(int index = 0; index < s->length; index++)
    {
        delete(s->element[index]);
    }
    delete(s->element);
    free(s);
}

void free_dict(dict d)
{
    for(int index = 0; index< d->length;index++)
    {
        delete(d->element[index]);
    }
    delete(d->element);
    free(d);
}



void get_encode(dict d, node n,  string str, bool left)
{
    if(!n){return;}
    int found = search_dict(d,n->character);
    if(found != -1&&!n->left&&!n->right) // Found
    {
        if(left){str += "0";}
        else if(!left){str += "1";}
//            cout<<n->character<<" "<<n->ascii<<" "<<n->weight<<" "<< str<<" "<<endl;
        d->element[found]->code = str;
    }
    
    else
    {
        if(left)
        { str += "0"; }
        else if(!left)
        { str += "1"; }
    }
    get_encode(d, n->left,str, true);
    get_encode(d, n->right,str, false);
}


string to_bin_str(char hex_str)
{
    switch (hex_str)
    {
        case '1': { return "0001"; }case '2': { return "0010"; }case '3': { return "0011"; }case '4': { return "0100"; }
        case '5': { return "0101"; }case '6': { return "0110"; }case '7': { return "0111"; }case '8': { return "1000"; }
        case '9': { return "1001"; }case 'a': { return "1010"; }case 'b': { return "1011"; }case 'c': { return "1100"; }
        case 'd': { return "1101"; }case 'e': { return "1110"; }case 'f': { return "1111"; }case 'A': { return "1010"; }
        case 'B': { return "1011"; }case 'C': { return "1100"; }case 'D': { return "1101"; }case 'E': { return "1110"; }
        case 'F': { return "1111"; }case '0': { return "0000";}
        default:return"";
    }
}

string reverse_str(string str)
{
    string return_str = "";
    for(int i = str.length()-1; i>=0; i--)
    {
        return_str += str[i];
    }
    return return_str;
}

int main() {
    //freopen("to_encode.in", "r", stdin);
    string inputstr;
    bool do_encode=false;
    char out_char[6];
    fread(out_char,1,6,stdin);
    if(out_char[0]=='E'){do_encode = true;}
    if (do_encode)
    {
        char a_char;
        while ((a_char = (getchar())) != EOF)
        {
            inputstr += a_char;
        }
        string my_str = inputstr;
//        printf("%s",my_str.c_str());
        list my_list = create_list();       // Create list

        for(int i = 0; i<my_str.length(); i++)                  // Fill the initial list
        { insert_first_list(my_list, my_str[i]); }

        auto char_dict = new dictionary;
        char_dict->length = my_list->length;
        char_dict->element = new d_node[char_dict->length];
        for(int j = 0; j < char_dict->length; j++)                            // Record characters into dictionary and initialize
        {
            char_dict->element[j] = new dictionary_node;
            char_dict->element[j]->name = my_list->element[j]->character;
            char_dict->element[j]->code = "";
        }

        fwrite("HUFFMAN", 1, 8,  stdout);   // Output HUFFMAN

        unsigned int frequency[256]={0};                         // Output frequency
        for(int i = 0; i<my_list->length; i++)
        {
            unsigned int temp;
            temp = my_list->element[i]->ascii;
            frequency[temp] = my_list->element[i]->weight;
        }
        fwrite(frequency, 4, 256, stdout);

        while(my_list->length>1)
        { insert_list(my_list,get_heap(my_list));} // Construct Huffman coding tree

        if(my_list->length>=1)
        {
            get_encode(char_dict, my_list->element[0]->left, "", true);
            get_encode(char_dict, my_list->element[0]->right, "", false);   // encoding
            
            string bi_code ;
            for(int i = 0; i<my_str.length(); i++)                  // Connect
            {
                int index = search_dict(char_dict, my_str[i]);
                if ( index != -1) {
                    bi_code += char_dict->element[index]->code;
                }
            }
            int add_zero=(bi_code.length() % 8);                // add zero
            for(int i = 0; i< add_zero; i++)
            { bi_code += '0'; }

            char hex_buf[8];// transfer to hex
            unsigned int num[1];
            for(int i = 0; i< bi_code.length(); i++)
            {
                if(bi_code[i])
                {
                    hex_buf[7-(i%8)]=bi_code[i];
                    if(((i+1)%8) == 0)
                    {
                        num[0] = strtol(hex_buf, nullptr, 2);
                        fwrite(num, 1, 1, stdout);
                    }
                }
            }
        }
        free_dict(char_dict);
        free_s(my_list);

    }
    else if(out_char[0]!='E')      // Decode
    {
        char huf[8];
        fread(huf,1,8,stdin);

        list my_list = create_list();
        int count_valid = 0;                    // Count and index
        int count_total = 0;                    // Count total frequency
        unsigned int frequency[256];

        fread(frequency,4,256,stdin);

        for(int i = 0; i<256; i++)
        {
            if(frequency[i]!=0)
            {
//                printf("here");
                char ascii_char = static_cast<char>(i);
                insert_first_list(my_list,ascii_char);
                my_list->element[count_valid]->weight = frequency[i];
                my_list->element[count_valid]->ascii = static_cast<uint8_t>(i);
                my_list->element[count_valid]->character = ascii_char;
                count_valid += 1;
                count_total += frequency[i];
            }
        }
        return 0;
        if (count_total>0)
        {

            unsigned int a_char;
            while ((a_char = (getchar())) != EOF)
            {
                unsigned int dec_num = a_char;
                char hex_str[1] ={0};
                sprintf(hex_str,"%x",dec_num);
                string temp = hex_str;
                if(temp.length()==1){inputstr+='0';}
                inputstr += hex_str;
            }
            string my_str = inputstr;

            while(my_list->length>1)
            { insert_list(my_list,get_heap( my_list));} // Construct Huffman coding tree

            string buf_str;                 // Deal with encoded substance
            string str_code;               // Get the code
            for(int i = 0; i<my_str.length(); i++)
            {
                buf_str += to_bin_str(my_str[i]);
                if(i%2 == 1)
                {
                    buf_str = reverse_str(buf_str);
                    str_code += buf_str;
                    buf_str = "";
                }
            }
            node get_char = my_list->element[0];
            string out_str;
            int have_done = 0;
            for (char this_code : str_code)
            {
                if(this_code =='0')
                {
                    get_char = get_char->left;
                    if(get_char->left==nullptr  &&  get_char->right==nullptr )
                    {
                        if(have_done == count_total){break;}
                        out_str += get_char->character;
                        have_done += 1;
                        get_char = my_list->element[0];
                    }
                }
                else if(this_code =='1')
                {
                    get_char = get_char->right;
                    if(get_char->left==nullptr &&  get_char->right==nullptr )
                    {
                        if(have_done == count_total){break;}
                        out_str += get_char->character;
                        have_done += 1;
                        get_char = my_list->element[0];
                    }
                }
            }

            char each_char [1];
            for(char emm : out_str)
            {
                each_char[0]=emm;
                fwrite(each_char,1, 1, stdout);
            }
        }
        free_s(my_list);
    }

    return 0;
}