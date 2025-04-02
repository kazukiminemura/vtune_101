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


# HPCパフォーマンス特性分析の設定と実行
ほとんどのクラスターは、ログインノードと計算ノードで構成されています。
通常、ユーザーはログインノードに接続し、スケジューラを使用してジョブを計算ノードに送信し、そこで実行します。クラスター環境では、MPIアプリケーションのプロファイリングを行うために、データ収集にはコマンドラインを使用し、ジョブが完了した後にパフォーマンス分析にはGUIを使用するのが最も実用的です。

分散環境でMPI関連のメトリクスを報告するには、以下のコマンドを入力します：
```
<mpi launcher> [options] vtune [options] -r <results dir> -- <application> [arguments]
```

VTune ProfilerのコマンドラインからHPCパフォーマンス特性分析を実行する手順は以下の通りです:    
1. **環境の準備:**    
VTune Profilerファイルをソースして環境を準備します。bashシェルを使用したデフォルトインストールの場合、以下のコマンドを使用します：
```
source /opt/intel/vtune_Profiler/vars.sh
```
2. **データの収集**    
heart_demoアプリケーションのhpc-performance分析を使用してデータを収集します。
このアプリケーションはOpenMPとMPIの両方を使用します。
アプリケーションの実行は、前述の構成を使用し、Intel® MPIライブラリを使用して4つの計算ノードにわたって16のMPIランクを使用します。
各MPIランクに4のOpenMPスレッドを使用します
```
export OMP_NUM_THREADS=4
mpirun -np 16 -ppn 4 vtune -collect hpc-performance -r vtune_mpi -- ./heart_demo -m ../mesh_mid -s ../setup_mid.txt -t 100
```
3. **分析の開始**    
分析が開始され、以下の命名規則を使用して4つの出力ディレクトリが生成されます    
    vtune_mpi.<node_name>

## Intel VTune Profiler GUI
収集された結果の1つをVTune Profilerユーザーインターフェースで開きます    
```
vtune-gui ./vtune_mpi.<node_name>
```

結果はIntel VTune Profilerで開かれ、アプリケーションパフォーマンスの概要を示す Summary ウィンドウが表示されます。    
"heart_demo"はMPI並列アプリケーションのため、Summary ウィンドウには通常のメトリクスに加えてMPIの不均衡情報や、実行のクリティカルパスにおけるMPIランクの詳細が示されます。    
- **MPI不均衡**：ノード内のすべてのランクによる平均MPIビジーウェイト時間を示します。この値は、理想的なバランスで節約可能な時間を示唆します。
- **クリティカルパスにおけるMPIランク**：ビジーウェイト時間が最小のランクです。
- **MPIビジーウェイト時間とトップシリアルホットスポット**：クリティカルパスのランクに対して表示されます。これは、スケーラビリティの深刻な欠陥を特定するのに役立ちます。通常これらは高い不均衡やビジーウェイトメトリクスと相関しています。

マルチノード実行でクリティカルパスのランクにおける著しいMPIビジーウェイト時間は、異常なランクが別のノードにある可能性を示す場合があります。    
今回の例では、一部の不均衡があり、コードのシリアル領域でかなりの時間が費やされています（図には示されていません）。    

<img width="820" alt="image" src="https://github.com/user-attachments/assets/aac5e682-d265-4b73-9fd4-0b1ed0ec2859" />

ノード間でプロファイルを収集することは可能ですが、すべてのMPIデータを表示する唯一の方法は、各ノードの結果を個別にロードすることです。    
詳細なMPIトレースには、Intel® Trace Analyzer and Collector を使用してください。Intel VTune Profiler 2024.0（およびそれ以降のバージョン）では、Summary ウィンドウに帯域幅利用率のヒストグラムが含まれています。このメトリクスは、帯域幅とパケットレートを示し、コードが高い帯域幅またはパケットレート利用率によって束縛されていた実行時間の割合を示します。ヒストグラムには最大DRAM帯域幅利用率が38GB/sであることが示されていますが、これは中程度の利用率です。これにより、改善の余地がまだあることが分かります。

<img width="817" alt="image" src="https://github.com/user-attachments/assets/6597cde9-849f-4919-b58b-da08b903b59b" />


"Bottom-up"タブに切り替えることで、さらに詳細な情報を確認できます。グルーピングを"Process"に設定してください。すると、このようなビューが表示されるはずです

<img width="1048" alt="image" src="https://github.com/user-attachments/assets/19c4b469-c7af-442e-8207-b083f90c352f" />

このコードではMPIとOpenMPの両方が使用されているため、"Bottom-up"ウィンドウにはCPUおよびメモリデータに加えて、両ランタイムに関連するメトリクスが表示されます。今回の例では、OpenMP*不均衡メトリクスが赤く強調表示されています。これは、スレッド化の改善がパフォーマンス向上に寄与する可能性を示唆しています。    

また、"Bottom-up"ウィンドウの下部にある実行タイムラインを確認し、DDRおよびMCDRAMの帯域幅、CPU時間などの複数のメトリクスをレビューしてください。このコードのDRAM帯域幅タイムラインは、中程度の帯域幅で連続的な利用率を示しています（スケールはGB/s単位）。

<img width="905" alt="image" src="https://github.com/user-attachments/assets/9eb1a67b-a4c2-4adf-9780-2ae369f52370" />

詳細なスレッドごとの実行時間と以下のメトリクスの内訳が重要です
- 有効時間 (Effective Time)
- スピンとオーバーヘッド時間 (Spin and Overhead Time)
- MPIビジーウェイト時間 (MPI Busy Wait Times)
デフォルトビューでは、Super Tiny設定が使用され、すべてのプロセスとスレッドが一緒に、パフォーマンスの視覚的なマップとして表示されます。

![image](https://github.com/user-attachments/assets/b220368f-728f-4cdd-8cd4-74fc040bf14a)






