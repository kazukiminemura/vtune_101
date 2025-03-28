# HPCパフォーマンス特性分析
計算集約型アプリケーションがCPU、メモリ、および浮動小数点演算ハードウェアリソースをどの程度効果的に使用しているかを特定します。 

## 動作の仕組み
HPCパフォーマンス特性分析は、アプリケーションのパフォーマンス面を理解するための出発点として使用できます。
Intel OpenMP*またはIntel MPIランタイムライブラリを使用するアプリケーションには、追加のスケーラビリティメトリクスが利用可能です。

<img width="505" alt="image" src="https://github.com/user-attachments/assets/3b9d27c0-0357-4d8f-9cfc-5fc0a2b94d26" />

Intel® VTune™ Profilerデータコレクターはイベントベースのサンプリング収集を使用してアプリケーションをプロファイルします。
Intel OpenMPランタイムライブラリのOpenMP分析メトリクスは、ランタイムライブラリで有効にされたユーザーAPI計測に基づいています。 
通常、コレクターは特定のアプリケーションのデータを収集しますが、必要に応じてシステム全体のパフォーマンスデータを限定的な詳細で収集することもできます。

## 使い方
```
ONEAPI_DEVICE_SELECTOR=level_zero:gpu vtune --collect hpc-performance -- ./a.out
```

結果
```
Generating a report                              Elapsed Time: 4.427s
    CPU
        SP GFLOPS: 0.537
        DP GFLOPS: 0.000
        x87 GFLOPS: 0.000
        CPI Rate: 0.754
        Average CPU Frequency: 4.658 GHz
        Total Thread Count: 2
    GPU
        GPU Stack Utilization: 46.1% (0.461 out of 1 GPU Stacks)
         | GPU utilization is low. Consider offloading more work to the GPU to
         | increase overall application performance.
         |
        GPU Accumulated Time: 2.041s
        IPC Rate: 1.126
Effective Physical Core Utilization: 5.7% (0.916 out of 16)
 | The metric value is low, which may signal a poor physical CPU cores
 | utilization caused by:
 |     - load imbalance
 |     - threading runtime overhead
 |     - contended synchronization
 |     - thread/process underutilization
 |     - incorrect affinity that utilizes logical cores instead of physical
 |       cores
 | Explore sub-metrics to estimate the efficiency of MPI and OpenMP parallelism
 | or run the Locks and Waits analysis to identify parallel bottlenecks for
 | other parallel runtimes.
 |
    Effective Logical Core Utilization: 4.2% (0.924 out of 22)
     | The metric value is low, which may signal a poor logical CPU cores
     | utilization. Consider improving physical core utilization as the first
     | step and then look at opportunities to utilize logical cores, which in
     | some cases can improve processor throughput and overall performance of
     | multi-threaded applications.
     |
GPU Stack Utilization: 46.1%
 | GPU utilization is low. Consider offloading more work to the GPU to increase
 | overall application performance.
 |
    XVE State: 6.5%
        Active: 6.5%
        Stalled: 36.1%
         | A significant portion of GPU time is lost due to stalls. Use GPU
         | Compute/Media Hotspots (preview) analysis to analyze HW usage
         | efficiency.
         |
        Idle: 57.4%
         | A significant portion of GPU time is spent idle. This is usually
         | caused by imbalance or thread scheduling problems.
         |
    Occupancy: 36.5% of peak value
     | Several factors including shared local memory, use of memory barriers,
     | and inefficient work scheduling can cause a low value of the occupancy
     | metric.
     |
Memory Bound: 38.5% of Pipeline Slots
 | The metric value is high. This can indicate that the significant fraction of
 | execution pipeline slots could be stalled due to demand memory load and
 | stores. Use Memory Access analysis to have the metric breakdown by memory
 | hierarchy, memory bandwidth information, correlation by memory objects.
 |
    Performance-core (P-core)
        Memory Bound: 38.5% of Pipeline Slots
         | The metric value is high. This can indicate that the significant
         | fraction of execution pipeline slots could be stalled due to demand
         | memory load and stores. Use Memory Access analysis to have the metric
         | breakdown by memory hierarchy, memory bandwidth information,
         | correlation by memory objects.
         |
            Cache Bound: 40.8% of Clockticks
             | A significant proportion of cycles are being spent on data
             | fetches from caches. Check Memory Access analysis to see if
             | accesses to L2 or L3 caches are problematic and consider applying
             | the same performance tuning as you would for a cache-missing
             | workload. This may include reducing the data working set size,
             | improving data access locality, blocking or partitioning the
             | working set to fit in the lower cache levels, or exploiting
             | hardware prefetchers. Consider using software prefetchers, but
             | note that they can interfere with normal loads, increase latency,
             | and increase pressure on the memory system. This metric includes
             | coherence penalties for shared data. Check Microarchitecture
             | Exploration analysis to see if contested accesses or data sharing
             | are indicated as likely issues.
             |
            DRAM Bound: 0.4% of Clockticks
    Efficient-core (E-core)
    Low-power Efficient-core (LPE-core)

    Bandwidth Utilization
    Bandwidth Domain  Platform Maximum  Observed Maximum  Average  % of Elapsed Time with High BW Utilization(%)
    ----------------  ----------------  ----------------  -------  ---------------------------------------------
    DRAM, GB/sec      76                          10.300    1.446                                           0.0%
Vectorization: 66.7% of Packed FP Operations
    Instruction Mix
        SP FLOPs: 2.8% of uOps
            Packed: 66.7% from SP FP
                128-bit: 66.7% from SP FP
                256-bit: 0.0% from SP FP
            Scalar: 33.3% from SP FP
        DP FLOPs: 0.0% of uOps
            Packed: 0.0% from DP FP
                128-bit: 0.0% from DP FP
                256-bit: 0.0% from DP FP
            Scalar: 0.0% from DP FP
        x87 FLOPs: 0.0% of uOps
        Non-FP: 97.2% of uOps
Collection and Platform Info
    Application Command Line: ./a.out
    User Name: root
    Operating System: 6.11.0-19-generic DISTRIB_ID=Ubuntu DISTRIB_RELEASE=24.04 DISTRIB_CODENAME=noble DISTRIB_DESCRIPTION="Ubuntu 24.04.2 LTS"
    Computer Name: intel-Alienware-m16-R2
    Result Size: 117.1 MB
    Collection start time: 05:42:06 28/03/2025 UTC
    Collection stop time: 05:42:11 28/03/2025 UTC
    Collector Type: Event-based sampling driver
    CPU
        Name: Intel(R) microarchitecture code named Meteorlake-P
        Frequency: 2.995 GHz
        Logical CPU Count: 22
        Max DRAM Single-Package Bandwidth: 76.000 GB/s
    GPU
        Name: Meteor Lake-P [Intel Arc Graphics]
        Vendor: Intel Corporation
        XVE Count: 128
        Max XVE Thread Count: 8
        Max Core Frequency: 2.250 GHz

If you want to skip descriptions of detected performance issues in the report,
enter: vtune -report summary -report-knob show-issues=false -r <my_result_dir>.
Alternatively, you may view the report in the csv format: vtune -report
<report_name> -format=csv.
vtune: Executing actions 100 % done
```

