#include <iostream>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;
// 1. BFS 長樹
// 2. SRPT 估 LB

int estimateLB(vector<pair<int, int>> proc, int n);


int main() {
    // x: burst time, y: arrival time
    vector<pair<int, int>> jobs = {{6, 0},
                                {2, 2},
                                {3, 2},
                                {2, 6},
                                {5, 7},
                                {2,9}};
    int n = 6;
    // create min heap according to x(objective value)
    typedef pair<int, vector<int>> permutation;
    priority_queue<permutation, vector<permutation>, greater<permutation>> heap;

    // {pair{process time, permutation{}}}
    heap.push(make_pair(6, vector<int> {1}));
    heap.push(make_pair(4, vector<int> {2}));
    heap.push(make_pair(5, vector<int> {3}));
    heap.push(make_pair(8, vector<int> {4}));
    heap.push(make_pair(12, vector<int> {5}));
    heap.push(make_pair(11, vector<int> {6}));

//    print best permutation
//    vector<int> vec = heap.top().second;
//    for(int j =0; j<vec.size(); j++){
//        cout << "init" << vec.at(j) << endl;
//    }
//    for(int i = 1; i <= n; i++){
//        vector<int> vec = heap.top().second;
//        // if job is not scheduled yet
//        if(find(vec.begin(), vec.end(), i) == vec.end()){
//            vec.push_back(i); // vec: new permutation TODO SRPT find object value
//            vector<pair<int, int>> toCount;
//            for(int j =0; j<vec.size(); j++){
//                toCount.push_back(jobs.at(vec.at(j)-1));
//            }
//            // print possible permutation
//            for(int x =0; x<toCount.size(); x++){
//                cout << toCount.at(x).first;
//                cout << toCount.at(x).second << endl;
//            }
//            cout << "------" << endl;
//
////            int res = estimateLB(toCount, toCount.size());
////            cout << res << endl;
////            heap.push(make_pair(2, vec));
//
//        }
//    }
//    print heap
//    while (! heap.empty() ) {
//        cout << heap.top().first << "\n";
//        heap.pop();
//    }


    int LB = estimateLB({{2,2}, {2,6}}, 2);
//    int LB = estimateLB(jobs, jobs.size());
    cout << LB;

    return 0;
}

// first: Process Length (pj)
// second: Arrival Time (rj)
int estimateLB(vector<pair<int, int>> proc, int n) {
    int diff = 0;
    int afterCountT = 0;
    vector<int> wt;
    vector<int> heap;
    // put Process Length of first element into heap
    heap.push_back(proc[0].first);
    // make min heap
    make_heap(heap.begin(), heap.end(), greater<>{});

    // adding process length of elements into heap
    // examine every element's arrival time and adjust the heap before putting the process length into heap
    for (int i = 1; i < n; i++) {
        // arrival time is in increasing order
        // if arrival time of job i is equal to i-1, put the process length of it into heap directly
        // if not, adjust heap then put the length into heap
        if (proc[i].second > proc[i - 1].second) {
            // time passed between two job
            diff = proc[i].second - proc[i - 1].second;

            // if the time passed can't even finished one job
            // root of heap (min job length) > time passed
            if (heap.front() > diff) {
                heap.front() -= diff;
            } else {
                // if the time passed can finish more than one job
                // arrival time of the previous job
                int previousT = proc[i - 1].second;

                while (diff > 0) {
                    int temp = heap.front();

                    if (diff >= temp) {
                        // if can finish more jobs
                        // delete root
                        pop_heap(heap.begin(), heap.end(), greater<>{});
                        heap.pop_back();
                        // update time passed (diff)
                        diff -= temp;
                        // count job finished time
                        wt.push_back(temp + previousT);
                        // update previous job finished time
                        previousT = temp + previousT;
                    } else {
                        // if no time left
                        heap.front() -= diff;
                        diff = 0;
                    }
                }
            }
        }

        // insert job length to heap
        heap.push_back(proc[i].first);
        push_heap(heap.begin(), heap.end(), greater<>{});

        // print heap
//        for (int i = 0; i < heap.size(); i++) {
//            cout << heap[i] << ", ";
//        }

        // save last job arrive time for later counting
        if (i == n - 1) {
            afterCountT = proc[i].second;
        }

    }

//    cout << "afterCountT "<< afterCountT << endl;

    // after all process length are put into heap
    // count the finished time of the remaining jobs in heap
    int num =heap.size();
    for (int j = 0; j < num; j++) {
        // count time
        wt.push_back(afterCountT + heap.front());
        afterCountT += heap.front();
        // delete the counted root min
        pop_heap(heap.begin(), heap.end(), greater<>{});
        heap.pop_back();
    }
    int total = 0;
    for (int b = 0; b < wt.size(); b++) {
        total += wt[b];
    }
    return total;
}
