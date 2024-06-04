#include<iostream>
#include<vector>
using namespace std;
int main(){
    // direct mapped cache
    int n; cin >> n;    // cache lines / blocks
    int data_sz; cin >> data_sz;    // data size

    vector<int> data(data_sz);
    for(int i = 0; i < data_sz; i++){
        cin >> data[i];
    }
    vector<int> cache(n, -1);

    int hit = 0, miss = 0;
    for(int i = 0; i < data_sz; i++){
        int idx = i%n;
        for(int j = 0; j < n; j++){
            if(cache[j] == data[i]){
                hit++;
                break;
            }
            if(j == n-1){
                miss++;
                cache[idx] = data[i];
            }
        }
    }
    cout << "Total Cache Misses:" << miss << endl;
}