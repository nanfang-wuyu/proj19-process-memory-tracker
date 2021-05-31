#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
// #include "process.h"

using namespace std;

struct process {
    /* data */
    int pid;
    int memory;
};

void show(vector<process> list);
vector<process> process_sort(vector<process> list, string target, bool direction);
vector<process> process_top(vector<process> list, int num);
vector<process> process_specific(vector<process> list, int pid);

bool com_pid_pos(const process &p1, const process &p2){
    return p1.pid > p2.pid;
}

bool com_pid_neg(const process &p1, const process &p2){
    return p1.pid < p2.pid;
}

bool com_mem_pos(const process &p1, const process &p2){
    return p1.memory > p2.memory;
}

bool com_mem_neg(const process &p1, const process &p2){
    return p1.memory < p2.memory;
}

int main(){
    vector<process> list;

    process p1;
    p1.pid = 1;
    p1.memory = 89;
    list.push_back(p1);
    process p2;
    p2.pid = 459;
    p2.memory = 129;
    list.push_back(p2);
    process p3;
    p3.pid = 872;
    p3.memory = 177;
    list.push_back(p3);
    process p4;
    p4.pid = 880;
    p4.memory = 23599;
    list.push_back(p4);
    process p5;
    p5.pid = 924;
    p5.memory = 10008;
    list.push_back(p5);
    show(list);

    list = process_sort(list, "memory", true);
    show(list);
    return 0;
}

vector<process> process_sort(vector<process> list, string target, bool direction){
    if (target=="pid"){
        if (direction){
            sort(list.begin(), list.end(), com_pid_pos);
        }else{
            sort(list.begin(), list.end(), com_pid_neg);
        }
    }
    else if (target=="memory"){
        if (direction){
            sort(list.begin(), list.end(), com_mem_pos);
        }else{
            sort(list.begin(), list.end(), com_mem_neg);
        }
    }
    return list;
}

vector<process> process_top(vector<process> list, int num){
    int count = 0;
    for (vector<process>::iterator it = list.begin(); it!=list.end();){
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

vector<process> process_specific(vector<process> list, int pid){
    int count = 0;
    for (vector<process>::iterator it = list.begin(); it!=list.end();){
        if (list[count].pid!=pid)
        {
            /* code */
            it = list.erase(it);
        }
        else
        {
            it++;
        }
        count++;
    }
    return list;
}

void show(vector<process> list){
    for (int i = 0; i<list.size(); i++){
        cout<<"pid: "<<list[i].pid<<"memory: "<<list[i].memory<<endl;
    }
}

