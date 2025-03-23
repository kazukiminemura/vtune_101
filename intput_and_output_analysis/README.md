# Input and Output Analysis
Intel® VTune™ Profiler の入力および出力分析は、以下を特定するのに役立ちます
- 外部PCIeデバイスおよび統合型アクセラレータによるプラットフォームI/O消費
  -  Intel® Data Direct I/O Technology (Intel® DDIO)およびメモリマップI/Oトラフィックを含むI/O帯域幅の消費
  -  Intel® DDIOの利用効率
-  メモリ帯域幅の消費
-  Intel® Ultra Path Interconnect (Intel® UPI)の帯域幅消費
-  ソフトウェアデータプレーンの利用率

入力および出力分析には、2つの主要なパフォーマンス指標タイプがあります
- プラットフォームレベルの指標: アプリケーションに依存しないハードウェアイベントに基づく指標
- OSおよびAPI固有の指標: ソフトウェアデータプレーン（DPDKおよびSPDK）、およびLinux*カーネルI/Oスタックのパフォーマンス指標
