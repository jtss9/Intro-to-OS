#include<iostream>
#include<deque>
using namespace std;
struct process{
    int arrive;
    int bust;
    int wait;
    int turnaround;
    int remain;
    int cnt;
    int level;
    int id;
};
int queue_num, process_num;
deque<process> p_list;
deque<process> q[10];
deque<process> qfinal;
int mode[10], time_quantum[10];
process last_p;

int f=0, rr_f=0;
void fix(process p){
    int sz = q[p.level].size();
    int ind = p.id;
    int l = p.level;
    for(int i=0;i<sz;i++){
        process tmp = q[l].front();
        q[l].pop_front();
        if(tmp.id == ind){
            tmp.level++;
            q[l+1].push_back(tmp);
        }else{
            q[l].push_back(tmp);
        }
    }
}

void solve(int i, int md){
    if(md==0){      // fcfs
        process tmp = q[i].front();
        if(tmp.id != last_p.id && last_p.remain!=0){
            if(last_p.level!=queue_num-1){
                if(rr_f){   // last process is from rr and it has been fixed
                    rr_f=0;
                }else{    // last process is from fcfs and srtf
                    fix(last_p);
                }
            }else{       // last process is in the last queue
                if(rr_f){
                    rr_f=0;
                }else if(f==2){       // rr process has been fixed
                    f=0;
                }else{          // fcfs and srtf
                    q[queue_num-1].pop_front();
                    q[queue_num-1].push_back(last_p);
                }
                
            }
        }
        q[i].pop_front();
        tmp.remain--;
        tmp.turnaround++;
        int sz = q[i].size();
        for(int ii=0;ii<sz;ii++){
            process p = q[i].front();
            q[i].pop_front();
            p.wait++;
            p.turnaround++;
            q[i].push_back(p);
        }
        if(tmp.remain){
            q[i].push_front(tmp);
        }else{
            qfinal[tmp.id] = tmp;
        }
        last_p = tmp;
    }else if(md==1){        // srtf
        int min=0;
        int ind=0;
        int remain=999;
        for(auto p : q[i]){
            if(p.remain <= remain){
                if(p.remain == remain && p.id > q[i][min].id){
                    continue;
                }
                min = ind;
                remain = p.remain;
            }
            ind++;
        }
        int size = q[i].size();
        process tmp;
        for(int ii=0;ii<size;ii++){
            if(ii==min){
                tmp = q[i].front();
                q[i].pop_front();
                continue;
            }
            process p = q[i].front();
            q[i].pop_front();
            q[i].push_back(p);
        }
        if(tmp.id != last_p.id && last_p.remain!=0){
            if(last_p.level!=queue_num-1){
                if(rr_f){       // last process is from rr and it has been fixed
                    rr_f=0;
                }else{          // last process is from fcfs and srtf
                    fix(last_p);
                }
            }else{
                if(rr_f){
                    rr_f=0;
                }else if(f==2){
                    f=0;
                }else{        // fcfs and srtf
                    q[queue_num-1].pop_front();
                    q[queue_num-1].push_back(last_p);
                }
            }     
        }
        tmp.remain--;
        tmp.turnaround++;
        int sz = q[i].size(); 
        for(int ii=0;ii<sz;ii++){
            process p = q[i].front();
            q[i].pop_front();
            p.wait++;
            p.turnaround++;
            q[i].push_back(p);
        }
        if(tmp.remain){
            q[i].push_front(tmp);
        }else{
            qfinal[tmp.id] = tmp;
        }
        last_p = tmp;
    }else if(md==2){        // rr
        int quantum = time_quantum[i];
        process tmp = q[i].front();
        if(tmp.id != last_p.id && last_p.remain!=0){
            if(last_p.level!=queue_num-1){
                if(rr_f){
                    rr_f=0;
                }else{
                    fix(last_p);
                }
            }else{
                if(rr_f){
                    rr_f=0;
                }else if(f==2){
                    f=0;
                }else{
                    q[queue_num-1].pop_front();
                    q[queue_num-1].push_back(last_p);
                }
            }
        }
        q[i].pop_front();
        tmp.remain--;
        tmp.turnaround++;
        tmp.cnt++;
        int sz = q[i].size();
        for(int ii=0;ii<sz;ii++){
            process p = q[i].front();
            q[i].pop_front();
            p.wait++;
            p.turnaround++;
            q[i].push_back(p);
        }
        if(tmp.remain){
            if(tmp.cnt == quantum){
                tmp.cnt = 0;
                if (i!=queue_num-1){        // if i is not the last queue, rr_f=1
                    tmp.level++;
                    tmp.wait--;
                    tmp.turnaround--;
                    q[i+1].push_back(tmp);
                    rr_f=1;
                }else{                      // i is the last queue, f=1
                    f=1;
                }
            }else{
                q[i].push_front(tmp);
            }
        }else{
            qfinal[tmp.id] = tmp;
        }
        last_p = tmp;
    }


    for(int k=i+1; k<queue_num; k++){
        int sz2 = q[k].size();
        for(int j=0;j<sz2;j++){
            process p = q[k].front();
            q[k].pop_front();
            p.wait++;
            p.turnaround++;
            q[k].push_back(p);
        }
    }
}

int main(){
    int cur_time=0, min_time=0;
    int total_wait=0, total_turnaround=0;
    cin >> queue_num >> process_num;
    // deque<process> q;
    // deque<process> q2;
    
    p_list.resize(process_num);
    for(int i=0;i<=queue_num;i++){
        q[i].resize(0);
    }
    qfinal.resize(process_num);
    for (int i=0;i<process_num;i++){
        qfinal[i].id = -1;
    }
    // 0 -1: FCFS
    // 1 -1: SRTF
    // 2  t: RR
    for(int i=0;i<queue_num;i++){
        cin >> mode[i] >> time_quantum[i];
    }
    for(int i=0;i<process_num;i++){
        int arr, bust;
        cin >> arr >> bust;
        min_time += bust;
        p_list[i].arrive = arr;
        p_list[i].bust = bust;
        p_list[i].remain = bust;
        p_list[i].wait = 0;
        p_list[i].turnaround = 0;
        p_list[i].cnt = 0;
        p_list[i].level = 0;
        p_list[i].id = i;
    }
    last_p.id = -1;
    while(1){
        for(auto p : p_list){
            if(p.arrive == cur_time){
                q[0].push_back(p);
            }
        }
        
        if(f==1){
            f=2;
            // last_p.wait++;
            // last_p.turnaround++;
            q[last_p.level].push_back(last_p);
        }
        for(int qu=0; qu<queue_num; qu++){
            if(q[qu].empty()){
                continue;
            }
            solve(qu, mode[qu]);
            break;
        }
        // printf("cur_time: %d\n", cur_time);
        // for(int qu=0; qu<queue_num; qu++){
        //     printf("queue %d: ", qu);
        //     for(auto p : q[qu]){
        //         printf("%d:%d,%d,%d; ", p.id, p.wait, p.turnaround, p.remain);
        //     }
        //     printf("\n");
        // }
        int flag=1;
        for(int qu=0; qu<queue_num; qu++){
            if(!q[qu].empty()){
                flag = 0;
            }
        }
        for(auto p : qfinal){
            if(p.id == -1){
                flag = 0;
            }
        }
        if(flag && cur_time >= min_time){
            break;
        }
        cur_time++;
        // printf("process: %d\n\n", last_p.id);
    }

    for(auto p : qfinal){
        cout << p.wait << " " << p.turnaround << "\n";
        total_wait += p.wait;
        total_turnaround += p.turnaround;
    }
    cout << total_wait << "\n" << total_turnaround << "\n";
}