## GUI
データの表示 HPCパフォーマンス特性ビューを使用して、以下を確認します：
- **物理コアの有効利用**: アプリケーションコードの実行による物理コアの利用状況を確認し、並列効率を探ります。シリアル時間と並列時間の使用に関するスケーラビリティの問題、OpenMP領域のチューニングの可能性、MPIの不均衡を探します。
- **メモリバウンド**: アプリケーションがメモリバウンドかどうかを評価します。より深い問題を理解するために、メモリアクセス分析を実行して問題を引き起こしている特定のメモリオブジェクトを特定します。
- **ベクトル化**: 浮動小数点ループが帯域幅バウンドかベクトル化されているかを判断します。帯域幅バウンドのループ/関数については、メモリアクセス分析を実行して帯域幅の消費を削減します。ベクトル化の最適化の機会については、Intel Advisorを使用してベクトル化分析を実行します。
- **Intel® Omni-Path Fabricの使用**: インターコネクトの限界に達することで引き起こされるパフォーマンスボトルネックを特定します。

## HPCパフォーマンス特性ビュー
HPCパフォーマンス特性ビューを使用して、計算集約型またはスループットアプリケーションのCPU使用率、メモリ効率、および浮動小数点利用率を推定します。計算集約型またはスループットアプリケーションは、経過時間の間にハードウェアリソースを効率的に使用する必要があります。HPCパフォーマンス特性分析を、アプリケーションのパフォーマンスと実行時間を最適化するための出発点として使用します。 

