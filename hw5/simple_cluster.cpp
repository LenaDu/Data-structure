#include <iostream>
#include <string>
#include <cmath>

using namespace std;

int city_num, road_num, threshold;

class Road
{
public:
    int dest1;
    int dest2;
    int length;
    Road() { dest1=0;dest2=0;length=0; }

    void record(int *array)
    {
        dest1 = array[0]; dest2 = array[1]; length = array[2];
    }

    bool is_less(Road &road)
    {
        if(length<road.length){return true;}
        else if(length==road.length)
        {
            if(dest1<road.dest1){return true;}
            if(dest1==road.dest1)
            {
                if(dest2<road.dest2){return true;}
            }
        }
        return false;
    }

    bool is_greater(Road &road)
    {
        if(length>road.length){return true;}
        else if(length==road.length)
        {
            if(dest1>road.dest1){return true;}
            if(dest1==road.dest1)
            {
                if(dest2>road.dest2){return true;}
            }
        }
        return false;
    }

    void clear()
    { dest1=0;dest2=0;length=0;}
};

void sort_min(int *a, int *b)
{
    if(*a<=*b){return;}
    else
    {
        int temp = *a;
        *a = *b;
        *b = temp;
    }
}
class Min_heap
{
public:
    Road *min;
    int min_layer = 0; // # of layer
    int min_len = 0;    // # of min heap
    bool full_min = false;
    Min_heap()
    {
        min = new Road[road_num+1];
    }
    ~Min_heap(){delete[] min;}
    void add(Road &road)
    {
        min[min_len+1] = road;
        min_len += 1;
        if(full_min)
        {
            min_layer += 1;
            full_min = false;
        }
        else
        {
            if(min_len == pow(2,min_layer+1)-1)
            { full_min = true; }
        }
    }

    Road pop()
    {
        Road temp = min[1];
        min[1] = min[min_len];
        min[min_len].clear();
        min_len -= 1;
        if(full_min)
        { full_min = false; }
        if(min_len == pow(2,min_layer)-1){min_layer -=1;full_min = true;}
        return temp;
    }

    void up() // insert
    {
        // min heap
        int i = min_len;
        while (i>1)
        {
            if (i%2 == 0) // left child
            {
                if(min[i].is_less(min[i/2]))
                {
                    Road temp = min[i / 2];
                    min[i / 2] = min[i];
                    min[i] = temp;
                }
                i = i/2;
            }
            else if(i%2 == 1) // right child
            {
                if(min[i].is_less(min[(i-1)/2]))
                {
                    Road temp = min[(i-1)/ 2];
                    min[(i-1)/ 2] = min[i];
                    min[i] = temp;
                }
                i = (i-1)/2;
            }
        }
    }

    void down() // remove
    {
        int i = 1; // start from root
        while(i < min_len/2)
        {
            Road left_child = min[2*i];
            if(2*i>min_len){break;} // reached to leaf
            if(2*i+1>min_len)
            {
                if(min[2*i].is_less(min[i]))
                {
                    Road temp = min[i];
                    min[i] = min[2*i];
                    min[2*i] = temp;
                }
                break;
            }
            else
            {
                Road right_child = min[2*i+1];
                if(left_child.is_less(min[i])||right_child.is_less(min[i]))
                {
                    if (!right_child.is_less(left_child))
                    {
                        Road temp = min[i];
                        min[i] = min[2 * i];
                        min[2 * i] = temp;
                        i = 2 * i;
                    }
                    else if (right_child.is_less(left_child))
                    {
                        Road temp = min[i];
                        min[i] = min[2 * i + 1];
                        min[2 * i + 1] = temp;
                        i = 2 * i + 1;
                    }
                }
                else{break;}
            }
        }
    }

};

class Max_heap
{
public:
    Road *max;
    int max_layer = 0; int max_len = 0;    // # of max heap
    bool full_max = false;
    Max_heap()
    {
//        max = new Road[road_num];
    }
    ~Max_heap()
    {
        delete[] max;
    }
    void add(Road road)
    {
        auto temp = new Road[max_len+2];
        for(int i=0; i<max_len; i++)
        {
            temp[i+1] = max[i+1];
        }
        delete[] max;
        max = temp;
        max[max_len+1] = road;
        max_len += 1;
        if(full_max)
        {
            max_layer += 1;
            full_max = false;
        }
        else
        {
            if(max_len == pow(2,max_layer+1)-1)
            { full_max = true; }
        }
    }
    Road pop()
    {
        Road temp = max[1];
        max[1] = max[max_len];
        max_len -= 1;
        if(full_max)
        { full_max = false; }
        if(max_len == pow(2,max_layer)-1){max_layer -=1;full_max = true;}
        return temp;
    }

