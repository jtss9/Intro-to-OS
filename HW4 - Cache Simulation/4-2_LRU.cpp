#include<iostream>
#include<vector>
using namespace std;

int main(){
    // k-way set associative cache w/ LRU
    int n, k; cin >> n >> k;    // cache lines / blocks, k-way
    int data_sz; cin >> data_sz;    // data size
    vector<int> data(data_sz);
    for(int i = 0; i < data_sz; i++)    cin >> data[i];
    vector<vector<pair<int, int>>> cache(n/k, vector<pair<int, int>>(k, make_pair(-1, -1)));

    // cache[i][j] = (data, LRU)
    // LRU = 0: most recently used
    // LRU = -1: empty
    int hit = 0, miss = 0;
    for(int i = 0; i < data_sz; i++){
        bool is_hit = false;
        // check if data[i] is in cache
        for(int j=0;j<n/k;j++){
            if(is_hit)  break;
            for(int l=0;l<k;l++){
                if(cache[j][l].first == data[i]){
                    hit++;
                    is_hit = true;
                    cache[j][l].second = -1;
                    break;
                }
            }
        }
        if(is_hit){
            for(int j=0;j<n/k;j++){
                for(int l=0;l<k;l++){
                    if(cache[j][l].first != -1){
                        cache[j][l].second++;
                    }
                }
            }
            continue;
        }

        // not in cache
        miss++;
        int idx = i%(n/k);
        // cache[idx] still not full
        bool fill_in = false;
        for(int j=0;j<k;j++){
            if(cache[idx][j].first == -1){
                cache[idx][j] = make_pair(data[i], -1);
                fill_in = true;
                break;
            }
        }
        if(fill_in){
            for(int j=0;j<n/k;j++){
                for(int l=0;l<k;l++){
                    if(cache[j][l].first != -1){
                        cache[j][l].second++;
                    }
                }
            }
            continue;
        }
        // cache[idx] is full
        int min_LRU = 0;
        for(int j=0;j<k;j++){
            if(cache[idx][j].second > cache[idx][min_LRU].second){
                min_LRU = j;
            }
        }
        cache[idx][min_LRU] = make_pair(data[i], -1);
        for(int j=0;j<n/k;j++){
            for(int l=0;l<k;l++){
                if(cache[j][l].first != -1){
                    cache[j][l].second++;
                }
            }
        }
    }
    cout << "Total Cache Misses:" << miss << endl;
}