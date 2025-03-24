# GPU Offload Analysis
GPUオフロード分析を使用して、SYCL*、Intel® Media SDK、およびOpenCL™ソフトウェア技術を明示的にサポートするグラフィックス処理ユニット（GPU）を使用したレンダリング、ビデオ処理、計算を実行するアプリケーションを分析します。ツールのインフラストラクチャは、システム内のすべてのコア間でクロックを自動的に調整し、CPUベースのワークロードとGPUベースのワークロードを統一された時間領域内で分析できるようにします。

この分析により以下が可能になります:
- アプリケーションがSYCLまたはOpenCLカーネルをどの程度効果的に使用しているかを特定し、GPU計算/メディアホットスポット分析でさらに詳しく調査する
- Intel Media SDKタスクの実行を時間経過で分析する（Linuxターゲットのみ対応）
- GPU使用状況を調査し、各時点のGPUエンジンのソフトウェアキューを分析する

## 使い方
```
ONEAPI_DEVICE_SELECTOR=level-zero:gpu vtune --collect gpu-offload -- ./a.out
```

```
Generating a report                              Elapsed Time: 69.411s
    GPU Time, % of Elapsed time: 96.7%

        GPU Time, % of Elapsed time
        GPU Engine        GPU Time  GPU Time, % of Elapsed time(%)
        ----------------  --------  ------------------------------
        Render and GPGPU   67.139s                           96.7%

        Top Hotspots when GPU was idle
        Function                  Module           CPU Time
        ------------------------  ---------------  --------
        __intel_avx_rep_memcpy    a.out              0.783s
        dma_map_sg_attrs          vmlinux            0.194s
        shmem_read_folio_gfp      vmlinux            0.177s
        memset                    libc-dynamic.so    0.088s
        [Skipped stack frame(s)]  [Unknown]          0.080s
        [Others]                  N/A                0.845s

Hottest Host Tasks
Host Task                          Task Time  % of Elapsed Time(%)  Task Count
---------------------------------  ---------  --------------------  ----------
zeEventHostSynchronize               67.206s                 96.8%         600
zeCommandListAppendMemoryCopy         0.175s                  0.3%         400
zeCommandQueueExecuteCommandLists     0.033s                  0.0%         600
zeMemAllocHost                        0.029s                  0.0%         601
zeModuleCreate                        0.004s                  0.0%           1
[Others]                              0.006s                  0.0%         608

Hottest GPU Computing Tasks
Computing Task                                                                                                                                                                                                                                                                                                          Total Time  Execution Time  % of Total Time(%)  SIMD Width  Peak XVE Threads Occupancy(%)  XVE Threads Occupancy(%)  SIMD Utilization(%)
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------  ----------  --------------  ------------------  ----------  -----------------------------  ------------------------  -------------------
single_workitem_per_workgroup_parallel_matrix_multiply(std::vector<float, std::allocator<float>>, std::vector<float, std::allocator<float>>, std::vector<float, std::allocator<float>>, int, int, int)::{lambda(sycl::_V1::handler&)#1}::operator()(sycl::_V1::handler&) const::{lambda(sycl::_V1::nd_item<(int)1>)#1}     65.881s         65.580s               99.5%          32                         100.0%                    100.0%               100.0%
even_more_parallel_matrix_multiply(std::vector<float, std::allocator<float>>, std::vector<float, std::allocator<float>>, std::vector<float, std::allocator<float>>, int, int, int)::{lambda(sycl::_V1::handler&)#1}::operator()(sycl::_V1::handler&) const::{lambda(sycl::_V1::id<(int)2>)#1}                               1.481s          1.480s               99.9%          32                         100.0%                     97.4%               100.0%
[Outside any task]                                                                                                                                                                                                                                                                                                          0.023s              0s                0.0%                                                                                      
Collection and Platform Info
    Application Command Line: ./a.out
    Operating System: 6.11.0-19-generic DISTRIB_ID=Ubuntu DISTRIB_RELEASE=24.04 DISTRIB_CODENAME=noble DISTRIB_DESCRIPTION="Ubuntu 24.04.2 LTS"
    Computer Name: intel-Alienware-m16-R2
    Result Size: 438.1 MB
    Collection start time: 13:14:39 24/03/2025 UTC
    Collection stop time: 13:15:49 24/03/2025 UTC
    Collector Type: Event-based sampling driver,Driverless Perf per-process sampling,User-mode sampling and tracing
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

Recommendations:
    XVE Array Stalled/Idle: 83.2% of Elapsed time with GPU busy
     | GPU metrics detect some kernel issues. Use GPU Compute/Media Hotspots
     | (preview) to understand how well your application runs on the specified
     | hardware.

If you want to skip descriptions of detected performance issues in the report,
enter: vtune -report summary -report-knob show-issues=false -r <my_result_dir>.
Alternatively, you may view the report in the csv format: vtune -report
<report_name> -format=csv.
vtune: Executing actions 100 % done
```