    void up() // insert
    {
        // max heap
        int i = max_len;
        while (i>1)
        {
            if (i%2 == 0) // left child
            {
                if(max[i/2].is_less(max[i]))
//                if(max[i/2]<max[i])
                {
                    Road temp = max[i / 2];
                    max[i / 2] = max[i];
                    max[i] = temp;
                }
                i = i/2;
            }
            else if(i%2 == 1) // right child
            {
                if(max[(i-1)/2].is_less(max[i]))
//                if(max[(i-1)/2]<max[i])
                {
                    Road temp = max[(i-1)/ 2];
                    max[(i-1)/ 2] = max[i];
                    max[i] = temp;
                }
                i = (i-1)/2;
            }
        }
    }

    void down() // remove
    {
        int i = 1;
        while(i<max_len/2)
        {
            if(2*i>max_len){break;}
//            if(max[i]>max[2*i]){break;}
            Road left_child = max[2*i];
            if(2*i+1>max_len && max[i].is_less(max[2*i]))
//            if(2*i+1>max_len && max[i]<max[2*i])
            {
                Road temp = max[i];
                max[i] = max[2*i];
                max[2*i] = temp;
                break;
            }
            Road right_child = max[2*i+1];
            if  (max[i].is_less(left_child) || max[i].is_less(right_child))
            {
                if (!left_child.is_less(right_child)) {
                    Road temp = max[i];
                    max[i] = max[2 * i];
                    max[2 * i] = temp;
                    i = 2 * i;
                }
                else if (left_child.is_less(right_child))
                {
                    Road temp = max[i];
                    max[i] = max[2 * i + 1];
                    max[2 * i + 1] = temp;
                    i = 2 * i + 1;
                }
            }
            else{break;}
        }
    }

};

class Region
{
public:
    Road *roads;    // Pointer to roads
    int *city;
    int num;        // Number of cities
    int roads_num;
    Region()
    {
        city = new int[city_num];
        num = 0;
        roads_num = 0;
    }
    ~Region()
    {
        delete[] city;
    }
    void add_city(int &c)
    {
        city[num] = c;
        num += 1;
    }

    void clear()
    {
        num = 0;
    }

    bool city_existed(int &c)
    {
        for(int i=0; i<num; i++)
        {
            if(city[i]==c)
            {return true;}
        }
        return false;
    }

    void add_road(Road r)
    {
        auto temp = new Road[roads_num+1];
        for(int i=0; i<roads_num; i++)
        {
            temp[i] = roads[i];
        }
        delete[] roads;
        roads = temp;
        roads[roads_num] = r;
        roads_num += 1;

    }

    void merge(Region *region)
    {
        int total_num = roads_num+region->roads_num;
        auto temp = new Road[total_num];
        for(int i=0; i<roads_num; i++)
        {
            temp[i] = roads[i];
        }
        for(int i=roads_num,j=0; i<total_num; i++,j++)
        {
            temp[i] = roads[j];
        }
        for(int i=0; i<region->num; i++)
        {
            if(city_existed(region->city[i]))
            {continue;}
            else
            { add_city(region->city[i]); }
        }
//        for(int i=0; i<roads_num; i++)
//        {
//            roads->add(region->roads->pop());
//            region->roads->down();
//            roads->up();
//        }
    }
};

class Set
{
public:
    Region *set;
    int count;
    int total;
    int remain;

