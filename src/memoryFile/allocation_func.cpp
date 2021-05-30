#include "allocation_info.cpp"

struct thread
{
    /* data */
    pthread_t thread_id;
    size_t total_leak_size;
    size_t leak_num;
};

vector<thread> thread_count(vector<allocation_info> list){
    vector<thread> threads;
    

    return threads;
}

bool com_a_size_pos(const allocation_info &a1, const allocation_info &a2){
    return a1.get_size() > a2.get_size();
}

bool com_a_size_neg(const allocation_info &a1, const allocation_info &a2){
    return a1.get_size() < a2.get_size();
}

bool com_a_tid_pos(const allocation_info &a1, const allocation_info &a2){
    return a1.get_thread_id() > a2.get_thread_id();
}

bool com_a_tid_neg(const allocation_info &a1, const allocation_info &a2){
    return a1.get_thread_id() < a2.get_thread_id();
}

bool com_t_size_pos(const thread &t1, const thread &t2){
    return t1.total_leak_size > t2.total_leak_size;
}

bool com_t_size_neg(const thread &t1, const thread &t2){
    return t1.total_leak_size < t2.total_leak_size;
}

bool com_t_tid_pos(const thread &t1, const thread &t2){
    return t1.thread_id > t2.thread_id;
}

bool com_t_tid_neg(const thread &t1, const thread &t2){
    return t1.thread_id < t2.thread_id;
}

bool com_t_num_pos(const thread &t1, const thread &t2){
    return t1.leak_num > t2.leak_num;
}

bool com_t_num_neg(const thread &t1, const thread &t2){
    return t1.leak_num < t2.leak_num;
}

vector<allocation_info> allocation_sort(vector<allocation_info> list, string target, bool direction){
    if (target=="size")
    {
        if (direction){
            sort(list.begin(), list.end(), com_a_size_pos);
        }else{
            sort(list.begin(), list.end(), com_a_size_neg);
        }
    }
    else if (target=="tid")
    {
        if (direction){
            sort(list.begin(), list.end(), com_a_tid_pos);
        }else{
            sort(list.begin(), list.end(), com_a_tid_neg);
        }
    }
    return list;
}

vector<thread> thread_sort(vector<thread> list, string order, bool direction){
    if (order=="size")
    {
        if (direction){
            sort(list.begin(), list.end(), com_t_size_pos);
        }else{
            sort(list.begin(), list.end(), com_t_size_neg);
        }
    }
    else if (order=="tid")
    {
        if (direction){
            sort(list.begin(), list.end(), com_t_tid_pos);
        }else{
            sort(list.begin(), list.end(), com_t_tid_neg);
        }
    }
    else if (order=="num")
    {
        if (direction){
            sort(list.begin(), list.end(), com_t_num_pos);
        }else{
            sort(list.begin(), list.end(), com_t_num_neg);
        }
    }
    return list;
}

vector<allocation_info> allocation_add(vector<allocation_info> list, allocation_info allocation, string order, bool direction){
    list.push_back(allocation);
    return allocation_sort(list, order, direction);
}

vector<thread> thread_add(vector<thread> t_list, allocation_info allocation, string order, bool direction){
    pthread_t tid = allocation.get_thread_id();
    size_t leak_size = allocation.get_size();
    int hit = -1;
    for (int i=0; i<t_list.size(); i++){
        if (t_list[i].thread_id == tid)
        {
            hit = i;
        }
    }
    if (hit == -1){
        thread t;
        t.thread_id = allocation.get_thread_id();
        t.total_leak_size = allocation.get_size();
        t.leak_num = 1;
        t_list.push_back(t);
    }
    else{
        t_list[hit].total_leak_size += leak_size;
        t_list[hit].leak_num += 1;
    }
    return thread_sort(t_list, order, direction);
}

vector<allocation_info> allocation_top(vector<allocation_info> list, int num){
    int count = 0;
    for (vector<allocation_info>::iterator it = list.begin(); it!=list.end();){
        count++;
        if (count > num)
        {
            /* code */
            it = list.erase(it);
        }
        else
        {
            it++;
        }
    }
    return list;
}

vector<thread> thread_top(vector<thread> list, int num){
    int count = 0;
    for (vector<thread>::iterator it = list.begin(); it!=list.end();){
        count++;
        if (count > num)
        {
            /* code */
            it = list.erase(it);
        }
        else
        {
            it++;
        }
    }
    return list;
}

// void show_allocation(vector<allocation_info> allocation_list){
//     cout << "TID " << "Address " << "Leak Size " << "Stacktrace " << endl;
//     for (size_t i = 0; i < allocation_list.size(); i++)
//     {
//         /* code */
//         cout << allocation_list[i].get_thread_id()<< " " << allocation_list[i].get_address() << " " << allocation_list[i].get_size() << " Bytes ";
//         vector<string> stacktrace = allocation_list[i].get_stacktrace();
//         cout<< "("<<stacktrace.size()<<") : ";
//         for (size_t j = 0; j < stacktrace.size(); j++)
//         {
//             /* code */
//             cout<<stacktrace[j]<<" ";
//         }
//         cout<<endl;
//     }
// }

// void show_thread(vector<thread> thread_list){
//     cout << "TID " << "size " << "num " << endl;
//     for (size_t i = 0; i < thread_list.size(); i++)
//     {
//         /* code */
//         cout << thread_list[i].thread_id << " " << thread_list[i].total_leak_size << " " << thread_list[i].leak_num << endl;
//     }
// }

void save_allocation(vector<allocation_info> list, string path){
    ofstream file(path);
    if (file.is_open()){
        file << "threadID,size,address,stacktrace" << endl;
        for (int i = 0; i<list.size(); i++){
            file << list[i].get_thread_id() << "," << list[i].get_size() << "," << list[i].get_address() << ",";
            for (int j = 0; j<list[i].get_stacktrace().size(); j++){
                file << list[i].get_stacktrace()[j] << " ";
            }
            file << endl;
        }
    }
}

void save_thread(vector<thread> list, string path){
    ofstream file(path);
    if (file.is_open()){
        file <<"threadID,size,number" << endl;
        for (int i = 0; i<list.size(); i++){
            file << list[i].thread_id << "," << list[i].total_leak_size << "," << list[i].leak_num << endl;
        }
    }
}

// int main(){
//     vector<allocation_info> allocation_list;
//     vector<thread> thread_list;
//     int num = 5;
//     for (size_t i = 0; i < num; i++)
//     {
//         /* code */
        
//         void * address;
        
//         size_t size = (i+1)*4;
//         char * s[] = {"1234", "5678", "0000"};
//         char ** stacktrace = s;
//         size_t depth = 3;
//         pthread_t thread_id = i%3;
        
//         allocation_info a(address, size, stacktrace, depth, thread_id);

//         string order = "tid";
//         bool direction = true;
//         // allocation_list.push_back(a);
//         allocation_list = allocation_add(allocation_list, a, order, direction);
//         thread_list = thread_add(thread_list, a, order, direction);
//     }
//     show_allocation(allocation_list);
//     show_thread(thread_list);
//     save_allocation(allocation_list, "/home/sakura/project/allocation.csv");
//     save_thread(thread_list, "/home/sakura/project/threads.csv");
//     return 0;
// }
