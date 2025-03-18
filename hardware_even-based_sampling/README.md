# Hardware Event-based Sampling Collection
イベントベースのサンプリングの平均オーバーヘッドは、1ミリ秒のサンプリング間隔で約2％です。

## イベントリストをいくつかのイベントグループに分割
同時に収集できるハードウェアイベント（パフォーマンスモニタリングカウンタ）の数は、CPUの能力によって制限されます。通常、4つ以上のイベントを収集することはできません。この制限を克服するために  
• アプリケーションを複数回実行し、各実行時に1つのイベントグループを収集します。  
• アプリケーションを1回だけ実行し、実行中にラウンドロビン方式でイベントグループを多重化します。  
この技術は、いくつかのOS/ハードウェアの組み合わせでは機能しない場合があります。  

## VTune Profilerは以下の分析タイプでハードウェアイベントベースのサンプリングコレクターを使用
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
# ONEAPI_DEVICE_SELECTOR=opencl:cpu vtune -collect-with runsa -knob enable-stack-collection=true -knob enable-system-cswitch=true -knob event-config=CPU_CLK_UNHALTED.THREAD:sa=3000000,INST_RETIRED.ANY:sample:sa=3000000 -- ./a.out
```
プロファイル結果
```
Collection and Platform Info
----------------------------
Parameter                 r000runsa                                                                                                             
------------------------  -------------------------------------------------------------------------------------------------------------------------
Application Command Line  ./a.out                                                                                                               
Operating System          6.11.0-19-generic DISTRIB_ID=Ubuntu
DISTRIB_RELEASE=24.04
DISTRIB_CODENAME=noble
DISTRIB_DESCRIPTION="Ubuntu 24.04.2 LTS"
Computer Name             intel-Alienware-m16-R2                                                                                                
Result Size               30662555                                                                                                              
Collection start time     02:41:02 18/03/2025 UTC                                                                                               
Collection stop time      02:41:05 18/03/2025 UTC                                                                                               
Collector Type            Driverless Perf per-process sampling                                                                                  

CPU
---
Parameter          r000runsa
-----------------  --------------------------------------------------
Name               Intel(R) microarchitecture code named Meteorlake-P
Frequency          2995199828
Logical CPU Count  22

Summary
-------
Elapsed Time:  2.948
Paused Time:   0.0

Event summary
-------------
Hardware Event Type      Core Type  Hardware Event Count:Self  Hardware Event Sample Count:Self  Events Per Sample  Precise:Self
-----------------------  ---------  -------------------------  --------------------------------  -----------------  ------------
INST_RETIRED.ANY         E-Core                    3027000000                              1009  3000000                    True
CPU_CLK_UNHALTED.THREAD  E-Core                   12435000000                              4145  3000000                    True
INST_RETIRED.ANY         P-Core                   15867000000                              5289  3000000                    True
CPU_CLK_UNHALTED.THREAD  P-Core                   32310000000                             10770  3000000                    True
vtune: Executing actions 100 % done
```
## VTune GUI 画面例
![image](https://github.com/user-attachments/assets/6a38635f-790e-45d7-9b72-eed86ea29cf0)
