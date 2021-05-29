
#include<cstdlib>
#include<string>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include <unistd.h>

using namespace std;

const char *make_clean_ = "make clean";
const char *make_ = "make";
const char *export_ = "export LD_PRELOAD=./leakfinder.so";
const char *ls_ = "ls";
const char *terminal_ = "gnome-terminal -e ./export_ld";

void system_cmd(const char *s);

int main(){

//    system_cmd(ls_);
    system_cmd(make_clean_);
    system_cmd(make_);
    sleep(5);
    system_cmd(export_);
//    system_cmd(terminal_);

}

void system_cmd(const char *s){
    try {
        system(s);
    } catch (exception e){
        cerr << "wrong command " << s << endl;
    }
}