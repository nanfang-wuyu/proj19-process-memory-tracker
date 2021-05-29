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

const wchar_t TAB_THIN_VERTICAL = L'\u2502';

using namespace std;


int main() {
    cout << BOLDCYAN << "Programme Start" << RESET << endl;
    user_call();
    cout << "end with failure" << endl;
}

void user_call() {
    while (true) {
        char mode;
        cout << "Input [a] or [b] or [c]:\n[a]:\tspecific TID info\n[b]:\ttotal memory leak record\n[c]:\tfind the top N memory leaking TID"
             << endl;
        cin >> mode;
        cout << GREEN << "Your input is " << RESET << mode << endl;
        vector<int> origin;
        vector<int> result;
        vector<int> ordered;
        if (mode == 'a') {
            int pid;
            cout << "Enter specific mode. Pls input the PID: " << endl;
            cin >> pid;
            cout << GREEN << "Your input is " << RESET << pid << endl;
            while (true) {
                cout << "\033c" << endl;
                cout << MAGENTA << setw(15) << "PID" << setw(15) << "MEM" << RESET << endl;
                for (int i = 0; i < result.size(); ++i) {
                    // cout << setw(15) << result[i].pid << setw(15) << result[i].memory << endl;
                }
                usleep(1000000);
            }
            break;
        }
}