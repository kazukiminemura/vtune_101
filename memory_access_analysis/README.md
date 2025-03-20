# Memory Access Analysis for Cache Misses and High Bandwidth Issues
メモリアクセス分析タイプは、ハードウェアイベントベースのサンプリングを使用して、以下の指標のデータを収集します：
- Loads and Stores 指標: ロードとストアの総数を表示
- LLC Miss Count 指標: 最終レベルキャッシュ (LLC) ミスの総数を表示
  - Local DRAM Access Count 指標: ローカルメモリで処理された LLC ミスの総数を表示
  - Remote DRAM Access Count 指標: リモートソケットメモリへのアクセス回数を表示
  - Remote Cache Access Count 指標: リモートソケットキャッシュへのアクセス回数を表示
- Memory Bound 指標: デマンドロードやストア命令による待機に費やしたサイクルの割合を表示
  - L1 Bound 指標: L1 データキャッシュにミスせずにマシンが停止していた頻度を表示
  - L2 Bound 指標: L2 キャッシュでマシンが停止していた頻度を表示
  - L3 Bound 指標: L3 キャッシュで CPU が停止した頻度、または兄弟コアとの競合頻度を表示
  - L3 Latency 指標: 非負荷状態で L3 キャッシュヒットによるデマンドロードアクセスのサイクル割合を表示（L3 レイテンシ制限の可能性あり）
  - NUMA: % of Remote Accesses 指標: リモート DRAM へのメモリ要求の割合を表示。この値が低いほど良いとされます。
  - DRAM Bound 指標: 主記憶（DRAM）で CPU が停止していた頻度を表示。この指標は、以下のような問題を特定するのに役立ちます：DRAM 帯域幅制約, UPI 利用制約, メモリレイテンシ問題
    - Remote / Local DRAM Ratio 指標: リモート DRAM ロードとローカル DRAM ロードの比率を定義
    - Local DRAM 指標: ローカルメモリからのロードで CPU が停止していた頻度を表示
    - Remote DRAM 指標: リモートメモリからのロードで CPU が停止していた頻度を表 
    - Remote Cache 指標: 他のソケットのリモートキャッシュからのロードで CPU が停止していた頻度を表示
- Average Latency 指標
