# Best First Search
## Algorithm Practice 
### Best First Search with Branch and Bound
預估該點LB = calScheduledSumC + estimateLB

1. 將所有單一 jobs 放入 heap
  heap:{pair{process time, permutation{}}}
  e.g. job 5, process time: 10，job 5 在heap中{10, {5}}
2. 找 process time 最小的優先長樹，預估加入新的 jobs (下一階層)的 process time(LB)
3. 當 heap 中 process time 最小的 permutation 包含所有的 jobs 才停止
4. 最佳解為 heap 中 process time 最小的 permutation

