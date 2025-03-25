# GPU Compute/Media Hotspots Analysis
GPUコンピュート/メディアホットスポット分析を使用して以下を行います。
- GPUの使用率が高いGPUカーネルを調査し、その利用効率を推定し、停止や低占有率の可能性のある理由や選択肢を特定します。
- 選択したGPUメトリクスに基づいて、アプリケーションの性能を時間経過で調査します。
- 最も負荷の高いSYCL*標準やOpenCL™カーネルについて、効率の悪いカーネルコードアルゴリズムや不適切なワークアイテム構成を分析します。

## 仕組み
Intel Graphics レンダーエンジンとハードウェア メトリクスGPUは、高度に並列化されたマシンであり、小型コアまたは実行ユニット（EU）の配列がグラフィックまたは計算作業を行います。各EUは複数の軽量スレッドを同時に実行します。これらのスレッドの1つが実行のために選ばれると、メモリや他のユニットからデータを待っている間に停止しているスレッドの停止を隠すことができます。

GPUの潜在能力を最大限に活用するには、できるだけ多くのスレッドをスケジュールできるようにし、アイドル状態のサイクルを最小化する必要があります。停止の最小化は、グラフィックスおよび汎用コンピューティングGPUアプリケーションにとって非常に重要です。

VTune ProfilerはIntel Graphicsのハードウェアイベントを監視し、サンプリング期間中のGPUリソース利用率についてのメトリクスを表示できます。たとえば、EUがアイドル状態、停止状態、またはアクティブ状態だったサイクルの割合や、メモリアクセスやその他の機能ユニットの統計情報を提供します。VTune ProfilerがGPUカーネルの実行をトレースする場合、各カーネルをGPUメトリクスで注釈付けします。  

## 使い方
```
vtune --collect gpu-hotspots -- ./a.out
```

結果例
```
Generating a report                              Elapsed Time: 70.963s
    GPU Time: 67.423s
XVE Array Stalled/Idle: 83.3% of Elapsed time with GPU busy
 | The percentage of time when the XVEs were stalled or idle is high, which has
 | a negative impact on compute-bound applications.
 |
    GPU L3 Bandwidth Bound: 0.8% of peak value

        Hottest GPU Computing Tasks Bound by GPU L3 Bandwidth
        Computing Task  Total Time
        --------------  ----------
    Occupancy: 99.7% of peak value

        Hottest GPU Computing Tasks with Low Occupancy
        Computing Task  Total Time  SIMD Width  Peak XVE Threads Occupancy(%)  Occupancy(%)  SIMD Utilization(%)
        --------------  ----------  ----------  -----------------------------  ------------  -------------------
Collection and Platform Info
    Application Command Line: ./a.out
    User Name: root
    Operating System: 6.11.0-19-generic DISTRIB_ID=Ubuntu DISTRIB_RELEASE=24.04 DISTRIB_CODENAME=noble DISTRIB_DESCRIPTION="Ubuntu 24.04.2 LTS"
    Computer Name: intel-Alienware-m16-R2
    Result Size: 466.0 MB
    Collection start time: 13:48:08 25/03/2025 UTC
    Collection stop time: 13:49:19 25/03/2025 UTC
    Collector Type: Event-based sampling driver,User-mode sampling and tracing
    CPU
        Name: Intel(R) microarchitecture code named Meteorlake-P
        Frequency: 2.995 GHz
        Logical CPU Count: 22
    GPU
        Name: Meteor Lake-P [Intel Arc Graphics]
        Vendor: Intel Corporation
        XVE Count: 128
        Max XVE Thread Count: 8
        Max Core Frequency: 2.250 GHz
        GPU OpenCL Info
            Version
            Max Compute Units: 128
            Max Work Group Size: 1,024
            Local Memory: 65.5 KB
            SVM Capabilities

If you want to skip descriptions of detected performance issues in the report,
enter: vtune -report summary -report-knob show-issues=false -r <my_result_dir>.
Alternatively, you may view the report in the csv format: vtune -report
<report_name> -format=csv.
vtune: Executing actions 100 % done
```

