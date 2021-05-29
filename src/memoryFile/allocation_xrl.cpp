#include "allocation_info.cpp"
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
using namespace std;

void show_allocation(vector<allocation_info> allocation_list);
void user_call(vector<allocation_info> origin_alloc_info);

int main(){
    vector<allocation_info> origin_alloc_info;
    // process_show(origin_proc_list);
    cout << BOLDBLACK << "Programme Start" << RESET << endl;

    // vector<allocation_info> allocation_list;
    int num = 5;
    
    for (size_t i = 0; i < num; i++)
    {
        /* code */
        
        void * address;
        
        size_t size = (i+1)*4;
        char * s[] = {"1234", "5678", "0000"};
        char ** stacktrace = s;
        size_t depth = 3;
        pthread_t thread_id = i;
        
        allocation_info a(address, size, stacktrace, depth, thread_id);
        origin_alloc_info.push_back(a);
    }
    
    show_allocation(origin_alloc_info);

    user_call(origin_alloc_info);

    cout << BOLDRED << "end with failure" << RESET << endl;
    return 0;
}

void user_call(vector<allocation_info> origin_alloc_info){
    while (true) {
        char mode;
        cout << "Input [a] or [b] or [c]:\n[a]:\tspecific TID info\n[b]:\tsorted memory leak record"
             << endl;
        cin >> mode;
        cout << GREEN << "Your input is " << RESET << mode << endl;
        vector<allocation_info> origin;
        vector<allocation_info> result;
        vector<allocation_info> ordered;

        if (mode == 'a') {
            int tid;
            cout << "Enter specific mode. Pls input the TID: " << endl;
            cin >> tid;
            cout << GREEN << "Your input is " << RESET << tid << endl;
            while (true) {
                // origin = fetchProcessList();
                result = origin_alloc_info;
                cout << "\033c" << endl;
                cout << BOLDWHITE << "Specific mode, focus on tid = " << tid << RESET << endl;
                show_allocation(origin_alloc_info);
                usleep(1000000);
            }
            break;
        } else if (mode == 'b') {
            /* code */
            char order;
            cout
                    << "Enter sorted mode. Pls select the order:\n[a]:\tAscend TID\n[b]:\tDescend TID\n[c]:\tAscend memory leaking\n[d]:\tDescend memory leaking"
                    << endl;
            cin >> order;
            cout << GREEN << "Your input is " << RESET << order << endl;
            while (true) {
                /* code */
                if (order == 'a') {
                    while (true) {
                        result = origin_alloc_info;
                        cout << "\033c" << endl;
                        cout << BOLDWHITE << "Sorted mode, filter is TID ascending" << RESET << endl;
                        show_allocation(origin_alloc_info);
                        usleep(1000000);
                    }
                    break;
                } else if (order == 'b') {
                    while (true) {
                        result = origin_alloc_info;
                        cout << "\033c" << endl;
                        cout << BOLDWHITE << "Sorted mode, filter is TID descending" << RESET << endl;
                        show_allocation(origin_alloc_info);
                        usleep(1000000);
                    }
                    break;;
                } else if (order == 'c') {
                    while (true) {
                        result = origin_alloc_info;
                        cout << "\033c" << endl;
                        cout << BOLDWHITE << "Sorted mode, filter is memory leak ascending" << RESET << endl;
                        show_allocation(origin_alloc_info);
                        usleep(1000000);
                    }
                    break;
                } else if (order == 'd') {
                    while (true) {
                        result = origin_alloc_info;
                        cout << "\033c" << endl;
                        cout << BOLDWHITE << "Sorted mode, filter is memory leak descending" << RESET << endl;
                        show_allocation(origin_alloc_info);
                        usleep(1000000);
                    }
                    break;
                } else {
                    cout << "Wrong input. Pls input again.";
                }
            }
            break;
        } else {
            cout << "Wrong input. Pls input again.";
        }
    }

}

void show_allocation(vector<allocation_info> allocation_list){
    cout << BOLDMAGENTA << setw(4) << "TID" << setw(12) << "Address" << setw(15) << "Leak Size" << setw(5) << "" << "Stacktrace" << RESET << endl;
    for (size_t i = 0; i < allocation_list.size(); i++)
    {
        /* code */
        cout << setw(4) << allocation_list[i].get_thread_id() << setw(12) << allocation_list[i].get_address() << setw(9) << allocation_list[i].get_size() << " Bytes" << setw(5) << "" << flush;
        vector<string> stacktrace = allocation_list[i].get_stacktrace();
        cout<< "("<<stacktrace.size()<<") :" << flush;
        for (size_t j = 0; j < stacktrace.size(); j++)
        {
            /* code */
            cout<<stacktrace[j]<<" ";
        }
        
        cout<<endl;
    }
    
}