HPCパフォーマンス特性ビューで提供されるパフォーマンスデータを解釈するための手順は次のとおりです：
1. パフォーマンスベースラインを定義する
2. 最適化の機会を特定する
3. ソースを分析する
4. プロセス/スレッドのアフィニティを分析する
5. 他の分析タイプを探る

## パフォーマンスベースラインを定義
まず、アプリケーション実行に関する一般的な情報を提供する「サマリーウィンドウ」を確認しましょう。
最適化の主な対象領域には、経過時間や浮動小数点演算毎秒（単精度、倍精度、レガシーx87）のカウントが含まれます。
赤いテキストは、最適化の可能性がある領域を示しています。フラグにカーソルを合わせると、コードを改善する方法に関する詳細が表示されます。
経過時間とGFLOPS（ギガフロップス）の値を、最適化前後のバージョンの比較基準として使用してください。

<img width="910" alt="image" src="https://github.com/user-attachments/assets/69790f1e-fbf7-4248-a9f0-83e72c8bfce6" />

## 最適化の機会を特定
「サマリーウィンドウ」を確認して、アプリケーションにおける主な最適化機会を見つけましょう。改善可能なパフォーマンス指標は赤で表示されます。特定された問題には、物理コアの有効利用、メモリボトルネック、ベクトル化、またはそれらの組み合わせが含まれる場合があります。以下のセクションでは、それぞれのパフォーマンス要素に対する次のステップが提案されています：
- トポロジー、メモリ、クロスソケット帯域幅
- CPU利用率
- GPU利用率
- メモリボトルネック
- ベクトル化

**トポロジー、メモリ、およびクロスソケット帯域幅**
HPC（ハイパフォーマンスコンピューティング）性能特性の視点で、「サマリーウィンドウ」から性能分析を開始しましょう。ここでは、「プラットフォームダイアグラム」が、DRAM、Intel® Ultra Path Interconnect（Intel® UPI）リンク、物理コアのシステムトポロジーおよび使用率メトリクスを表示します。
最適ではないアプリケーショントポロジーにより、DRAMやIntel® QuickPath Interconnect（Intel® QPI）またはIntel® Ultra Path Interconnect（Intel® UPI）間のクロスソケットトラフィックが発生する可能性があります。これらの事象はパフォーマンスを制限する要因となる場合があります。
HPC（ハイパフォーマンスコンピューティング）性能特性の視点で、「サマリーウィンドウ」から性能分析を開始しましょう。ここでは、「プラットフォームダイアグラム」が、DRAM、Intel® Ultra Path Interconnect（Intel® UPI）リンク、物理コアのシステムトポロジーおよび使用率メトリクスを表示します。

最適ではないアプリケーショントポロジーにより、DRAMやIntel® QuickPath Interconnect（Intel® QPI）またはIntel® Ultra Path Interconnect（Intel® UPI）間のクロスソケットトラフィックが発生する可能性があります。これらの事象はパフォーマンスを制限する要因となる場合があります。

<img width="428" alt="image" src="https://github.com/user-attachments/assets/e71385ad-79d1-4373-87f9-c63227c48844" />

## CPU利用率
![image](https://github.com/user-attachments/assets/658f4d1f-0dc1-4c6b-bfa0-aa27e5a2c019)
- **物理コアの有効利用率**
アプリケーションの並列効率を測定する指標として、物理コアの有効利用率を探索してください。100%の値は、アプリケーションコードの実行が利用可能な物理コアすべてを使用していることを示します。値が100%未満の場合、並列処理の非効率性の原因を発見するために、第二レベルのメトリクスを確認する価値があります。
- **論理コアの利用率**
論理コアの利用が、アプリケーションの同時実行性の向上や全体的な性能向上につながる場合があります。論理コアの効果的な活用方法を検討してください

## GPU利用率
GPU利用率メトリクスが表示される条件
- アプリケーションがGPUを使用している場合
- システムがGPUデータを収集するように設定されている場合（「GPU分析のためのシステム設定」を参照してください）
**経過時間のセクション**: GPUセクションでは、アプリケーションがどのように作業をGPUにオフロードしているかの概要が表示されます
<img width="379" alt="image" src="https://github.com/user-attachments/assets/06f8ad9a-c9ae-4c12-b1f4-6f12f8ef8e0f" />



  
