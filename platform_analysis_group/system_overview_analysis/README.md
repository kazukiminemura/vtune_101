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
