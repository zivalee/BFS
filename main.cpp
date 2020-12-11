#include <iostream>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;
// 1. BFS 長樹
// 2. SRPT 估 LB

int estimateLB(vector<pair<int, int>> proc, int n);

int calScheduledSumC(vector<pair<int, int>> node);

int main() {
    // x: burst time, y: arrival time
    vector<pair<int, int>> jobs = {{6, 0},
                                   {2, 2},
                                   {3, 2},
                                   {2, 6},
                                   {5, 7},
                                   {2, 9}};
    int n = 6;
    // create min heap according to x(objective value)
    typedef pair<int, vector<int>> permutation;
    priority_queue<permutation, vector<permutation>, greater<permutation>> heap;

    // {pair{process time, permutation{}}}
    for (int i = 0; i < n; i++) {
        // i = scheduled job
        int sumC = jobs[i].first + jobs[i].second;
        // estimate not scheduled jobs (use SRPT)
        vector<pair<int, int>> originJobs = jobs;
        originJobs.erase(originJobs.begin() + i);
        int estimate = estimateLB(originJobs, n - 1);

        heap.push(make_pair(sumC + estimate, vector<int>{i + 1}));
    }


    while (heap.top().second.size() != n) {
        vector<int> origin = heap.top().second;
        heap.pop();

        for (int i = 1; i <= n; i++) {
            vector<int> vec = origin;

            // if job is not scheduled yet
            if (find(vec.begin(), vec.end(), i) == vec.end()) {
                vec.push_back(i);

                // updated scheduled jobs;
                vector<int> scheduled;  // job index
                vector<pair<int, int>> toCount;
                // updated unscheduled jobs;
                vector<pair<int, int>> unscheduledToCount;
                for (int j = 0; j < vec.size(); j++) {
                    toCount.push_back(jobs.at(vec.at(j) - 1));
                    scheduled.push_back(vec.at(j));
                }

                for (int a = 1; a <= n; a++) {
                    if (find(scheduled.begin(), scheduled.end(), a) == scheduled.end()) {
                        unscheduledToCount.push_back(jobs.at(a - 1));
                    }
                }
                // print scheduled jobs index
                for (int x = 0; x < vec.size(); x++) {
                    cout << vec[x];
                }
                cout << endl;

                // print scheduled jobs
//                for (int x = 0; x < toCount.size(); x++) {
//                    cout << toCount.at(x).first;
//                    cout << toCount.at(x).second << endl;
//                }
//                cout << endl;

                // print unscheduledToCount
//                for (int x = 0; x < unscheduledToCount.size(); x++) {
//                    cout << unscheduledToCount.at(x).first << unscheduledToCount.at(x).second << endl;
//                }
//                cout << "------" << endl;


                int scheduledSumC = calScheduledSumC(toCount);
                int unscheduledSumC = 0;
                if (unscheduledToCount.size() != 0) {
                    unscheduledSumC = estimateLB(unscheduledToCount, unscheduledToCount.size());
                }

                heap.push(make_pair(scheduledSumC + unscheduledSumC, vec));

//                cout << "scheduled:" << scheduledSumC << endl;
//                cout << "unscheduled:" << unscheduledSumC << endl;
//                cout << "-------------------------" << endl;

            }
        }
    }

    cout <<"Best: "<< heap.top().first << endl;
    cout << "Best Permutation: ";
    for (int x = 0; x < heap.top().second.size(); x++) {
        cout << heap.top().second[x];
    }

    return 0;
}


int calScheduledSumC(vector<pair<int, int>> node) {
    vector<int> sum;
    auto sumC = 0;
    for (auto i = 0; i < node.size(); i++) {
        if (i == 0 || node[i].second > sum.back())
            sum.push_back(node[i].first + node[i].second);
        else
            sum.push_back(sum.back() + node[i].first);
    }
    for (auto i = 0; i < sum.size(); i++)
        sumC += sum[i];
    return sumC;
}


// first: Process Length (pj)
// second: Arrival Time (rj)
int estimateLB(vector<pair<int, int>> proc, int n) {
    int diff = 0;
    int afterCountT = 0;
    vector<int> wt;
    vector<int> *heap = new vector<int>();
    // put Process Length of first element into heap
    heap->push_back(proc[0].first);
    // make min heap
    make_heap(heap->begin(), heap->end(), greater<>{});

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
            if (heap->front() > diff) {
                heap->front() -= diff;
            } else {
                // if the time passed can finish more than one job
                // arrival time of the previous job
                int previousT = proc[i - 1].second;

                while (diff > 0 && heap->begin() != heap->end()) {
                    int temp = heap->front();

                    if (diff >= temp) {
                        // if can finish more jobs
                        // delete root
                        pop_heap(heap->begin(), heap->end(), greater<>{});
                        heap->pop_back();
                        // update time passed (diff)
                        diff -= temp;
                        // count job finished time
                        wt.push_back(temp + previousT);
                        // update previous job finished time
                        previousT = temp + previousT;
                    } else {
                        // if no time left
                        heap->front() -= diff;
                        diff = 0;
                    }
                }
            }
        }

        // insert job length to heap
        heap->push_back(proc[i].first);
        push_heap(heap->begin(), heap->end(), greater<>{});

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
    int num = heap->size();
    for (int j = 0; j < num; j++) {
        // count time
        wt.push_back(afterCountT + heap->front());
        afterCountT += heap->front();
        // delete the counted root min
        pop_heap(heap->begin(), heap->end(), greater<>{});
        heap->pop_back();
    }
    int total = 0;
    for (int b = 0; b < wt.size(); b++) {
        total += wt[b];
    }
    delete heap;
    return total;
}
