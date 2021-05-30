#include <iostream>
#include <vector>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <iomanip>
#include <wchar.h>
#include <locale.h>
#include <unistd.h>
#include <string>
#include <algorithm>
#include <sstream>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

const wchar_t TAB_THIN_VERTICAL = L'\u2502';

using namespace std;

struct process {
    /* data */
    int pid;
    int memory;
};

void listFiles(const char *dir);

void scanDirectorFromPath(const char *path);

bool isNum(string str);

void user_call();

vector<process> process_sort(vector<process> list, string target, bool direction);

vector<process> process_top(vector<process> list, int num);

vector<process> process_specific(vector<process> list, int pid);

vector<process> fetchProcessList();

string _commend;
vector <string> dirs;
vector <string> process_dirs;
vector <process> process_list;

#include <sstream>

bool com_pid_pos(const process &p1, const process &p2) {
    return p1.pid > p2.pid;
}

bool com_pid_neg(const process &p1, const process &p2) {
    return p1.pid < p2.pid;
}

bool com_mem_pos(const process &p1, const process &p2) {
    return p1.memory > p2.memory;
}

bool com_mem_neg(const process &p1, const process &p2) {
    return p1.memory < p2.memory;
}

vector<process> process_sort(vector<process> list, string target, bool direction) {
    if (target == "pid") {
        if (direction) {
            sort(list.begin(), list.end(), com_pid_pos);
        } else {
            sort(list.begin(), list.end(), com_pid_neg);
        }
    } else if (target == "memory") {
        if (direction) {
            sort(list.begin(), list.end(), com_mem_pos);
        } else {
            sort(list.begin(), list.end(), com_mem_neg);
        }
    }
    return list;
}

vector<process> process_top(vector<process> list, int num) {
    int count = 0;
    for (vector<process>::iterator it = list.begin(); it != list.end();) {
        count++;
        if (count > num) {
            /* code */
            it = list.erase(it);
        } else {
            it++;
        }
    }
    return list;
}

vector<process> process_specific(vector<process> list, int pid) {
    int count = 0;
    for (vector<process>::iterator it = list.begin(); it != list.end();) {
        if (list[count].pid != pid) {
            /* code */
            it = list.erase(it);
        } else {
            it++;
        }
        count++;
    }
    return list;
}

void process_show(vector<process> list){
    cout<<"-----begin-----"<<endl;
    for (int i = 0; i<list.size(); i++){
        cout<<"pid: "<<list[i].pid<<"memory: "<<list[i].memory<<endl;
    }
    cout<<"-----end-----"<<endl;
}




vector<process> fetchProcessList(){
    process_dirs.clear();
    process_list.clear();

//    _commend = "cat /proc";
//    system("cat /proc");

//    char tar_dir[] = "D:\\Clion_projects\\";
    char tar_dir[] = "/proc/";

//    strcat(tar_dir, "*.*");
//    listFiles(tar_dir);
    scanDirectorFromPath(tar_dir);
    // cout << "--------- dirs ---------" << endl;
//    for (auto a : dirs) cout << a << endl;
    for (int i = 0;i<dirs.size();++i){
//        cout << dirs[i] << endl;
        if (isNum(dirs[i])) {
//            cout << "This is a folder for process " << dirs[i] << endl;
            process_dirs.push_back(dirs[i]);
        }else {
//            cout << "This is not a process " << endl;
        }
    }

    ifstream infile;

    for (int i = 0;i<process_dirs.size();i++){

        string pid = process_dirs[i];
        infile.open(("/proc/"+pid+"/statm").c_str(), ios::in);

        if(!infile.is_open ()){
            cout << "Open file failure" << endl;
            vector<process> null;
            return null;
        }


        string temp;
        while(getline(infile,temp)){
            istringstream str(temp);
            string data;
            vector<string> datas;
            while (str >> data) {
                datas.push_back(data);
            }
//            cout << "Process " << pid << " memory is " << datas[1] << "*4 KB" << endl;
            int i_pid, i_mem;
            istringstream new_pid(pid);
            new_pid >> i_pid;
            istringstream new_mem(datas[1]);
            new_mem >> i_mem;
            process_list.push_back(process{i_pid, i_mem});
        }
//
//        while (!infile.eof())            // 若未到文件结束一直循环
//        {
//
//        }
        infile.close();
//        break;


    }

    // for (int i = 0;i<process_list.size();++i) {
    //     cout << process_list[i].pid << " " << process_list[i].memory << endl;
    // }

//    _commend = "cat /proc/PID/status";
//    system("cat /proc/PID/status");

    return process_list;
}

