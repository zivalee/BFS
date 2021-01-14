#include <iostream>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;
// 1. BFS 長樹
// 2. SRPT 估 LB

// create min heap according to x(objective value)
typedef pair<int, vector<int>> permutation;
priority_queue<permutation, vector<permutation>, greater<permutation>> heap;


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
int estimateLB(vector<pair<int, int>> proc, int n, int nowTime) {
    int diff = 0;
    int afterCountT = 0;
    vector<int> wt;
    vector<int> *heap = new vector<int>();
    // put Process Length of first element into heap
    heap->push_back(proc[0].first);
    if(proc[0].second < nowTime){
        proc[0].second = nowTime;
    }
    // make min heap
    make_heap(heap->begin(), heap->end(), greater<>{});

    // adding process length of elements into heap
    // examine every element's arrival time and adjust the heap before putting the process length into heap
    for (int i = 1; i < n; i++) {
        if(proc[i].second < nowTime){
            proc[i].second = nowTime;
        }
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

        // save last job arrive time for later counting
        if (i == n - 1) {
            afterCountT = proc[i].second;
        }

    }

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


int main() {
    // x: burst time, y: arrival time
    vector<pair<int, int>> jobs;
    // read csv
    ifstream myFile("/home/ziva/CLionProjects/C++/BFS/test_instance.csv");

    // Make sure the file is open
    if (!myFile.is_open()) throw runtime_error("Could not open file");

    // helper vars
    string line, colname;
    int tempCol = 0;
    // read first line (the column names)
    if (myFile.good()) {
        getline(myFile, line);

        // create a stringstream from line
        stringstream ss(line);

        // extract each column name
        while (getline(ss, colname, ',')) {
            // initialize and add <colname, int> pairs to data
            stringstream colnameS(colname);
            colnameS >> tempCol;
            jobs.push_back({tempCol, 0});
        }
    }

    while (getline(myFile, line)) {
        stringstream ss(line);
        string val;

        int colIdx = 0;
        int temp = 0;

        while (getline(ss, val, ',')) {
            // convert string to int
            stringstream valS(val);
            valS >> temp;

            jobs[colIdx].second = temp;
            colIdx++;
        }
    }
    myFile.close();

    // delete first column r_j, p_j
    jobs.erase(jobs.begin());

    // edit job number
    int n = 20;
    int count = n;

    // {pair{process time, permutation{}}}
    // initial jobs
    for (int i = 0; i < n; i++) {
        // i = scheduled job
        int sumC = jobs[i].first + jobs[i].second;
        // estimate not scheduled jobs (use SRPT)
        vector<pair<int, int>> originJobs = jobs;
        originJobs.erase(originJobs.begin() + i);
        int estimate = estimateLB(originJobs, n - 1, 0);

        heap.push(make_pair(sumC + estimate, vector<int>{i + 1}));
    }

    clock_t start = clock();

    while (heap.top().second.size() != n) {
        vector<int> origin = heap.top().second;
        heap.pop();

        for (int i = 1; i <= n; i++) {
            vector<int> vec = origin;
            // updated scheduled jobs;
            vector<int> scheduled;  // job index
            vector<pair<int, int>> toCount;
            // updated unscheduled jobs;
            vector<pair<int, int>> unscheduledToCount;
            // if job is not scheduled yet
            if (find(vec.begin(), vec.end(), i) == vec.end()) {
                vec.push_back(i);

                for (int j = 0; j < vec.size(); j++) {
                    toCount.push_back(jobs.at(vec.at(j) - 1));
                    scheduled.push_back(vec.at(j));
                }
                for (int a = 1; a <= n; a++) {
                    if (find(scheduled.begin(), scheduled.end(), a) == scheduled.end()) {
                        unscheduledToCount.push_back(jobs.at(a - 1));
                    }
                }

                int scheduledSumC = calScheduledSumC(toCount);
                int unscheduledSumC = 0;
                if (unscheduledToCount.size() != 0) {
                    int nowTime = 0;
                    for (int c = 0; c < toCount.size(); c++) {
                        nowTime += toCount[c].first;
                    }
                    unscheduledSumC = estimateLB(unscheduledToCount, unscheduledToCount.size(), nowTime);
                }

                heap.push(make_pair(scheduledSumC + unscheduledSumC, vec));
                count += 1;
            }
        }
    }

    clock_t end = clock();

    cout << "n: " << n << endl;
    cout << "Best: " << heap.top().first << endl;
    cout << "Best Permutation: ";
    for (int x = 0; x < heap.top().second.size(); x++) {
        cout << heap.top().second[x] << ", ";
    }
    double elapsed_secs = double(end - start) / CLOCKS_PER_SEC;
    cout << endl << "elapsed run time: " << elapsed_secs << " s, " << endl;
    cout << "number of nodes visited: " << count;
    return 0;
}
