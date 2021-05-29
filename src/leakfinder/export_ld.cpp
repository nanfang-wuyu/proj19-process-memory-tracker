
#include<cstdlib>
#include<string>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>

const char *export_ = "export LD_PRELOAD=./leakfinder.so";

int main(){
    system(export_);
}
