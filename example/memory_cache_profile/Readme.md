このC++コードは、大規模な配列（ベクター）に対して並列処理を行うプログラムです

# 目的
約100億個のchar値（約1GB）を持つ配列を作成し、それを 112スレッドで分割して、各要素を ×2.0f する処理を並列で実行します。

# 使い方
```
icpx bandwith_cache_test.cpp
vtune --collect memory-access -- ./a.out
```

# プロファイル結果 1GB with 112 threads (9.3MB per threads)
25.6％のCPUがL3にデータがないためストール

```
Elapsed Time:	0.989s
    CPU Time:	5.885s
    Memory Bound:	78.3%
    L1 Bound:	3.1%
    L2 Bound:	1.5%
    L3 Bound:	25.6%
    DRAM Bound:	30.7%
    Store Bound:	14.1%
    NUMA: % of Remote Accesses:	0.0%
    UPI Utilization Bound:	0.0%
    Loads:	14,850,422
    Stores:	196,880,599
    LLC Miss Count:	0
    Total Thread Count:	114
    Paused Time:	0s
```


# プロファイル結果 100MB with 112 threads (0.9MB per threads)
ストールなし
```
Elapsed Time:	0.108s
    CPU Time:	0.130s
    Memory Bound:	43.9%
    L1 Bound:	0.0%
    L2 Bound:	0.0%
    L3 Bound:	0.0%
    DRAM Bound:	0.0%
    Store Bound:	67.6%
    NUMA: % of Remote Accesses:	0.0%
    UPI Utilization Bound:	0.0%
    Loads:	7,000,210
    Stores:	29,500,850
    LLC Miss Count:	0
    Total Thread Count:	113
    Paused Time:	0s
```

# 1GB配列操作時の考察
1. L3キャッシュミスとDRAMアクセス
L3 Bound 25.6%：L3キャッシュにデータがないため、CPUが待たされている。
DRAM Bound 30.7%：メインメモリ（DRAM）からの読み出しがボトルネック。    
→ キャッシュミスが多発しており、CPUがデータを待つ時間が長くなっています。
2. スレッド数の過剰
112スレッドは、物理コア数やL3キャッシュの共有状況を超えており、キャッシュの競合やスレッドスケジューリングのオーバーヘッドが発生。
各スレッドが小さなチャンク（9.3MB）を扱っているが、L3キャッシュの容量（通常数十MB）を超えているため、キャッシュスラッシングが起きている可能性があります。



