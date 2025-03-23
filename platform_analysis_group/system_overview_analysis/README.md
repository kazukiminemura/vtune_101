# System Overview Analysis
- Intel® VTune™ Profiler 2025.0 バージョンにPlatofrm Profiler機能の最終版が含まれています。
- 機能改善やセキュリティ修正は行われません。

システム概要分析は以下のプロファイリングモードをサポートしています：
- Hardware Even-Based Samplig：コードがCPU、GPU、DRAM、I/O、PCIeをどの程度効率的に利用しているかを識別するためのエントリーポイント分析。
- Hardware Tracing（LinuxおよびAndroidターゲット）：コードをマイクロ秒レベルで分析し、遅延問題の原因を特定するのに役立つモード。
さらに、システム概要分析を使用して、システムの電力使用量データを取得できます。これには、ソケットおよびDRAMモジュールごとの電力使用量の内訳が含まれます。

## 使用方法
```
vtune --collect system-overview  -- ./a.out
```
## 結果例
```
Generating a report                              Elapsed Time: 69.343s
    CPU Time: 37.739s
        User: 11.572s
        Kernel: 26.168s
        Unknown: -0.000s
    Context Switch Count: 57,126
        Preemption: 1,022
        Synchronization: 54,375
        I/O Wait: 1,729
    Total Thread Count: 186
    Paused Time: 0s

Top Hotspots
Function        Module                        CPU Time  % of CPU Time(%)
--------------  ----------------------------  --------  ----------------
func@0x1881e0   libze_intel_gpu.so.1.6.32567    6.330s             16.8%
update_curr     vmlinux                         1.782s              4.7%
_raw_spin_lock  vmlinux                         1.646s              4.4%
__schedule      vmlinux                         1.477s              3.9%
func@0xde0      [vdso]                          1.465s              3.9%
[Others]        N/A                            25.039s             66.3%

Average DRAM Bandwidth
Package    Total, GB/sec  Average Bandwidth Read, GB/sec  Average Bandwidth Write, GB/sec
---------  -------------  ------------------------------  -------------------------------
package_0          4.986                           4.763                            0.222

Bandwidth Utilization
Bandwidth Domain  Platform Maximum  Observed Maximum  Average  % of Elapsed Time with High BW Utilization(%)
----------------  ----------------  ----------------  -------  ---------------------------------------------
DRAM, GB/sec      25                          23.400    4.964                                           0.0%
Collection and Platform Info
    Application Command Line: ./a.out
    Operating System: 6.11.0-19-generic DISTRIB_ID=Ubuntu DISTRIB_RELEASE=24.04 DISTRIB_CODENAME=noble DISTRIB_DESCRIPTION="Ubuntu 24.04.2 LTS"
    Computer Name: intel-Alienware-m16-R2
    Result Size: 259.7 MB
    Collection start time: 09:16:48 23/03/2025 UTC
    Collection stop time: 09:17:58 23/03/2025 UTC
    Collector Type: Driverless Perf system-wide sampling
    CPU
        Name: Intel(R) microarchitecture code named Meteorlake-P
        Frequency: 2.995 GHz
        Logical CPU Count: 22
        Cache Allocation Technology
            Level 2 capability: not detected
            Level 3 capability: not detected

Recommendations:
    I/O Wait Time: 67.810s
     | There is significant usage of I/O devices. Use Input and Output to
     | analyze utilization of IO subsystem, CPU, and processor buses.

If you want to skip descriptions of detected performance issues in the report,
enter: vtune -report summary -report-knob show-issues=false -r <my_result_dir>.
Alternatively, you may view the report in the csv format: vtune -report
<report_name> -format=csv.
vtune: Executing actions 100 % done
```

# GUI
## Hardware Even-Based Samplig
このモードでは、CPU、GPU、およびI/Oリソースの全体的な使用率をキャプチャし、次のステップに関する推奨事項を確認することができます。このモードは、システムのパフォーマンス問題を分類するためのエントリーレベルの分析として使用します。  

Linuxターゲットの場合、システム概要分析では以下のFtrace*イベントを収集します：
- sched（スケジューリング）
- freq（周波数）
- idle（アイドル状態）
- workq（ワークキュー）
- irq（割り込み）
- softirq（ソフト割り込み）

![image](https://github.com/user-attachments/assets/6b8ab53e-beff-4e13-a52c-80ed05750d01)


## Hardware Tracing Mode (Linux and Android Targets)
ハードウェアトレーシングモードでは、以下のことが可能です.
- ユーザーモードとカーネルモードの移行および割り込みの分析
- 予期しないプロセスやシステムサービスの実行を調査
- 静的インストルメンテーションなしで、ワークロード実行の特定の段階を測定
- CPUコアの動作をマイクロ秒レベルで分析
- カーネル/ドライバやアプリケーションモジュールを分析し、正確なCPU時間をナノ秒精度で測定
- 以下によって発生するレイテンシー問題の分類とトラブルシューティング
    - 実行コードフローの変更
    - 他のプロセスによるプリエンプション
    - リソース共有の問題
    - ページフォールト
    - 予期しないウェイクアップによる電力消費の問題

**使用例**
```
vtune --collect system-overview -knob collecting-mode=hw-tracing  -- ./a.out
```

![Screenshot 2025-03-23 191205](https://github.com/user-attachments/assets/9c35c01c-aa0f-4e13-8482-e0efcb822da9)

## Power Usage Analysis
システム概要分析の電力消費分析機能を利用して、システムのエネルギー消費特性を把握してください。この機能により、システム全体の消費電力量を詳細に評価することが可能です。これには、特定のコンポーネントやプロセスが消費する電力の分析が含まれ、効率改善のための貴重なデータを提供します。

**使用例**
```
ONEAPI_DEVICE_SELECTOR=opencl:cpu vtune --collect system-overview -knob  analyze-power-usage=true -- ./a.out
```

データ収集が完了したら、SummaryウィンドウのEnergy Consumptionセクションをご確認ください。
このセクションでは、データ収集中にシステム全体で消費された総電力量が表示されます。また、CPUパッケージとDRAMモジュールごとの消費電力の内訳も確認できます。
![image](https://github.com/user-attachments/assets/55ca311f-c390-4e00-b8ff-c49e367fbf11)

Platformウィンドウに切り替えると、時間経過に伴う電力消費の詳細なビューを確認できます。
また、各デバイスの消費電力と、DRAM帯域幅、CPU周波数、CPU使用率の異なる指標を相関させることができます。

**(注記)**
タイムライン上では、デバイスの消費電力が「ミリジュール毎秒（millijoules per second）」として表されますが、これは物理的には「ミリワット（milliwatts）」と等価です。

## Throttling Analysis
もしCPUが安全な温度範囲を超えて動作している場合、システムが安定化を試みる過程でCPUの周波数が大幅に低下することがあります。この周波数低下（スロットリング）は、CPUの動作温度を安全な範囲に戻すために発生し、重大なパフォーマンス低下を引き起こす可能性があります。

このようなCPUのスロットリングの原因を特定するためには、システム概要分析を実行してください。この分析では、温度上昇を引き起こす可能性のある要因や、パフォーマンスに影響を与えるその他のプラットフォームレベルの問題を確認できます。

**使用例**
```
ONEAPI_DEVICE_SELECTOR=opencl:cpu vtune --collect system-overview -knob analyze-throttling-reasons=true  -- ./a.out
```

![Screenshot 2025-03-23 194542](https://github.com/user-attachments/assets/3938d473-05e7-4c35-b982-0c4c68507425)