## GUI
![Screenshot 2025-03-24 221852](https://github.com/user-attachments/assets/21945dd0-0dca-433c-9071-4035b152c543)

デフォルトでは、GPUオフロード分析により、GPU利用率オプションが有効になり、GPUの稼働状況を時間ごとに確認し、アプリケーションがCPU依存かGPU依存かを理解することができます。その結果、Graphicsウィンドウのタイムラインビューを確認すると、以下のような現象が見られることがあります：
- GPUがほとんどの時間稼働している
- 稼働時間の間に小さなアイドル状態のギャップがある
- GPUソフトウェアキューがほとんどゼロに減少しない

これらの状況が存在する場合、アプリケーションがGPU依存であると結論付けることができます。一方、稼働時間の間のギャップが大きく、ギャップの間にCPUが稼働している場合、アプリケーションはCPU依存であると判断されます。
しかし、このような明確な状況は稀であり、すべての依存関係を理解するには詳細な分析が必要です。たとえば、アプリケーションがGPUエンジンの使用が直列化されている場合（例えば、ビデオ処理を担当するGPUエンジンとレンダリングを担当するGPUエンジンが交互に負荷がかかっている場合）には、誤ってGPU依存とみなされることがあります。

## Summary window
GPUオフロード分析がデータ収集を完了すると以下の指標が表示されます：
- GPU使用率
- GPUアイドル時間
- Xe Link使用状況
- CPUホストで実行された最も活発なコンピューティングタスク
- GPUがアイドル状態のときにCPUで実行された最も活発なコンピューティングタスク
- Intel® oneAPI Collective Communications Library (oneCCL) の通信タスクで最も活発なもの
- GPUで実行された最も活発なコンピューティングタスク（占有率情報を含む

<img width="1577" alt="image" src="https://github.com/user-attachments/assets/0fd39993-eac8-46b9-a1ea-893a197a97bd" />

## 複数のGPU分析
複数のIntel GPUをシステムに接続すると、VTune ProfilerはこれらのアダプタをすべてターゲットGPUプルダウンメニューに識別します。以下のガイドラインに従ってください：
- ターゲットGPUプルダウンメニューを使用して、プロファイルを作成したいデバイスを指定します
- ターゲットGPUプルダウンメニューは、VTune Profilerがシステム上で動作している複数のGPUを検出した場合にのみ表示されます。このメニューには、各GPUの名前とそのアダプタのバス/デバイス/機能（BDF）が表示されます。この情報は、Windows（タスクマネージャーを参照）またはLinux（lspciコマンドを実行）システム上でも確認できます。
- GPUを選択しなかった場合、VTune Profilerはデフォルトでリスト内の最新のデバイスファミリーを選択します
- すべてのデバイスを選択して、システムに接続されているすべてのGPUで分析を実行します。
- マルチアダプタ/タイル分析では、キャラクタライゼーションモードでの完全な計算セットは使用できません。
分析が完了すると、VTune Profilerは各GPUのタイル情報を含む概要結果をサマリーウィンドウに表示します

### GPUアダプタの命名規則
GPUアダプタを指すためにエイリアスを使用します
- エイリアスは、プロファイリング結果のサマリー、グリッド、タイムラインセクションでGPUアダプタを識別します。GPUアダプタの完全な名前は、収集およびプラットフォーム情報セクションに表示され、BDFの詳細も含まれます
- 同じマシンで収集されたすべての結果に対して、1つのエイリアスがGPUアダプタを識別します。
- エイリアスは「GPU 0」「GPU 1」などの命名規則に従います。
- エイリアスの割り当ては以下の順序で行われます
  1. 最も低いPCIアドレスから始まるIntel GPUアダプタ
  2. 非Intel GPUアダプタ
  3. ドライバーなどのその他のソフトウェアデバイス

### GPUトポロジー図
複数のIntel GPU（またはマルチスタックGPU）をシステムに接続してGPU分析を実行すると、サマリーウィンドウにGPU間の接続情報がGPUトポロジー図として表示されます。この図には、システムに接続された最大2つのソケットと6つのGPUに関するクロスGPU情報が含まれます。


