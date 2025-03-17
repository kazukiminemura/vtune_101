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

##　プロファイリングサンプル  
please refer sample app [https://github.com/kazukiminemura/sycl_101](https://github.com/kazukiminemura/sycl_101/tree/main/sycl_matrix_multiplication_gpu)

```
# ONEAPI_DEVICE_SELECTOR=opencl:cpu vtune --collect hotspots -knob sampling-mode=hw -knob enable-stack-collection=true -- ./a.out
vtune: Warning: To profile kernel modules during the session, make sure they are available in the /lib/modules/kernel_version/ location.
vtune: Collection started. To stop the collection, either press CTRL-C or enter from another console window: vtune -r /home/intel/projects/sycl_101/sycl_matrix_multiplication_gpu/r000hs -command stop.
M: 1024, N: 1024, K: 1024
sequential matrix multiplication: 2.16584 seconds
Running on: Intel(R) Core(TM) Ultra 7 155H
somewhat parallel matrix multiplication: 0.367389 seconds
Even more parallel matrix multiplication: 0.0619769 seconds
single workitem per workgroup parallel matrix multiplication: 0.317544 seconds
column parallel matrix multiplication: 0.0892 seconds
Tiled matrix mulpilication: 0.0279524 seconds
vtune: Collection stopped.
```