## GUI
GPUコンピュート/メディアホットスポット分析がデータ収集を完了すると、summuryウィンドウに以下のメトリクスが表示されます：
- GPU時間
- 占有率
- 現在の計算タスク構成で予想される最大占有率
- GPU上で実行された最もアクティブな計算タスク
- 最もアクティブなIntel® oneAPI Collective Communications Library（oneCCL）の通信タスク

## 特性分析の設定
特性構成オプションを使用して以下を行います
- レンダリングおよびGPGPUエンジンの使用状況を監視する（Intel Graphicsのみ対応）
- エンジンの負荷がかかっている部分を特定する
- GPUとCPUのデータを関連付ける
### 特性を選択すると、GPUメトリクスのプラットフォーム固有のプリセットを選択できます
Dynamic Instruction Countプリセットを除いて、他のすべてのプリセットは以下のデータを収集します（実行ユニットの活動に関して）
- EU配列のアクティブ状態
- EU配列の停止状態
- EU配列のアイドル状態
- コンピューティングスレッドの開始
- スレッドの占有率
- コア周波数
### 各プリセットには追加のメトリクスが含まれています
- **概要メトリクスセット**(overview)では、メモリの読み書き帯域幅やXVEパイプラインの使用率など、一般的なGPUメモリアクセスを追跡する追加のメトリクスを含みます。これらのメトリクスは、グラフィックスや計算集約型のアプリケーションに役立ちます。
- **グローバルメモリアクセス メトリクスグループ**(overview, full-compute)では、GPUとシステムメモリ間、およびGPUスタック間の帯域幅を示します。XVEから遠く離れたメモリレベルでは、不要なメモリアクセス操作が性能に大きな影響を及ぼします。
- **LSE/SLM アクセス メトリクスグループ**(lsc-slm)は、XVEからL1キャッシュへのトラフィックをカバーします。このメトリクスグループは、情報を収集するために2回のアプリケーション実行が必要です。
- **HDC アクセス メトリクスグループ**(hdc)では、L1キャッシュを通過するXVEからL3へのトラフィックを測定します。
- **フルコンピュート メトリクスグループ**(full-compute)は、他のすべてのイベントセットを組み合わせたものです。そのため、複数のアプリケーション実行が必要です。
- **Dynamic Instruction Count メトリクスグループ**(instruction-count)は、特定の命令クラスの実行頻度をカウントします。このメトリクスグループにより、各カーネルによるSIMD利用効率についての洞察も得られます。
- **ビデオコーデックアクティビティおよびビデオ強化アクティビティ メトリクスグループ**(video-requests, video-command-parser-busy)は、メディアエンジンのイベント収集をサポートするIntelハードウェアプラットフォームでGPUコンピュート/メディアホットスポット分析を実行する際に利用可能です。

メディアエンジンからのイベント収集をサポートするIntelアーキテクチャには、以下のコードネームのマイクロアーキテクチャが含まれます（これに限定されません）
- Meteor Lake
- Arrow Lake
- Lunar Lake
- Battlemage


## 特性分析の追加データ
Trace GPU programming APIsオプションを使用して、Intel Processor Graphics上で実行されているSYCL、OpenCL™、またはIntel Media SDKプログラムを分析します。このオプションは、CPU側でアプリケーションのパフォーマンスに影響を与える可能性があります。

