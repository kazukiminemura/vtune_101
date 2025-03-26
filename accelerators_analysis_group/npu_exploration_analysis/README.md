# NPUエクスプロレーション分析
ニューラルプロセッシングユニット（NPU）は、オペレーティングシステムによって明示的にオフロードされたAIワークロードのパフォーマンスを加速することができます。NPUは、AIおよび機械学習（ML）ワークロードのパフォーマンスを向上させるために特別に設計されています。Intel® Distribution of OpenVINO™ツールキットを使用して、人気のあるMLモデル（音声認識や画像認識タスクなど）をIntel NPUにオフロードします。その後、NPUエクスプロレーション分析を使用してAIをプロファイルします。

## 準備
sudo apt-get install opencl-headers  
Building a standalone driver: https://github.com/intel/linux-npu-driver/blob/main/docs/overview.md


```
cd linux-npu-driver
git submodule update --init --recursive

cmake -B build -S . -DENABLE_NPU_COMPILER_BUILD=ON
cmake --build build --parallel $(nproc)

# install the driver in the system
sudo cmake --install build

# reload the intel_vpu module to load new firmware
sudo rmmod intel_vpu
sudo modprobe intel_vpu

export LD_LIBRARY_PATH=<path/to/build>/linux-npu-driver_openvino2024/build/lib:$LD_LIBRARY_PATH
```


## NPUデータ収集モード
Intel® VTune™ ProfilerはNPUのパフォーマンスに関するハードウェアメトリクスを次の2つの方法で収集できます：
- **Time-based mode**  
より大きなワークロードに使用します。合理的な効率と低減されたオーバーヘッドでアプリケーションを最適化します。
- **Query-based mode**  
より小さなワークロードに使用します。実行時間が長くなっても、アプリケーションをより効率的に最適化します。ワークロードがDDRメモリに依存している場合に効果的に検査します。

## NPU探索ビュー
Intel Neural Processing Units（NPU）上のAIまたはMLワークロードのパフォーマンスを評価および最適化します。
Intel® VTune™ ProfilerはNPUとDDRメモリ間のDDR帯域幅に関するNOCメトリックセットデータを収集します。
データ収集が完了すると、Intel® VTune™ Profilerは結果を準備し、サマリウィンドウに表示します

# 使い方(windows only)
```
vtune --collect npu -- C:\Users\MTL\Documents\Intel\OpenVINO\openvino_cpp_samples_build\intel64\Release\benchmark_app.exe -m classification.xml -d NPU
```

```
Generating a report                              Elapsed Time: 68.938s
    Total Thread Count: 13
    Paused Time: 0s

NPU Device Load
Device             NPU DDR Data Transferred
-----------------  ------------------------
Intel(R) AI Boost                   1.1 TB

NPU Top Compute Tasks
Computing Task (NPU)     Computing Task Time  Computing Task Count
-----------------------  -------------------  --------------------
zeAppendGraphExecute                110.736s                69,689
zeAppendGraphInitialize               0.000s                     1

Top Tasks
Task Type                          Task Time  Task Count  Average Task Time
---------------------------------  ---------  ----------  -----------------
zeFenceHostSynchronize               58.915s      69,694             0.001s
zeCommandQueueExecuteCommandLists     0.688s      69,694             0.000s
zeMemAllocHost                        0.008s           8             0.001s
zeCommandQueueCreate                  0.008s           2             0.004s
zeCommandListCreate                   0.000s           5             0.000s
Collection and Platform Info
    Application Command Line: C:\Users\MTL\Documents\Intel\OpenVINO\openvino_cpp_samples_build\intel64\Release\benchmark_app.exe "-m" "classification.xml" "-d" "NPU"
    Operating System: Microsoft Windows 10
    Computer Name: intel
    Result Size: 173.9 MB
    Collection start time: 01:19:12 26/03/2025 UTC
    Collection stop time: 01:20:22 26/03/2025 UTC
    Collector Type: User-mode sampling and tracing
    CPU
        Name: Intel(R) microarchitecture code named Meteorlake-P
        Frequency: 2.995 GHz
        Logical CPU Count: 22
        Cache Allocation Technology
            Level 2 capability: not detected
            Level 3 capability: not detected

If you want to skip descriptions of detected performance issues in the report,
enter: vtune -report summary -report-knob show-issues=false -r <my_result_dir>.
Alternatively, you may view the report in the csv format: vtune -report
<report_name> -format=csv.
vtune: Executing actions 100 % done
```


# Summary window
サマリウィンドウには、次のセクションから始まるNPUパフォーマンスデータが表示されます
- NPUデバイス負荷 - このセクションは、NPUとDDRメモリ間で転送されたデータ量を示します。
- NPUトップ計算タスク - このセクションは、タスクがNPU上で実行された総時間を記録します。

![image](https://github.com/user-attachments/assets/b21bff4a-a4d5-45f5-b3f7-5073c3caa95a)


# Bottom-up window
ホストタスクの検査を続けるには、ボトムアップウィンドウに切り替えます。グループ化プルダウンメニューで、"Task Domain/Task Type/Function/Call Stack"のグループ化を選択します。
![image](https://github.com/user-attachments/assets/3c047e66-ef35-4c7f-8a77-d1efc24b48fb)



デバイスタスクが開始された瞬間からの実行を確認します。これは、タスクがコンピューティングキューに追加された瞬間です。
コンピューティングキューセクションでは、点線の上のグラフ部分がタスクがNPUで実行された期間を示します。
点線の下のグラフ部分は、タスクがNPUで実行されるためにキューで待機していた期間を示します。タスクはNPUでの実行が完了するとコンピューティングキューから削除されます。
