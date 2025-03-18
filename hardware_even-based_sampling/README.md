# Hardware Event-based Sampling Collection
イベントベースのサンプリングの平均オーバーヘッドは、1ミリ秒のサンプリング間隔で約2％です。

## イベントリストをいくつかのイベントグループに分割
同時に収集できるハードウェアイベント（パフォーマンスモニタリングカウンタ）の数は、CPUの能力によって制限されます。通常、4つ以上のイベントを収集することはできません。この制限を克服するために  
• アプリケーションを複数回実行し、各実行時に1つのイベントグループを収集します。  
• アプリケーションを1回だけ実行し、実行中にラウンドロビン方式でイベントグループを多重化します。  
この技術は、いくつかのOS/ハードウェアの組み合わせでは機能しない場合があります。  

## VTune Profilerは以下の分析タイプで使用できる
• 異常検出  
• ホットスポット（ハードウェアイベントベースのサンプリングモード  
• パフォーマンススナップショット  
• マイクロアーキテクチャ探索  
• メモリアクセス  
• GPUコンピュート/メディアホットスポット（プレビュー）  
• GPUオフロード（プレビュー）  
• システム概要  
• スレッド処理  
• HPCパフォーマンス特性評価    

## 前提条件
サンプリングドライバーをインストール(vtune101 README.mdを参照)  
システム全体およびアンコアイベントの収集を有効にするには、rootまたはsudoを使用して/proc/sys/kernel/perf_event_paranoidを0に設定します
```
echo 0 > /proc/sys/kernel/perf_event_paranoid
```
マイクロアーキテクチャ探索分析タイプでの収集を有効にするには、開かれたファイルディスクリプタのデフォルト制限を増やします。rootまたはsudoを使用して、/etc/security/limits.confのデフォルト値を100*<論理CPUコア数>に増やします。
```
<user> hard nofile <100 * 論理CPUコア数>
<user> soft nofile <100 * 論理CPUコア数>
```

## プロファイリング例
サンプルアプリケーション [https://github.com/kazukiminemura/sycl_101](https://github.com/kazukiminemura/sycl_101/tree/main/sycl_matrix_multiplication_gpu)

ハードウェアイベントサンプリングはデフォルトでは有効ではないので、オプションをいくつか加える必要がある。
```
# ONEAPI_DEVICE_SELECTOR=opencl:cpu vtune -collect-with runsa -knob enable-pebs-collection=true -knob enable-system-cswitch=true -knob event-config=CPU_CLK_UNHALTED.THREAD:sa=3000000,INST_RETIRED.ANY:sample:sa=3000000 -- ./a.out
```
プロファイル結果
```
Collection and Platform Info
----------------------------
Parameter                 r003runsa                                                                                                             
------------------------  -------------------------------------------------------------------------------------------------------------------------
Application Command Line  ./a.out                                                                                                               
User Name                 root                                                                                                                  
Operating System          6.11.0-19-generic DISTRIB_ID=Ubuntu
DISTRIB_RELEASE=24.04
DISTRIB_CODENAME=noble
DISTRIB_DESCRIPTION="Ubuntu 24.04.2 LTS"
Computer Name             intel-Alienware-m16-R2                                                                                                
Result Size               15641413                                                                                                              
Collection start time     03:00:50 18/03/2025 UTC                                                                                               
Collection stop time      03:00:52 18/03/2025 UTC                                                                                               
Collector Type            Event-based sampling driver                                                                                           

CPU
---
Parameter          r003runsa
-----------------  --------------------------------------------------
Name               Intel(R) microarchitecture code named Meteorlake-P
Frequency          2995199976
Logical CPU Count  22

Summary
-------
Elapsed Time:             2.371
Paused Time:              0.0
CPU Time:                 2.276
Average CPU Utilization:  0.0
CPI Rate:                 2.033

Event summary
-------------
Hardware Event Type                      Core Type  Hardware Event Count:Self  Hardware Event Sample Count:Self  Events Per Sample  Precise:Self
---------------------------------------  ---------  -------------------------  --------------------------------  -----------------  ------------
INST_RETIRED.ANY                         P-Core                    5208000000                              1736  3000000                    True
CPU_CLK_UNHALTED.THREAD                  P-Core                   10650000000                              3550  3000000                    True
CPU_CLK_UNHALTED.REF_TSC                 P-Core                    6744010116                              3372  2000003                    True
INST_RETIRED.ANY                         E-Core                      27000000                                 9  3000000                    True
CPU_CLK_UNHALTED.THREAD                  E-Core                      60000000                                20  3000000                    True
CPU_CLK_UNHALTED.REF_TSC                 E-Core                             0                                 0  2000003                    True
INST_RETIRED.ANY                         LPE-Core                    57000000                                19  3000000                    True
CPU_CLK_UNHALTED.THREAD                  LPE-Core                    51000000                                17  3000000                    True
CPU_CLK_UNHALTED.REF_TSC                 LPE-Core                    72000108                                36  2000003                    True
Retire_Latency_CPU_CLK_UNHALTED.THREAD   E-Core                    8877000008                                20  3000000                    True
Retire_Latency_INST_RETIRED.ANY          E-Core                             9                                 9  3000000                    True
Retire_Latency_CPU_CLK_UNHALTED.REF_TSC  P-Core               423931125895743                              3372  2000003                    True
Retire_Latency_CPU_CLK_UNHALTED.THREAD   P-Core               670576683000004                              3550  3000000                    True
Retire_Latency_INST_RETIRED.ANY          P-Core               320184219000004                              1736  3000000                    True
Retire_Latency_CPU_CLK_UNHALTED.REF_TSC  LPE-Core                 34664052006                                36  2000003                    True
Retire_Latency_INST_RETIRED.ANY          LPE-Core                     3000018                                19  3000000                    True
Retire_Latency_CPU_CLK_UNHALTED.THREAD   LPE-Core                  8676000005                                17  3000000                    True
vtune: Executing actions 100 % done
```
## VTune GUI 画面例
<img width="1916" alt="image" src="https://github.com/user-attachments/assets/df14a4bf-cddc-44e5-ac69-98b1c8f66774" />

