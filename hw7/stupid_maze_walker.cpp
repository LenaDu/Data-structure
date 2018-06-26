#include <iostream>
#include <string>

using namespace std;

class MazePoint
{
public:
    int p_row;
    int p_col;
    char type;
    bool walked;
    MazePoint()
    {
        p_row = -1;
        p_col = -1;
        walked = false;
    }
    void record(int row, int col, char c)
    {
        p_row = row;
        p_col = col;
    }
};

bool is_near(MazePoint &p1, MazePoint &p2)
{
    return abs(p1.p_row-p2.p_row)+abs(p1.p_col-p2.p_col)<=1;
}

class Sublist
{
public:
    MazePoint* route;
    int num;
    int s_capacity;
    int valid; // sign of index
    Sublist()
    {
        num = 0;
        s_capacity = 5;
        route = new MazePoint[s_capacity];
    }
    void add_point_sub(MazePoint &p)
    {
        if(num>=s_capacity)
        {
            s_capacity = 2*s_capacity + 1;
            auto temp = new MazePoint[s_capacity];
            for(int i=0; i<num; i++)
            {
                temp[i] = route[i];
            }
            if( route!= nullptr){delete[] route;}
            route = temp;
        }
        route[num]=p;
        num += 1;
    }
};

class List
{
public:
    Sublist* sublist;
    int num;    // recording num of sublists
    int capacity;
    List()
    {
        num = 0;
        capacity = 1;
        sublist = new Sublist[capacity];
    }
    void record_start(MazePoint &p) // Initialize the start of List
    {
        num += 1;
        sublist[0].add_point_sub(p);
    }
    void new_sublist()
    {
        if(num>=capacity)
        {
            capacity = 2*capacity + 1;
            auto temp = new Sublist[capacity];
            for(int i=0; i<num; i++)
            {
                temp[i] = sublist[i];
            }
            if( sublist!= nullptr){delete[] sublist;}
            sublist = temp;
        }
        num += 1;
    }
    void add_point(MazePoint &p, int len)   // add point the the len_th sublist
    {
        if(len>=num)
        {
            new_sublist();
        }
        sublist[len].add_point_sub(p);
    }
    int can_walk_vertical(MazePoint &p, int len)
    {
        int m = p.p_row;
        int n= p.p_col;
        for(int index=0; index <sublist[len].num;index++)
        {
            MazePoint tmp = sublist[len].route[index];  // Next point
            if(abs(tmp.p_row-m)==1 && tmp.p_col==n && !tmp.walked){return index;}
        }
        return -1;
    }
};

class Solution
{
public:
    MazePoint* points;
    int num;
    int capacity;
    Solution()
    {
        num = 0; capacity = 1; points = new MazePoint[capacity];
    }
    void add_point(MazePoint &p)
    {
        if(num>=capacity)
        {
            capacity = 2*capacity + 1;
            auto temp = new MazePoint[capacity];
            for(int i=0; i<num; i++)
            {
                temp[i] = points[i];
            }
            if( points!= nullptr){delete[] points;}
            points = temp;
        }
        points[num]=p;
        num += 1;
    }
    MazePoint pop()    // return the remained number of solution
    {
        MazePoint return_p = points[num-1];
        points[num-1].p_row=-1;
        points[num-1].p_col=-1;
        num -= 1;
        return return_p;
    }
};

class MazeRow
{
public:
    MazePoint* points;
    int index;
    MazeRow()
    {
        index = 0;
    }
    void record_row(string &s,int col)
    {

        points = new MazePoint[col];
        MazePoint temp;
        for(int i=0; i<col; i++)
        {
            temp.p_col = i;
            temp.walked = false;
            temp.type = s[i];
            points[index] = temp;
            index += 1;
        }
    }
};

class Maze
{
public:
    MazeRow* line;
    int row;
    int col;
    int capacity;
    MazePoint start;
    MazePoint end;

    Maze()
    {
        row = 0;
        col = 0;
        capacity = 10;
        line = new MazeRow[capacity];
    }
    void get_col(string &s)
    {
        col = s.length();
    }
    void add_row(string &s)
    {
        MazeRow new_row;
        new_row.record_row(s,col);
        if(row>=capacity)
        {
            capacity = 2*capacity + 1;
            auto temp = new MazeRow[capacity];
            for(int i=0; i<row; i++)
            {
                temp[i] = line[i];
            }
            if(line!= nullptr){delete[] line;}
            line = temp;
        }
        line[row]=new_row;
        row += 1;
    }
    void mark_walked(int m, int n)
    {
        line[m].points[n].walked = true;
    }
    bool cmp(MazePoint &p1, MazePoint &p2)
    {
        return p1.p_row==p2.p_row && p1.p_col==p2.p_col;
    }
    bool search_around(MazePoint &p, List &l, int len)
    {
        MazePoint temp;
        bool result1=false, result2=false, result3=false, result4=false;
        if(p.p_col>0)   // left
        {
            temp = line[p.p_row].points[p.p_col-1];
            temp.p_row = p.p_row;
            if(temp.type==' ' && !temp.walked)
            {
                temp.walked = true;
                l.add_point(temp, len+1);
                mark_walked(p.p_row, p.p_col-1);
            }
            if(cmp(temp,end)){result1 = true;}
        }
        if(p.p_col<col-1)     // right
        {
            temp = line[p.p_row].points[p.p_col+1];
            temp.p_row = p.p_row;
            if(temp.type==' ' && !temp.walked)
            {
                temp.walked = true;
                l.add_point(temp, len+1);
                mark_walked(p.p_row, p.p_col+1);
            }
            if(cmp(temp,end)){result2 = true;}
        }
        if(p.p_row>0)   // above
        {
            temp = line[p.p_row-1].points[p.p_col];
            temp.p_row = p.p_row-1;
            if(temp.type==' ' && !temp.walked)
            {
                temp.walked = true;
                l.add_point(temp, len+1);
                mark_walked(p.p_row-1, p.p_col);
            }
            if(cmp(temp,end)){result3 = true;}
        }
        if(p.p_row<row-1)   // below
        {
            temp = line[p.p_row+1].points[p.p_col];
            temp.p_row = p.p_row-1;
            if(temp.type==' ' && !temp.walked)
            {
                temp.walked = true;
                l.add_point(temp, len+1);
                mark_walked(p.p_row+1, p.p_col);
            }
            if(cmp(temp,end)){result4 = true;}
        }
        return (result1||result2||result3||result4)||is_near(temp,end);
    }
};