    Set()
    {
        set = new Region[city_num];
        count=0;
    }
    ~Set()
    {
        delete[] set;
    }
    void add_road(Road &r)
    {
        int merge_factor = -1;
        int merge_index = -1;
        int merge_index_2 = -1;
        bool need_new = true;
        for(int i=0; i<count; i++)
        {
            if(set[i].city_existed(r.dest1))    // d1 existed
            {
                if(set[i].city_existed(r.dest2)){return;}   // both existed
                else
                {
                    set[i].add_city(r.dest2);               // d1 existed, d2 does not exist
                    set[i].add_road(r);
                    merge_factor = r.dest2;
                    merge_index = i;
                    need_new = false;
                    remain -= 1;
                    break;
                }
            }
            else if(set[i].city_existed(r.dest2))           // d1 does not exist, d2 existed
            {
                set[i].add_city(r.dest1);
                set[i].add_road(r);
                merge_factor = r.dest1;
                merge_index = i;
                need_new = false;
                remain -= 1;
                break;
            }
        }

        if(!need_new)
        {
            if(count > 0)
            {
                for(int i=0; i<count; i++)
                {
                    if(i == merge_index){continue;}
                    if(set[i].city_existed(merge_factor))
                    {
                        merge_index_2 = i;
                    }
                }
                if(merge_index_2 == -1)
                { return; }
                sort_min(&merge_index, &merge_index_2);
                set[merge_index].merge(&set[merge_index_2]);
                set[merge_index_2].clear();
                count -= 1;
            }
        }
            // A completely new region
        else if(need_new)
        {
            count += 1;
            set[count-1].add_city(r.dest1);
            set[count-1].add_city(r.dest2);
            set[count-1].add_road(r);
            remain -= 1;
        }
    }
    void add_sortset(int* sort_set)
    {
        for(int i=0; i<count; i++)
        {
            sort_set[2*i] = i; sort_set[2*i+1] = set[i].roads_num;
        }

    }

    void qs_set(int* arr, int lo, int hi)
    {
        if(lo<hi)
        {
            int p = parti(arr, lo, hi);
            qs_set(arr, lo, p-1);
            qs_set(arr, p+1, hi);
        }
    }
    void swap(int* arr, int i1, int i2)
    {
        int temp1 = arr[2*i1]; int temp2 = arr[2*i1+1];
        arr[2*i1] = arr[2*i2]; arr[2*i1+1] = arr[2*i2+1];
        arr[2*i2] = temp1;     arr[2*i2+1] = temp2;
    }

    int parti(int* arr, int lo, int hi)
    {
        int pivot = arr[2*hi+1];
        int i = lo-1;
        for(int j=lo; j<=hi-1; j++)
        {
            if(arr[2*j+1]<pivot)
            {
                i=i+1;
                swap(arr, i, j);
            }
        }
        swap(arr,i+1,hi);
        return i+1;
    }


};




bool enough(Set *subset, int city_num)
{
    int cities = 0;
    for(int i=0; i<subset->count; i++)
    {
        cities += subset->set[i].num;
    }
    return city_num <= cities;
}

int main() {   // destination1, destination2, road length
    cin>>city_num>>road_num>>threshold;
    int road[3] = {};
    Min_heap heap;
    // Store and sort the roads
    for(int i=0; i<road_num; i++)
    {
        cin>>road[0]>>road[1]>>road[2];
        sort_min(&road[0],&road[1]);
        Road temp; temp.record(road);
        heap.add(temp);
        heap.up();
    }
    // Initialize and create
    Set bigset; bigset.total = city_num;bigset.remain = city_num;

    int times = heap.min_len;
    for(int i=0; i<times; i++)
    {
        Road temp = heap.pop();
        heap.down();
        bigset.add_road(temp);
        if(bigset.remain == threshold){break;}
    }
    int sort_set[2*bigset.count];
    bigset.add_sortset(sort_set);
    bigset.qs_set(sort_set,0,bigset.count-1);

    // Pop and output
    cout<<"["<<endl;
    for(int i=0; i<threshold; i++)
    {
        if(i<bigset.count)
        {
            cout<<"["<<endl;
            int index =sort_set[2*(bigset.count-i-1)];
            int total_road = bigset.set[index].roads_num;
            for(int j=0; j<total_road; j++)
            {
                Road temp = bigset.set[index].roads[j];
//                bigset.set[index].roads->down();
                cout<<"["<<temp.dest1<<","<<temp.dest2<<","<<temp.length<<"]";
                if(j<total_road-1){cout<<","<<endl;}
                else{cout<<endl;}
            }
            if(i<bigset.count-1){cout<<"],"<<endl;}
            else
            {
                if(bigset.count==threshold){ cout<<"]"<<endl;}
                else{cout<<"],"<<endl;}
            }
        }
        else
        {
            if(i<threshold-1){cout<<"[\n],"<<endl;}
            else{cout<<"[\n]"<<endl;}
        }
    }

    cout<<"]"<<endl;
    return 0;
}