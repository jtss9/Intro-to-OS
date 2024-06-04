#include <iostream>
#include <cstdint>
#include <vector>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <math.h>

using namespace std;

int n, m;
int t_num;
vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1), global_count = 0;
mutex mtx;
vector <thread> t;
int cnt[100]={};
int c=0;

void solve(int index, uint64_t current, int tid) {
    if (index == m) {
        if (current == (one << n) - 1){
            cnt[tid]++;
        }
    } else {
        solve(index + 1, current, tid);
        solve(index + 1, current | subsets[index], tid);
    }
}
void find(int index, uint64_t current){
    if(t_num == 1){
        solve(index, current, 0);
        return;
    }
    if(pow(2, index+1) == t_num){
        t.emplace_back(solve, index+1, current, c++);
        t.emplace_back(solve, index+1, current | subsets[index], c++);
    }
    else{
        find(index+1, current);
        find(index+1, current | subsets[index]);
    }
    if(t.size() == t_num){
        for(auto &th : t) th.join();
        t.clear();
    }
}


int main(int argc, char* argv[]) {
    cin >> n >> m;

    subsets.resize(m);
    for (int i = 0; i < m; i++) {
        int p, temp;
        cin >> p;
        for (int j = 0; j < p; j++) {
            cin >> temp;
            subsets[i] |= (one << temp);
        }
    }
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
    find(0, 0);

    for (int i=0;i<t_num;i++) 
        global_count += cnt[i];
    cout << global_count << endl;
    return 0;
}