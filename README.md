# Best First Search with Branch and Bound
## Algorithm Practice 
預估該點LB = calScheduledSumC + estimateLB

1. 將所有單一 jobs 放入 heap<br>
heap:{pair{process time, permutation{}}}<br>
e.g. job 5, process time: 10，job 5 在 heap 中存為{10, {5}}
2. 找 process time 最小的優先長樹，預估加入新的 jobs (下一階層)的 process time(LB)
3. 當 heap 中 process time 最小的 permutation 包含所有的 jobs 才停止
4. 最佳解為 heap 中 process time 最小的 permutation

