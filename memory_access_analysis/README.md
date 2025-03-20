# Memory Access Analysis for Cache Misses and High Bandwidth Issues
メモリアクセス分析タイプは、ハードウェアイベントベースのサンプリングを使用して、以下の指標のデータを収集します：
- **Loads and Stores** 指標: ロードとストアの総数を表示
- **LLC Miss Coun**t 指標: 最終レベルキャッシュ (LLC) ミスの総数を表示
  - **Local DRAM Access Count** 指標: ローカルメモリで処理された LLC ミスの総数を表示
  - **Remote DRAM Access Count** 指標: リモートソケットメモリへのアクセス回数を表示
  - **Remote Cache Access Count** 指標: リモートソケットキャッシュへのアクセス回数を表示
- **Memory Bound** 指標: デマンドロードやストア命令による待機に費やしたサイクルの割合を表示
  - **L1 Bound** 指標: L1 データキャッシュにミスせずにマシンが停止していた頻度を表示
  - **L2 Bound** 指標: L2 キャッシュでマシンが停止していた頻度を表示
  - **L3 Bound** 指標: L3 キャッシュで CPU が停止した頻度、または兄弟コアとの競合頻度を表示
  - **L3 Latency** 指標: 非負荷状態で L3 キャッシュヒットによるデマンドロードアクセスのサイクル割合を表示（L3 レイテンシ制限の可能性あり）
  - **NUMA: % of Remote Accesse**s 指標: リモート DRAM へのメモリ要求の割合を表示。この値が低いほど良いとされます。
  - **DRAM Bound** 指標: 主記憶（DRAM）で CPU が停止していた頻度を表示。この指標は、以下のような問題を特定するのに役立ちます：DRAM 帯域幅制約, UPI 利用制約, メモリレイテンシ問題
    - **Remote / Local DRAM Ratio** 指標: リモート DRAM ロードとローカル DRAM ロードの比率を定義
    - **Local DRAM** 指標: ローカルメモリからのロードで CPU が停止していた頻度を表示
    - **Remote DRAM** 指標: リモートメモリからのロードで CPU が停止していた頻度を表 
    - **Remote Cache** 指標: 他のソケットのリモートキャッシュからのロードで CPU が停止していた頻度を表示
- **Average Latenc**y 指標

