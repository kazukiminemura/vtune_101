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

<img width="1575" alt="image" src="https://github.com/user-attachments/assets/80782a52-90a3-4f9a-9ff3-41bcdc1ebb9d" />

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
- 概要メトリクスセットでは、メモリの読み書き帯域幅やXVEパイプラインの使用率など、一般的なGPUメモリアクセスを追跡する追加のメトリクスを含みます。これらのメトリクスは、グラフィックスや計算集約型のアプリケーションに役立ちます。
- グローバルメモリアクセス メトリクスグループでは、GPUとシステムメモリ間、およびGPUスタック間の帯域幅を示します。XVEから遠く離れたメモリレベルでは、不要なメモリアクセス操作が性能に大きな影響を及ぼします。
- LSE/SLM アクセス メトリクスグループは、XVEからL1キャッシュへのトラフィックをカバーします。このメトリクスグループは、情報を収集するために2回のアプリケーション実行が必要です。
- HDC アクセス メトリクスグループでは、L1キャッシュを通過するXVEからL3へのトラフィックを測定します。
- フルコンピュート メトリクスグループは、他のすべてのイベントセットを組み合わせたものです。そのため、複数のアプリケーション実行が必要です。
- Dynamic Instruction Count メトリクスグループは、特定の命令クラスの実行頻度をカウントします。このメトリクスグループにより、各カーネルによるSIMD利用効率についての洞察も得られます。
- ビデオコーデックアクティビティおよびビデオ強化アクティビティ メトリクスグループは、メディアエンジンのイベント収集をサポートするIntelハードウェアプラットフォームでGPUコンピュート/メディアホットスポット分析を実行する際に利用可能です。

