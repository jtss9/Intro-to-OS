#include<iostream>
#include<vector>
using namespace std;

int main(){
    // k-way set associative cache w/ LFU
    int n, k; cin >> n >> k;    // cache lines / blocks, k-way
    int data_sz; cin >> data_sz;    // data size
    vector<int> data(data_sz);
    for(int i = 0; i < data_sz; i++)    cin >> data[i];
    vector<vector<pair<int, int>>> cache(n/k, vector<pair<int, int>>(k, make_pair(-1, 0)));

    // cache[i][j] = (data, LFU)
    // LFU : used times
    // LFU = 0 : empty
    int hit = 0, miss = 0;
    for(int i=0; i<data_sz; i++){
        bool is_hit = false;
        // check if data[i] is in cache
        for(int j=0;j<n/k;j++){
            if(is_hit)  break;
            for(int l=0;l<k;l++){
                if(cache[j][l].first == data[i]){
                    hit++;
                    is_hit = true;
                    cache[j][l].second++;
                    break;
                }
            }
        }
        if(is_hit)  continue;
        // not in cache
        miss++;
        int idx = i%(n/k);
        // cache[idx] still not full
        bool fill_in = false;
        for(int j=0;j<k;j++){
            if(cache[idx][j].first == -1){
                cache[idx][j] = make_pair(data[i], 1);
                fill_in = true;
                break;
            }
        }
        if(fill_in) continue;
        // cache[idx] is full
        int min_LFU = 0;
        for(int j=0;j<k;j++){
            if(cache[idx][j].second < cache[idx][min_LFU].second){
                min_LFU = j;
            }
        }
        cache[idx][min_LFU] = make_pair(data[i], 1);
    }
    cout << "Total Cache Misses:" << miss << endl;
}