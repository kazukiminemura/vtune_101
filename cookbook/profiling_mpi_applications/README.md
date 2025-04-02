# MPIアプリケーションのプロファイリング
サンプルアプリケーション: [heart_demo](https://github.com/AlexeyMalkhanov/Cardiac_demo)    
ツール:
- Intel® C++ コンパイラ
- Intel® MPI ライブラリ 2021.11
- Intel VTune Profiler 2024.0 以降
- Intel VTune Profiler - Application Performance Snapshot
最新バージョンの VTune Profiler は、製品ダウンロードページからダウンロードしてください。 オペレーティングシステム: Linux*

CPU: Intel(R) Core(TM) Ultra 7 155H

# 使い方
**ビルド**
```
source /opt/intel/oneapi/setvars.sh
git clone https://github.com/CardiacDemo/Cardiac_demo.git
cd Cardiac_demo
mkdir build
cd build
mpiicpx ../heart_demo.cpp ../luo_rudy_1991.cpp ../rcm.cpp ../mesh.cpp -g -o heart_demo -O3 -std=c++17 -qopenmp -parallel-source-info=2
```

**注意**
サンプルコードが古いので、以下のエラーが出ます。"class vector"を"class std::vector"に書き換えてください。
```
../mesh.cpp:9:16: error: explicit instantiation of 'std::vector' must occur in namespace 'std'
    9 | template class vector<Facet>;
```

**実行**
```
export OMP_NUM_THREADS=4
mpirun -np 16 -ppn 4 aps ./heart_demo -m ../mesh_mid -s ../setup_mid.txt -t 100
aps --report <aps_result>
```

**結果**
```
| Summary information
|--------------------------------------------------------------------
  Application                   : heart_demo
  Report creation date          : 2025-04-02 17:06:52
  Number of ranks               : 16
  Ranks per node                : 16
  OpenMP threads number per Rank: 4
  HW Platform                   : Intel(R) microarchitecture code named Meteorlake-P
  Frequency                     : 3.00 GHz
  Logical core count per node   : 22
  Collector type                : Event-based sampling driver,Driverless Perf system-wide counting
  Used statistics               : ./aps_result_20250402_96120000-d1ee-a222-c731-0600c392f516/
|
| Your application might underutilize the available logical CPU cores
| because of insufficient parallel work, blocking on synchronization, or too much I/O. Perform function or source line-level profiling with tools like Intel(R) VTune(TM) Profiler to discover why the CPU is underutilized.
|
  Elapsed Time:                             171.25 s
  SP GFLOPS:                                  0.01
  DP GFLOPS:                                  6.63
  Average CPU Frequency:                      3.12 GHz
  IPC Rate:                                   1.39
  GPU Accumulated Time:                       0.03 s
  GPU Average XVE IPC Rate:                   1.00 out of 2
  MPI Time:                                  65.98 s            38.53% of Elapsed Time
| Your application is MPI bound. This may be caused by high busy wait time
| inside the library (imbalance), non-optimal communication schema or MPI
| library settings. Explore the MPI Imbalance metric if it is available or use
| MPI profiling tools like Intel(R) Trace Analyzer and Collector to explore
| possible performance bottlenecks.
    MPI Imbalance:                           32.38 s            18.91% of Elapsed Time
| The application workload is not well balanced between MPI ranks.For more
| details about the MPI communication scheme use Intel(R) Trace Analyzer and
| Collector.
    Top 5 MPI functions (avg time):
        MPI_Waitall:                         58.48 s            34.15% of Elapsed Time
        MPI_Isend:                            4.77 s             2.78% of Elapsed Time
        MPI_Barrier:                          1.26 s             0.73% of Elapsed Time
| Some of the individual values contributing to this average metric are
| statistical outliers that can significantly distort the average metric value.
| They can also be a cause of performance degradation.
| Please use --counters or --metrics="MPI Hotspot 3 - MPI_Barrier" reports for
| details.
        MPI_Irecv:                            0.76 s             0.44% of Elapsed Time
        MPI_Init:                             0.69 s             0.40% of Elapsed Time
  OpenMP Imbalance:                          48.08 s            28.08% of Elapsed Time
| The application workload is not well balanced between MPI ranks.For more
| details about the MPI communication scheme use Intel(R) Trace Analyzer and
| Collector.
  Physical Core Utilization:                 63.90%
| The metric is below 80% threshold, which may signal a poor physical CPU cores
| utilization caused by: load imbalance, threading runtime overhead, contended
| synchronization, insufficient parallelism, incorrect affinity that utilizes
| logical cores instead of physical cores. Perform threading analysis with tools
| like Intel(R) VTune(TM) Profiler to discover why physical cores are
| underutilized.
  Average Physical Core Utilization:         10.22 out of 16 Physical Cores
  GPU Stack Utilization:                      0.00%
| The percentage of time when the XVEs were stalled or idle is high, which has a
| negative impact on compute-bound applications.
    XVE State:
       Active:                                2.00%
       Stalled:                               6.70%
       Idle:                                 91.20%
| A significant portion of GPU time is spent idle. This is usually caused by
| imbalance or thread scheduling problems.
  GPU Occupancy:                              5.00% of Peak Value
| Low value of the occupancy metric may be caused by inefficient work
| scheduling. Make sure work items are neither too small nor too large.
  Memory Stalls:                             37.00% of Pipeline Slots
| The metric value can indicate that a significant fraction of execution
| pipeline slots could be stalled due to demand memory load and stores. See the
| second level metrics to define if the application is cache- or DRAM-bound and
| the NUMA efficiency. Use Intel(R) VTune(TM) Profiler Memory Access analysis to
| review a detailed metric breakdown by memory hierarchy, memory bandwidth
| information, and correlation by memory objects.
    Cache Stalls:                            16.00% of Cycles
    DRAM Stalls:                             10.10% of Cycles
    DRAM Bandwidth
       Peak:                                 33.00 GB/s
       Average:                              33.00 GB/s
       Bound:                               100.00%
| The system spent much time heavily utilizing DRAM bandwidth. Improve data
| accesses to reduce cacheline transfers from/to memory using these possible
| techniques: 1) consume all bytes of each cacheline before it is evicted (for
| example, reorder structure elements and split non-hot ones); 2) merge
| compute-limited and bandwidth-limited loops; 3) use NUMA optimizations on a
| multi-socket system. Note: software prefetches do not help a bandwidth-limited
| application. Run Intel(R) VTune(TM) Profiler Memory Access Analysis to
| identify data structures to be allocated in High Bandwidth Memory (HBM), if
| available.
  Vectorization:                             37.30%
| A significant fraction of floating point arithmetic instructions are scalar.
| Use Intel Advisor to see possible reasons why the code was not vectorized.
     Instruction Mix:
       SP FLOPs:                              0.00% of uOps
       DP FLOPs:                             14.50% of uOps
          Packed:                            37.40% from DP FP
             128-bit:                        37.40%
| A significant fraction of floating point arithmetic vector instructions
| executed with partial vector load. A possible reason is compilation with a
| legacy instruction set. Check the compiler options. Another possible reason is
| compiler code generation specifics. Use Intel(R) Advisor to learn more.
             256-bit:                         0.00%
          Scalar:                            62.60% from DP FP
| A significant fraction of floating point arithmetic instructions are scalar.
| Use Intel(R) Advisor to see possible reasons why the code was not vectorized.
       Non-FP:                               85.50% of uOps
 Disk I/O Bound:                              0.00 s
 Memory Footprint:
 Resident:
       Per node:
           Peak resident set size    :         1574.00 MB (node intel-Alienware-m16-R2)
           Average resident set size :         1574.00 MB
       Per rank:
           Peak resident set size    :          156.00 MB (rank 0)
           Average resident set size :           98.38 MB
 Virtual:
       Per node:
           Peak memory consumption    :        55277.00 MB (node intel-Alienware-m16-R2)
           Average memory consumption :        55277.00 MB
       Per rank:
           Peak memory consumption    :         3502.00 MB (rank 7)
           Average memory consumption :         3454.81 MB

Graphical representation of this data is available in the HTML report: /home/intel/projects/Cardiac_demo/build/aps_report_20250402_171940.html
```

# HTML
aps_report_YYYYMMDD_<stamp>.html ファイルは、作業ディレクトリに作成されます。    
<stamp> 番号は既存のレポートを上書きしないように使用されます。レポートには、以下の情報が含まれます：
- MPI と OpenMP* の不均衡
- メモリフットプリントと物理コアの利用率
- 浮動小数点スループット
- レポートの上部には、アプリケーションの主要な懸念事項が強調されています。

<img width="1915" alt="image" src="https://github.com/user-attachments/assets/24cfdae3-2a32-48e0-8e98-fa3d95883527" />

スナップショットは、このアプリケーションが全体的にMPI通信によって制約されていることを示しています。アプリケーションは以下の問題も抱えています：
- OpenMP* の不均衡
- 物理コアの利用率
- ベクトル化の問題
MPI時間セクションには、MPIの不均衡や使用される主要なMPI関数呼び出しなどの詳細が含まれています。このセクションから、コードが主にポイント・ツー・ポイント通信を使用しており、不均衡が中程度であることがわかります。