**使用上の注意点**
- OpenCLカーネル: Intel Graphics上で動作するWindowsおよびLinuxターゲットで特性分析を実行できます。
- Intel Media SDKプログラム分析: Intel Graphics上で動作するWindowsおよびLinuxターゲットで利用可能です。
- 特性分析のサポート対象: Launch ApplicationおよびAttach to Processターゲットタイプのみがサポートされます。
- Level Zeroランタイム: Attach to Processターゲットタイプをサポートしていません。
- Attach to ProcessモードでのOpenCLカーネルのプロファイリング: 計算キューがすでに作成されているプロセスにアタッチした場合、VTune Profilerはこのキュー内のOpenCLカーネルのデータを表示しません。
- メモリ帯域幅を計算するためのデータ収集: Analyze memory bandwidthオプションを使用します。この分析を行うには、Intelサンプリングドライバーをインストールしてください。
- GPUサンプリング間隔: GPUハードウェアメトリクス収集用のGPUサンプル間隔（ミリ秒単位）を指定します。デフォルトでは、VTune Profilerは1ミリ秒の間隔を使用します

## ソース分析の設定
ソース分析では、VTune Profiler を使用して、パフォーマンスに重要な基本ブロックや、GPU カーネル内のメモリアクセスによる問題を特定できます。
ソース分析のラジオボタンを選択すると、構成ペインがドロップダウンメニューとして展開され、分析したい問題の種類を指定するプロファイリングモードを選択できます：
- **基本ブロック遅延オプション**：アルゴリズムの非効率性による問題を特定するのに役立ちます。このモードでは、VTune Profiler はすべての基本ブロックの実行時間を測定します。基本ブロックとは、単一のエントリポイントと単一の出口ポイントを持つ直線コードシーケンスです。後処理中に、VTune Profiler は基本ブロック内の各命令の実行時間を計算します。このモードは、どの操作がよりコストがかかるかを理解するのに役立ちます
- **メモリ遅延オプション**：メモリアクセスによる遅延問題を特定するのに役立ちます。このモードでは、VTune Profiler はメモリ読み取り/同期命令をプロファイリングし、それらがカーネル実行時間に与える影響を推定します。GPU Compute/Media Hotspots 分析をキャラクタリゼーションモードで実行し、GPU カーネルがスループットまたはメモリにボトルネックがあると特定し、同じ基本ブロックからのどのメモリ読み取り/同期命令がより多くの時間を要しているかを調査したい場合は、このオプションを使用してください。


基本ブロック遅延またはメモリ遅延プロファイリングモードでは、GPU Compute/Media Hotspots 分析は以下の指標を使用します：
- **推定 GPU サイクル数**：プロファイリングされた命令を実行するのに GPU が消費する平均サイクル数
- **平均レイテンシ**：メモリ読み取りおよび同期命令の平均遅延（サイクル単位）
- **インスタンスごとの GPU 命令数**：1 つのカーネルインスタンスあたりに実行される GPU 命令の平均数。
- **スレッドごとの GPU 命令数**：1 つのカーネルインスタンスあたりに 1 スレッドが実行する GPU 命令の平均数。
**命令カウントプロファイリングモード**(instruction-count)を有効にすると、VTune Profiler はカーネルが実行した命令を以下のグループに分類して表示します。
<img width="1037" alt="image" src="https://github.com/user-attachments/assets/ae557598-cd2e-475f-9050-b3a07e93deaf" />


# データの表示
VTune Profilerは分析を実行し、GPUコンピュート/メディアホットスポットのビューでデータを開き、以下のウィンドウでさまざまなプラットフォームデータを提供します
## summary
全体およびエンジンごとのGPU使用率、EUが停止またはアイドル状態であった時間の割合とその潜在的な理由、そして最も負荷の高いGPU計算タスクを表示します。
<img width="1575" alt="image" src="https://github.com/user-attachments/assets/80782a52-90a3-4f9a-9ff3-41bcdc1ebb9d" />

## Graphics
各スレッドごとのCPUおよびGPU使用データを表示し、さまざまな種類のGPUメモリアクセスを分析するためのGPUハードウェアメトリクスの拡張リストを提供します。GPUメトリクスの説明については、グリッド内の列名にカーソルを合わせるか、右クリックして「この列は何ですか？」コンテキストメニューオプションを選択します
<img width="1580" alt="image" src="https://github.com/user-attachments/assets/6ad7d06c-cc90-4dec-b299-23020565eadb" />
