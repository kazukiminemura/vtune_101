# Performance Snapshot
```
ONEAPI_DEVICE_SELECTOR=opencl:cpu vtune --collect performance-snapshot -- ./a.out
```

サンプル例
![image](https://github.com/user-attachments/assets/04fa23e7-ff24-4186-b146-b1f4b0230067)




## 次のお勧めステップ
```
Recommendations:
    Hotspots: Start with Hotspots analysis to understand the efficiency of your algorithm.
     | Use Hotspots analysis to identify the most time consuming functions.
     | Drill down to see the time spent on every line of code.
    Threading: There is poor utilization of logical CPU cores (3.8%) in your application.
     |  Use Threading to explore more opportunities to increase parallelism in
     | your application.
    GPU Offload: GPU Active Time (6.7%) is low. Consider offloading more work to the GPU to increase overall application performance.
     | Use GPU Offload analysis to explore code execution on various CPU and GPU
     | cores on your platform. Estimate how your code benefits from offloading
     | to the GPU. Identify whether your application is CPU or GPU bound.
    Microarchitecture Exploration: There is low microarchitecture usage (10.0%) of available hardware resources.
     | Run Microarchitecture Exploration analysis to analyze CPU
     | microarchitecture bottlenecks that can affect application performance.
    Memory Access: The Memory Bound metric is high  (40.5%). A significant fraction of execution pipeline slots could be stalled due to demand memory load and stores.
     | Use Memory Access analysis to measure metrics that can identify memory
     | access issues.
    HPC Performance Characterization: Vectorization value is 67.4%, but vector registers were not fully utilized. Using the latest vector instruction set can improve parallelism for this code. Consider either recompiling the code with the latest instruction set or using Intel Advisor to get vectorization help.
     | Use HPC Performance Characterization analysis to examine the performance
     | of compute-intensive applications. Understand CPU/GPU utilization and get
     | information about OpenMP efficiency, memory access, and vectorization.
```