## 使用方法
```
ONEAPI_DEVICE_SELECTOR=opencl:cpu vtune --collect memory-access -- ./a.out
```
結果例
```
vtune: Executing actions 75 % Generating a report                              Elapsed Time: 35.106s
    CPU Time: 191.028s
    Performance-core (P-core)
        Memory Bound: 66.3% of Pipeline Slots
         | The metric value is high. This can indicate that the significant
         | fraction of execution pipeline slots could be stalled due to demand
         | memory load and stores. Use Memory Access analysis to have the metric
         | breakdown by memory hierarchy, memory bandwidth information,
         | correlation by memory objects.
         |
            L1 Bound: 1.0% of Clockticks
            L2 Bound: 8.0% of Clockticks
             | This metric shows how often machine was stalled on L2 cache.
             | Avoiding cache misses (L1 misses/L2 hits) will improve the
             | latency and increase performance.
             |
            L3 Bound: 56.0% of Clockticks
             | This metric shows how often CPU was stalled on L3 cache, or
             | contended with a sibling Core. Avoiding cache misses (L2
             | misses/L3 hits) improves the latency and increases performance.
             |
            DRAM Bound: 16.4% of Clockticks
             | This metric shows how often CPU was stalled on the main memory
             | (DRAM). Caching typically improves the latency and increases
             | performance.
             |
            Store Bound: 0.0% of Clockticks
    Efficient-core (E-core)
    Uncore
        DRAM Bandwidth Bound: 0.0% of Elapsed Time
    Loads: 126,698,244,835
    Stores: 4,859,625,572
    Average Latency (cycles): 190
    Total Thread Count: 25
    Paused Time: 0s

Bandwidth Utilization
Bandwidth Domain  Platform Maximum  Observed Maximum  Average  % of Elapsed Time with High BW Utilization(%)
----------------  ----------------  ----------------  -------  ---------------------------------------------
DRAM, GB/sec      76                          30.300   10.771                                           0.0%

Top Tasks
Task Type         Task Time  Task Count  Average Task Time
----------------  ---------  ----------  -----------------
tbb_parallel_for   686.068s      10,639             0.064s
tbb_custom          61.671s       2,560             0.024s
Collection and Platform Info
    Application Command Line: ./a.out
    Operating System: 6.11.0-19-generic DISTRIB_ID=Ubuntu DISTRIB_RELEASE=24.04 DISTRIB_CODENAME=noble DISTRIB_DESCRIPTION="Ubuntu 24.04.2 LTS"
    Computer Name: intel-Alienware-m16-R2
    Result Size: 2.3 GB
    Collection start time: 09:20:25 20/03/2025 UTC
    Collection stop time: 09:21:00 20/03/2025 UTC
    Collector Type: Driverless Perf system-wide sampling
    CPU
        Name: Intel(R) microarchitecture code named Meteorlake-P
        Frequency: 2.995 GHz
        Logical CPU Count: 22
        Max DRAM Single-Package Bandwidth: 76.000 GB/s
        Cache Allocation Technology
            Level 2 capability: not detected
            Level 3 capability: not detected

If you want to skip descriptions of detected performance issues in the report,
enter: vtune -report summary -report-knob show-issues=false -r <my_result_dir>.
Alternatively, you may view the report in the csv format: vtune -report
<report_name> -format=csv.
vtune: Executing actions 100 % done
```

## GUI
### Summary: アプリケーション全体の実行に関する統計情報を表示します
<img width="1565" alt="image" src="https://github.com/user-attachments/assets/d50abf55-a0fe-43aa-957e-97a039924f7b" />
- 分析構成で「最大DRAM帯域幅を評価する (Evaluate max DRAM bandwidth)」オプションを選択した場合、Platform Diagram は平均DRAM利用率を表示します。それ以外の場合は、平均DRAM帯域幅が表示されます。  
- 平均UPI利用率 (Average UPI Utilization) 指標は、送信レベルでのUPI利用率を示します。ソケット間のUPIリンクの数に関係なく、Platform Diagram では単一のクロスソケット接続を表示します。複数のリンクがある場合、図にはその最大値が表示されます。
- 各ソケットの上部には、平均物理コア利用率 (Average Physical Core Utilization) 指標があり、分析中のアプリケーションによる物理コアの利用状況を示します。

### Bottom-up: 各ホットスポットオブジェクトの指標ごとのパフォーマンスデータを表示します
Analyze memory objects」オプションを有効にしてデータを収集すると、Bottom-up ウィンドウにグリッドおよび Call Stack ペイン内でメモリアロケーションのコールスタックも表示されます。関数レベルの後にメモリオブジェクトグループ化レベルを使用して、アロケーションコールのソース位置としてメモリオブジェクトを表示します。
<img width="1582" alt="image" src="https://github.com/user-attachments/assets/ae4dacf9-e618-4399-9686-2b5d22228c33" />


### **Platform**: Task API、Ftrace/Systrace イベントタスク、OpenCL™ API タスクなど、コードで指定されたタスクの詳細を提供します。対応するプラットフォーム指標が収集されている場合、Platform ウィンドウは、ソフトウェアキュー上の GPU 使用率、CPU 時間使用率、OpenCL™ カーネルデータ、GPU ハードウェア指標の Overview グループ、メモリ帯域幅、および CPU 周波数ごとの GPU パフォーマンスとして、時間経過データを表示します。
![image](https://github.com/user-attachments/assets/4c58cdf7-4438-46b2-8933-1cf143dea11b)
