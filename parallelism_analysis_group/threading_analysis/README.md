# スレッディング分析
スレッディング分析を使用すると、アプリケーションが利用可能なプロセッサの計算コアをどれだけ効率的に使用しているかを特定し、スレッディングランタイムの使用における非効率性や、スレッディング同期の競合に関する問題点を探ることができます。  
これらの競合は、スレッドを待機状態にして効果的なプロセッサ活用を妨げる原因となります

ntel® VTune™ Profilerは、「Effective CPU Utilization（効果的なCPU使用率）」という指標をスレッディング効率の主な測定基準として使用します。  
この指標は、アプリケーションが利用可能な論理コアをどのように活用しているかを基に構築されています。スループットコンピューティングでは、通常、1つの物理コアにつき1つの論理コアを負荷するのが一般的です。  
スレッディング分析における以下の観点は、CPU使用率が低い理由を示唆する可能性があります：  
- **スレッド数**: アプリケーションのスレッド数をざっと確認することで、スレッディングの非効率性（固定されたスレッド数が原因で、より多くのコアへのスケーリングが制限されたり、スレッドの過剰割り当てが発生したりする）に関する手掛かりを得ることができます。
- **待機時間**（トレースベースまたはコンテキストスイッチベース）: 同期オブジェクトやI/O待ちによるスレッドの待機時間を分析します。
- **スピン待機とオーバーヘッド時間**: スレッディングランタイムのオーバーヘッドやスピンウェイト（ビジーウェイトやアクティブウェイト）の影響を見積もります

<img width="1558" alt="image" src="https://github.com/user-attachments/assets/5d944f63-216a-4086-887a-1669ef5547b0" />

-** ユーザーモードサンプリングとトレーシング**: 同期オブジェクトを認識し、トレーシングを使用してオブジェクトごとのスレッド待機時間を収集できます。これはスレッド間の相互作用セマンティクスを理解し、そのデータに基づいて最適化を行うのに役立ちます。Intel VTune Profiler では、2つの同期オブジェクトグループをサポートしています。一つはスレッド間の同期に通常使用されるオブジェクト（例: ミューテックスやセマフォ）、もう一つはI/O操作の待機に関連するオブジェクト（例: ストリーム）です
- **ハードウェアイベントベースのサンプリングとコンテキストスイッチ**: コンテキストスイッチ情報に基づいてスレッドの非アクティブな待機時間を収集します。この場合、スレッドオブジェクトの定義はありませんが、コールスタックに基づく待機時間を使用することで問題のある同期関数を特定できます。この方法は前の収集モードよりもオーバーヘッドが少なくて済みます。また、コンテキストスイッチに基づく分析ではスレッドのプリエンプション時間も示され、これはスレッド過剰割り当てがシステムに与える影響を測定するのに有用です。

## ユーザーモードサンプリングとトレーシングの仕組み
VTune ProfilerがスレッディングおよびブロッキングAPIをインストルメント化し、実行時に呼び出しをインターセプトしてスレッド相互作用フローを構築し、同期オブジェクトを検出します。
ユーザーモードサンプリングとトレーシング収集分析モードを使用すると、各同期オブジェクトがアプリケーションに与える影響を評価し、アプリケーションが各同期オブジェクトやブロッキングAPIでどの程度の待機時間を要したかを理解できます。
分析では、タイムラインビュー上でスレッドの実行フローが一つのスレッドから別のスレッドに移行し、同期オブジェクトを解放および取得する様子が表示されます。
このモードがアプリケーション実行時に大きなオーバーヘッドを引き起こす場合、待機時間収集をより控えめに行う方法として、ハードウェアイベントベースのサンプリングとコンテキストスイッチモードを試してください。