void scanDirectorFromPath(const char* path)
{
    dirs.clear();
    DIR *dir;
    dir = opendir(path);
    if(dir == NULL){
        cout<<"Open Dir failed!"<<endl;
        return;
    }
    struct dirent *dirent_;
    while(dirent_ = readdir(dir))
    {
        string dir_name = dirent_ ->d_name;
        //ignore . or .. file
        if(dir_name  == "." || dir_name  == ".."){
            continue;
        }else if (dir_name .find(".cms") != string::npos){
            size_t start_pos = dir_name.find_first_of("_");
            int appID = atoi(dir_name.substr(0,start_pos ).c_str());
            size_t end_pos = dir_name.find_last_of("_");
            int orgID = atoi(dir_name.substr(start_pos +1,end_pos ).c_str());
            cout<< "CMS File's appID:"<<appID <<endl;
            cout<< "CMS File's orgID:"<<orgID <<endl;
        }
//        cout<< " File's Name:"<<dir_name <<endl;
        dirs.push_back(dir_name);


    }
}

bool isNum(string str)
{
    for (int i = 0; i < str.size(); i++)
    {
        int tmp = (int)str[i];
        if (tmp >= 48 && tmp <= 57)
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    return true;
}


int main() {
    vector<process> origin_proc_list = fetchProcessList();
    // process_show(origin_proc_list);
    cout << BOLDCYAN << "Programme Start" << RESET << endl;
    user_call();
    cout << "end with failure" << endl;
}

void user_call() {
    while (true) {
        char mode;
        cout << "Input [a] or [b] or [c]:\n[a]:\tspecific\n[b]:\tsorted\n[c]:\tfind the top N memory cosumed TID"
             << endl;
        cin >> mode;
        cout << GREEN << "Your input is " << RESET << mode << endl;
        vector<process> origin;
        vector<process> result;
        vector<process> ordered;
        if (mode == 'a') {
            int tid;
            cout << "Enter specific mode. Pls input the TID: " << endl;
            cin >> tid;
            cout << GREEN << "Your input is " << RESET << tid << endl;
            while (true) {
                origin = fetchProcessList();
                result = process_specific(origin, tid);
                cout << "\033c" << endl;
                cout << BOLDWHITE << "Specific mode on TID " << tid << RESET << endl;
                cout << BOLDMAGENTA << setw(15) << "TID" << setw(15) << "MEM" << RESET << endl;
                for (int i = 0; i < result.size(); ++i) {
                    cout << setw(15) << result[i].pid << setw(15) << result[i].memory << endl;
                }
                usleep(1000000);
            }
            break;
        } else if (mode == 'b') {
            /* code */
            char order;
            cout
                    << "Enter sorted mode. Pls select the order:\n[a]:\tAscend TID\n[b]:\tDescend TID\n[c]:\tAscend memory consuming\n[d]:\tDescend memory consuming"
                    << endl;
            cin >> order;
            cout << GREEN << "Your input is " << RESET << order << endl;
            while (true) {
                /* code */
                if (order == 'a') {
                    while (true) {
                        origin = fetchProcessList();
                        result = process_sort(origin, "pid", false);
                        cout << "\033c" << endl;
                        cout << BOLDWHITE << "Ascend TID mode" << RESET << endl;
                        cout << BOLDMAGENTA << setw(15) << "TID" << setw(15) << "MEM" << RESET << endl;
                        for (int i = 0; i < result.size(); ++i) {
                            cout << setw(15) << result[i].pid << setw(15) << result[i].memory << endl;
                        }
                        usleep(1000000);
                    }
                    break;
                } else if (order == 'b') {
                    /* code */
                    while (true) {
                        origin = fetchProcessList();
                        result = process_sort(origin, "pid", true);
                        cout << "\033c" << endl;
                        cout << BOLDWHITE << "Descend TID mode" << RESET << endl;
                        cout << BOLDMAGENTA << setw(15) << "TID" << setw(15) << "MEM" << RESET << endl;
                        for (int i = 0; i < result.size(); ++i) {
                            cout << setw(15) << result[i].pid << setw(15) << result[i].memory << endl;
                        }
                        usleep(1000000);
                    }
                    break;
                } else if (order == 'c') {
                    /* code */
                    while (true) {
                        origin = fetchProcessList();
                        result = process_sort(origin, "memory", false);
                        cout << "\033c" << endl;
                        cout << BOLDWHITE << "Ascend memory consuming mode" << RESET << endl;
                        cout << BOLDMAGENTA << setw(15) << "TID" << setw(15) << "MEM" << RESET << endl;
                        for (int i = 0; i < result.size(); ++i) {
                            cout << setw(15) << result[i].pid << setw(15) << result[i].memory << endl;
                        }
                        usleep(1000000);
                    }
                    break;
                } else if (order == 'd') {
                    /* code */
                    while (true) {
                        origin = fetchProcessList();
                        result = process_sort(origin, "memory", true);
                        cout << "\033c" << endl;
                        cout << BOLDWHITE << "Descend memory consuming mode" << RESET << endl;
                        cout << BOLDMAGENTA << setw(15) << "TID" << setw(15) << "MEM" << RESET << endl;
                        for (int i = 0; i < result.size(); ++i) {
                            cout << setw(15) << result[i].pid << setw(15) << result[i].memory << endl;
                        }
                        usleep(1000000);
                    }
                    break;
                } else {
                    cout << "Wrong input. Pls input again.";
                }
            }
            break;
        } else if (mode == 'c') {
            origin = fetchProcessList();
            // process_show(origin);
            /* code */
            int num;
            cout << "Provide top N memory consuming thread. Input N:" << endl;
            cin >> num;
            cout << GREEN << "Your input is " << RESET << num << endl;
            while (true) {
                origin = fetchProcessList();
                ordered = process_sort(origin, "memory", true);
                result = process_top(ordered, num);
                // process_show(origin);
                // process_show(ordered);
                // process_show(result);
                // system("CLS");
                cout << "\033c" << endl;
                // cout<< MAGENTA << setw(5) << "No." << setw(15) << "PID" << setw(15) << "MEM" << RESET << endl;
                // for (int i = 0; i < 10; ++i) {
                //     cout << CYAN << setw(5) << i+1 << RESET << setw(15) << origin[i].pid << setw(15) << origin[i].memory << endl;
                // }
                // cout<< MAGENTA << setw(5) << "No." << setw(15) << "PID" << setw(15) << "MEM" << RESET << endl;
                // for (int i = 0; i < 10; ++i) {
                //     cout << CYAN << setw(5) << i+1 << RESET << setw(15) << ordered[i].pid << setw(15) << ordered[i].memory << endl;
                // }
                cout << BOLDWHITE << "Top " << num << " memory consuming TID" << RESET << endl;
                cout<< BOLDMAGENTA << setw(5) << "No." << setw(15) << "TID" << setw(15) << "MEM" << RESET << endl;
                for (int i = 0; i < result.size(); ++i) {
                    cout << CYAN << setw(5) << i+1 << RESET << setw(15) << result[i].pid << setw(15) << result[i].memory << endl;
                }
                usleep(1000000);
            }
            break;
        } else {
            cout << "Wrong input. Pls input again.";
        }
    }
}