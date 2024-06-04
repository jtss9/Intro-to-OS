#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

int n, global_count = 0;
mutex mtx;

bool isPrime(int num){
    if(num == 1) return false;
    for(int i = 2; i * i <= num; i++){
        if(num % i == 0) return false;
    }
    return true;
}

void count_prime(int start, int end){
    int local_count = 0;
    for(int i=start;i<=end;i++){
        if(isPrime(i)) local_count++;
    }
    lock_guard<mutex> lock(mtx);
    global_count += local_count;
}

int main(int argc, char* argv[]){
    cin >> n;
    int t_num = 1;
    int opt;
    while((opt = getopt(argc, argv, "t:")) != -1){
        switch(opt){
            case 't':
                t_num = atoi(optarg);
                break;
            default:
                break;
        }
    }
    vector <thread> t;
    int seg_sz = n / t_num;
    for(int i=0;i<t_num;i++){
        int start = i*seg_sz+1;
        int end = (i==t_num-1)?n:(i+1)*seg_sz;
        t.emplace_back(count_prime, start, end);
    }
    for(auto &th : t) th.join();

    cout << global_count << endl;
    return 0;
}