![image](https://github.com/user-attachments/assets/3af924ef-95ed-4aef-8fff-36d51686b13f)

## ハードウェアイベントベースのサンプリングとコンテキストスイッチの仕組み
マルチタスクオペレーティングシステムでは、すべてのソフトウェアスレッドが一定の時間スライス（スレッド実行クォンタム）内で実行されます。ハードウェアイベントベースのサンプリングとコンテキストスイッチモードでは、スレッドがプロセッサ上でスケジュールされて開始および終了する際（つまりスレッドクォンタムの境界で）、プロファイラーが制御を獲得します。
このモードでは、スレッドが非アクティブ化される理由も特定します。理由には、同期の明示的な要求やスレッドクォンタムの終了（オペレーティングシステムのスケジューラーが現在のスレッドをプリエンプトしてより高優先度のスレッドを実行する場合）などがあります。
スレッドが非アクティブである間の時間は測定され、「非アクティブ待機時間（Inactive Wait Time）」と呼ばれます。非アクティブ待機時間は非アクティブの理由に基づいて区別されます：
- **非アクティブ同期待機時間**（Inactive Sync Wait Time）：同期の要求によって発生します。
- **プリエンプション待機時間**（Preemption Wait Time）：プリエンプトによって発生します

image 

コンテキストスイッチ情報がコールスタックと共に収集されるため、待機関数とその呼び出し経路を通じて非アクティブ待機時間の原因を探ることが可能です。ハードウェアイベントベースのサンプリングとコンテキストスイッチモードでは、同期オブジェクトやI/O操作によって待機が発生したコード上の箇所を表示します。  
このモードは、ハードウェアイベントベースのサンプリング収集に基づいており、システム上で稼働中のすべてのプロセスを分析し、コンテキストスイッチデータを提供することでシステム全体のパフォーマンスを評価します。Linuxシステムでは、ドライバ不要のPerfベースの収集を使用した非アクティブ待機時間収集がカーネルバージョン4.4以降で利用可能です。また、非アクティブ時間の原因はカーネルバージョン4.17以降で利用可能です。


## 使い方
```
vtune --collect threading -knob sampling-and-waits=hw -- ./a.out
```

**結果**
```
Generating a report                              Elapsed Time: 69.108s
    Paused Time: 0.104s
Effective CPU Utilization: 1.5% (0.339 out of 22 logical CPUs)
 | The metric value is low, which may signal a poor logical CPU cores
 | utilization caused by load imbalance, threading runtime overhead, contended
 | synchronization, or thread/process underutilization. Explore sub-metrics to
 | estimate the efficiency of MPI and OpenMP parallelism or run the Locks and
 | Waits analysis to identify parallel bottlenecks for other parallel runtimes.
 |
    Total Thread Count: 4
     | The number of threads in the application is significantly lower than the
     | number of logical cores on the machine. Check if it is resulted by thread
     | number hard coding that limits application scalability.
     |
    Inactive Wait Time with poor CPU Utilization: 1232.611s (100.0% from Inactive Wait Time)
        Inactive Sync Wait Time: 334.203s
        Preemption Wait Time: 898.408s
         | The wait time because of thread preemption is significant and can
         | signal about thread oversubscription or conflicts with other
         | processes or kernel activities by CPU usage. It can also be a result
         | of improper thread affinity. Keeping the thread number correspondent
         | to the number of logical cores on the system can help to avoid thread
         | oversubscription and as a result wait time on thread preemption.
         |

        Top functions by Inactive Wait Time with Poor CPU Utilization.
        Function                      Module                        Inactive Wait Time  Inactive Sync Wait Time  Inactive Sync Wait Count  Preemption Wait Time  Preemption Wait Count
        ----------------------------  ----------------------------  ------------------  -----------------------  ------------------------  --------------------  ---------------------
        func@0x1889a0                 libze_intel_gpu.so.1.6.32567            434.525s                       0s                         0              434.525s                 13,620
        __schedule                    vmlinux                                 320.564s                 115.158s                       602              205.406s                     67
        io_schedule_timeout           vmlinux                                 166.464s                 150.041s                       595               16.423s                      5
        __GI___sched_setaffinity_new  libc.so.6                               163.083s                       0s                         0              163.083s                     22
        usleep_range_state            vmlinux                                  68.946s                  68.946s                       458                    0s                      0
        [Others]                      N/A                                      79.029s                   0.058s                       110               78.971s                     93
    Spin and Overhead Time: 42.377s (64.4% of CPU Time)
     | A significant portion of CPU time is spent waiting. Use this metric to
     | discover which synchronizations are spinning. Consider adjusting spin
     | wait parameters, changing the lock implementation (for example, by
     | backing off then descheduling), or adjusting the synchronization
     | granularity.
     |

        Top Functions with Spin or Overhead Time
        Function             Module                        Spin and Overhead Time  (% from CPU Time)(%)
        -------------------  ----------------------------  ----------------------  --------------------
        func@0x1889a0        libze_intel_gpu.so.1.6.32567                 42.350s                 64.4%
        io_schedule_timeout  vmlinux                                       0.011s                  0.0%
        perf_iterate_ctx     vmlinux                                       0.005s                  0.0%
        usleep_range_state   vmlinux                                       0.003s                  0.0%
        __sched_yield        libc.so.6                                     0.002s                  0.0%
        [Others]             N/A                                           0.006s                  0.0%
Collection and Platform Info
    Application Command Line: ./a.out
    Operating System: 6.11.0-19-generic DISTRIB_ID=Ubuntu DISTRIB_RELEASE=24.04 DISTRIB_CODENAME=noble DISTRIB_DESCRIPTION="Ubuntu 24.04.2 LTS"
    Computer Name: intel-Alienware-m16-R2
    Result Size: 499.2 MB
    Collection start time: 14:37:39 27/03/2025 UTC
    Collection stop time: 14:38:48 27/03/2025 UTC
    Collector Type: Driverless Perf per-process sampling
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

## GUI
スレッディング分析の結果は、「スレッディング効率」ビューに表示され、次のウィンドウ/ペインで構成されています
- 概要ウィンドウ: アプリケーション全体の実行に関する統計を表示し、CPU時間やプロセッサの利用率を特定します
- ボトムアップウィンドウ: ボトムアップツリーにおいてホットスポット関数を表示し、関数ごとのCPU時間およびCPU利用率を示します。
- トップダウンツリーウィンドウ: 呼び出しツリーにおけるホットスポット関数を表示し、関数単独のパフォーマンス指標（Self値）と関数およびその子要素全体のパフォーマンス指標（Total値）を示します。
- 呼び出し元/呼び出し先ウィンドウ: 選択したフォーカス関数の親関数および子関数を表示します。
- プラットフォームウィンドウ: CPUおよびGPUの利用率、フレームレート、メモリ帯域幅、ユーザータスク（該当するメトリックが収集されている場合）に関する詳細を提供します。

## Summary
この分析結果の概要ウィンドウに提供されるアプリケーションレベルのデータをまず分析します。**経過時間（Elapsed time）**の値を、最適化の前後で結果を比較するための基準（ベースライン）として使用してください。

<img width="1380" alt="image" src="https://github.com/user-attachments/assets/dda4fee0-1212-448d-a88a-8adf7b2dcfbc" />

スピン時間、オーバーヘッド時間、待機時間、スレッド総数を調査して、パフォーマンス問題の主な原因を特定してください。

## Bottom-up
高いスレッド待機時間は、CPU利用率の低下を引き起こす可能性があります。並列アプリケーションで一般的な問題の一つは、アプリケーション実行のクリティカルパスにある同期オブジェクト（例: ロック）でスレッドが長時間待機しすぎることです。並列パフォーマンスは、コアが十分に活用されていない状態で待機が発生すると悪化します。スレッディング分析は、スレッド待機時間を解析し、同期のボトルネックを特定するのに役立ちます。
ボトムアップウィンドウを探索して、パフォーマンスに最も重要な同期オブジェクトを特定してください。ただし、場合によっては、あまり重要でないスレッドが、稀に長時間オブジェクトを待機している場合もあります。通常は、待機時間（Wait Time）と待機回数（Wait Count）の両方が高い待機に焦点を当て、それらが効率性や同時実行性に問題を抱えている場合に調整作業を行うことをお勧めします。
デフォルトでは、同期オブジェクトは待機時間順にソートされています。Wait Time by Utilization列ヘッダーのボタンをクリックして列を展開すると、利用率ごとの時間分布を表示できます。

<img width="1589" alt="image" src="https://github.com/user-attachments/assets/0442108d-f8c8-480d-a27d-cfc0453122c7" />


**同期オブジェクトのスタック寄与度の特定**
「ボトムアップ」または「トップダウンツリー」ペインで選択した同期オブジェクトに対し、スタックペインのナビゲーションボタンを使用すると、その同期オブジェクトに関連する全体の処理時間に対する現在表示中のスタックの寄与度を示す寄与バーを確認できます。また、「コールスタック」ペインのドロップダウンリストを使用して、異なる種類のスタックに関するデータを表示することもできます。
待機時間（Wait Time）が最も長い（またはバー形式が選択されている場合、赤いバーが最も長い）同期オブジェクトおよび待機回数（Wait Count）の値が高い同期オブジェクトについて、待機時間を排除または最小化することを試みてください。
ハードウェアイベントベースのサンプリングとコンテキストスイッチモードでは、関数を非アクティブ同期待機時間（Inactive Sync Wait Time）でソートします。「呼び出し元/呼び出し先」ペインを使用して、非アクティブ同期待機時間が高い待機関数を呼び出しているアプリケーション内の呼び出し箇所を特定します

**スピンおよびオーバーヘッド時間**
スレッディング分析では、アプリケーションがスレッディングランタイムでどれだけの時間を費やしているかを示します。この時間には、ビジーウェイト（アクティブウェイト）や並列作業のスケジューリングに伴うオーバーヘッドが含まれます。目標は、アクティブウェイトやタスクスケジューリングに費やされるCPUサイクルを最小限に抑えることです。

**スレッド数（Thread Count）**
スレッディング分析では、アプリケーションがスレッド数の過剰割り当て（オーバーサブスクリプション）状態に費やした時間を表示します。これは、アプリケーションがマシン上の論理コア数を超えるスレッドを実行している場合にフラグとして示されます。過剰なスレッド数で実行すると、一部のスレッドが他のスレッドの完了を待機したり、コンテキストスイッチに時間が浪費されるため、CPU時間が増加する可能性があります。
もう一つの一般的な問題は、スレッド数が固定されている場合です。この場合、異なるコア数を持つプラットフォーム上で実行すると、パフォーマンスが低下する可能性があります。例えば、利用可能なコア数よりも大幅に少ないスレッド数で実行すると、アプリケーションの経過時間が増加することがあります。
概要ウィンドウにあるTotal Thread Count（スレッド総数）メトリックを使用して、アプリケーションにスレッド数の過剰割り当てがあるか、スレッド数の増加が有益かを判断してください。
ハードウェアイベントベースのサンプリングとコンテキストスイッチモードでは、Preemption Wait Time（プリエンプション待機時間）メトリックを使用して、過剰割り当ての影響を見積もります。作業スレッドにおけるこのメトリックの値が高いほど、過剰割り当てがアプリケーション性能に与える影響が大きいことを示します。なお、スレッドのプリエンプションは、他のアプリケーションやシステム上で実行中のカーネルスレッドとの競合によっても引き起こされる可能性があります。

**タイムラインの確認**
「ボトムアップ/トップダウンツリー」ウィンドウの下部にあるタイムラインペインでは、アプリケーション内のスレッドの挙動やCPU利用率のメトリックが時間経過とともにどのように変化するかを確認できます。データを分析し、問題のある領域を選択して、コンテキストメニューオプションを使用してズームインします。
VTune Profilerは、「スレッド」エリア内の各スレッドごとのCPU時間の合計として、全体的なCPU利用率メトリックを計算します。最大CPU利用率の値は、「プロセッサコアの数」×100% に相当します。

<img width="1585" alt="image" src="https://github.com/user-attachments/assets/3e8207c4-53a4-45a3-a774-0233caa0a506" />

特定の時間枠でアプリケーションの動作を理解する方法はタイムライン上で特定の範囲を選択し、右クリックして「Zoom In（ズームイン）」および「Filter In by Selection（選択範囲でフィルター）」を選択してください。この操作により、VTune Profilerがその時間範囲内で使用された関数や同期オブジェクトを表示します。
ユーザーモードサンプリングとトレーシング収集モードでは、タイムラインの「Transitions（遷移）」オプションを選択してスレッドの相互作用を調査します。

<img width="1595" alt="image" src="https://github.com/user-attachments/assets/8d1c38f6-a7d0-4a30-a7e8-2d716a5f7d6e" />


ハードウェアイベントベースのサンプリングとコンテキストスイッチモードにおけるタイムラインを活用すると非アクティブ待機を調査するのに役立ちます。タイムライン上で非アクティブ時間の領域を選択すると、コンテキストスイッチに対応する待機スタックがスタックペインに表示されます




