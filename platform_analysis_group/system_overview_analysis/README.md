# System Overview Analysis
- Intel® VTune™ Profiler 2025.0 バージョンにPlatofrm Profiler機能の最終版が含まれています。
- 機能改善やセキュリティ修正は行われません。

システム概要分析は以下のプロファイリングモードをサポートしています：
- Hardware Even-Based Samplig：コードがCPU、GPU、DRAM、I/O、PCIeをどの程度効率的に利用しているかを識別するためのエントリーポイント分析。
- Hardware Tracing（LinuxおよびAndroidターゲット）：コードをマイクロ秒レベルで分析し、遅延問題の原因を特定するのに役立つモード。
さらに、システム概要分析を使用して、システムの電力使用量データを取得できます。これには、ソケットおよびDRAMモジュールごとの電力使用量の内訳が含まれます。

# Hardware Even-Based Samplig
このモードでは、CPU、GPU、およびI/Oリソースの全体的な使用率をキャプチャし、次のステップに関する推奨事項を確認することができます。このモードは、システムのパフォーマンス問題を分類するためのエントリーレベルの分析として使用します。  

Linuxターゲットの場合、システム概要分析では以下のFtrace*イベントを収集します：
- sched（スケジューリング）
- freq（周波数）
- idle（アイドル状態）
- workq（ワークキュー）
- irq（割り込み）
- softirq（ソフト割り込み）


# Hardware Tracing Mode (Linux and Android Targets)