bool not_go_back(List &l, Solution &s)
{
    MazePoint compare = s.points[s.num-1];
    Sublist sub = l.sublist[s.num];
    for(int i=0; i<sub.num; i++)
    {
        if(is_near(sub.route[i],compare)&&!sub.route[i].walked)
        {
            return true;}
    }
    return false;
}

int main() {
    string rd_line;
    int start_col=-1, end_col=-1, start_row=-1, end_row=-1,row = 0;
    bool stop = false;
    Maze maze;
    // to get col
    if(getline(cin, rd_line))
    {
        if(rd_line.find('*')!=-1)
        {
            maze.get_col(rd_line);
            start_col = rd_line.find('*');
            start_row = row;
        }
        if(rd_line.find('+')!=-1)
        {
            end_col = rd_line.find('+');
            end_row = row;
        }
        maze.col = rd_line.length();
        maze.add_row(rd_line);
        row+=1;
    }
    // Continue to get the whole maze
    while(getline(cin, rd_line))    // read start & end & each line
    {
        if(rd_line.find('*')!=-1)
        {
            maze.get_col(rd_line);
            start_col = rd_line.find('*');
            start_row = row;
        }
        if(rd_line.find('+')!=-1)
        {
            end_col = rd_line.find('+');
            end_row = row;
        }
        maze.add_row(rd_line);
        row+=1;
    }

    // Record the start and the end point
    MazePoint start, end;
    start.record(start_row,start_col,'*');
    end.record(end_row,end_col,'+');
    maze.start = start;
    maze.end = end;

    // Initialize the list
    List list;
    list.record_start(start);


    int len = 0;
    // Find all possibilities
    maze.search_around(start,list,len);
    while(!stop && len<list.num)
    {
        int i;
        for(i=0; i<list.sublist[len].num; i++)
        {
            stop = maze.search_around(list.sublist[len].route[i],list,len);
            if(is_near(list.sublist[len].route[i],end)){break;}
            if(stop){break;}
        }
        if(is_near(list.sublist[len].route[i],end)){break;}
        len += 1;
    }
    if(list.sublist[len].num==0){len -= 1;}
    Sublist temp_sub = list.sublist[len-1];
    bool no_sol = true;
    for(int i=0; i<temp_sub.num; i++)
    {
        if(is_near(temp_sub.route[i],end))
        {
            no_sol = false;
            break;
        }
    }
    // Case that no solution
    if(no_sol&&len==list.num){cout<<"NO SOLUTION";return 0;}

    list.sublist[len+1].num=0;
    list.add_point(end,len+1);

    for(int i=0; i<=len+1; i++)
    {
        list.sublist[i].valid = list.sublist[i].num; // Initialize the valid num
        for(int j=0; j<list.sublist[i].num; j++)
        {
            list.sublist[i].route[j].walked = false;
        }
    }


    // Draw the solution
    Solution solution;
    solution.add_point(start);
    for(int i=1; i<=len+1; i++)
    {
        Sublist sublist = list.sublist[i];
        if(!not_go_back(list, solution))
        {
            while(!not_go_back(list, solution))     // Have to go back
            {
                solution.pop(); // Pop from solution list
            }
            i = solution.num-1;
            continue;
        }
        if(sublist.valid == 1)  // Only one valid point left
        {
            for(int j=0; j<sublist.num; j++)    // Search for which is the valid one
            {
                if(!sublist.route[j].walked)
                {
                    list.sublist[i].route[j].walked = true;
                    list.sublist[i].valid -= 1;
                    solution.add_point(list.sublist[i].route[j]);
                }
            }
        }
        else if (sublist.valid > 1) // Not only one
        {
            int ver_index = list.can_walk_vertical(solution.points[solution.num-1],i);
            if(ver_index!=-1)   // Can walk vertically
            {
                list.sublist[i].route[ver_index].walked = true;
                list.sublist[i].valid -= 1;
                solution.add_point(list.sublist[i].route[ver_index]);
            }
            else    // Cannot walk vertically
            {
                for(int j=0; j<sublist.num; j++)
                {
                    if(is_near(sublist.route[j],solution.points[solution.num-1]) && !sublist.route[j].walked)
                    {
                        list.sublist[i].route[j].walked = true;
                        list.sublist[i].valid -= 1;
                        solution.add_point(list.sublist[i].route[j]);
                    }
                }
            }
        }
    }

    for(int i=0; i<solution.num; i++)
    {
        MazePoint temp = solution.points[i];
        maze.line[temp.p_row].points[temp.p_col].type = '.';
    }

    //Display maze
    for(int i=0; i<maze.row; i++)
    {
        for(int j=0; j<maze.col; j++)
        {
            cout<<maze.line[i].points[j].type;
        }
        cout<<endl;
    }
    cout<<len+1<<endl;

    return 0;
}