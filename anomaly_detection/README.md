# アノマリ検出
ループ反復のような頻繁に繰り返されるコードの間隔でのパフォーマンス異常を特定します。  
マイクロ秒およびナノ秒レベルで詳細な分析を行います。   
アプリケーションのパフォーマンスは、時折発生するパフォーマンス異常によって妨げられることがあります。  
パフォーマンス異常とは、短期間で断続的に発生し、回復不能な結果を引き起こす問題のことです。

アノマリ検出分析を使用して、他の方法では特定が難しいアプリケーションのパフォーマンス異常を特定します。この分析タイプは、Intel® Processor Trace（Intel® PT）技術を使用してトレースデータの収集と詳細な時間およびイベントの測定を行います。


# 使用法
コマンド
```
vtune --collect anomaly-detection -- ./a.out
```
結果
```
Collection and Platform Info
----------------------------
Parameter                 r010pt                                                                                                                  
------------------------  -------------------------------------------------------------------------------------------------------------------------
Application Command Line  ./a.out                                                                                                                 
Operating System          6.11.0-19-generic DISTRIB_ID=Ubuntu
DISTRIB_RELEASE=24.04
DISTRIB_CODENAME=noble
DISTRIB_DESCRIPTION="Ubuntu 24.04.2 LTS"
Computer Name             intel-Alienware-m16-R2                                                                                                  
Result Size               1286237354                                                                                                              
Collection start time     08:09:33 19/03/2025 UTC                                                                                                 
Collection stop time      08:09:37 19/03/2025 UTC                                                                                                 
Collector Type            Event-based sampling driver                                                                                             

CPU
---
Parameter          r010pt
-----------------  --------------------------------------------------
Name               Intel(R) microarchitecture code named Meteorlake-P
Frequency          2995199819
Logical CPU Count  22

Summary
-------
Elapsed Time:             3.868
Paused Time:              0.0
CPU Time:
Average CPU Utilization:  0.980

Event summary
-------------
Hardware Event Type  Core Type  Hardware Event Count:Self  Hardware Event Sample Count:Self  Events Per Sample  Precise:Self
-------------------  ---------  -------------------------  --------------------------------  -----------------  ------------
PT_CLOCKTICKS        [Unknown]                          0                                 0  1
PT_WAIT              [Unknown]                          0                                 0  1
PT_INACTIVE          [Unknown]                          0                                 0  1
PT_INSTRUCTIONS      [Unknown]                          0                                 0  1
PT_BRANCHES          [Unknown]                          0                                 0  1
ITERATION_COUNT      [Unknown]                          0                                 0  1
LOOP_ENTRY_COUNT     [Unknown]                          0                                 0  1
CALL_COUNT           [Unknown]                          0                                 0  1
vtune: Executing actions 100 % done
```
