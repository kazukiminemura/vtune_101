# Hotspots for CPU

User-Mode Sampling
```
ONEAPI_DEVICE_SELECTOR=opencl:cpu vtune --collect hotspots -- ./a.out
```
Hardware Even-based Sampling
```
ONEAPI_DEVICE_SELECTOR=opencl:cpu vtune --collect hotspots -knob sampling-mode=hw -knob enable-stack-collection=true -- ./a.out

```

# GUI
VTuneプロファイラーはInstruction Pointer(IP)のサンプルをdata collectionに保存し解析する。

## 分析ビュー
**サマリーウィンドウ：**  
アプリケーション全体の実行統計を表示し、CPU時間とプロセッサの利用率を分析します。  
**ボトムアップウィンドウ：**  
ボトムアップツリーでホットスポット関数を表示し、関数ごとのCPU時間とCPU利用率を示します。  
**トップダウンツリーウィンドウ：**  
コールツリーでホットスポット関数を表示し、関数のみのパフォーマンス指標（自己値）と関数とその子関数を合わせたパフォーマンス指標（合計値）を示します。  
**コーラー/カリーウィンドウ：**  
選択されたフォーカス関数の親関数と子関数を表示します。  
**プラットフォームウィンドウ：**  
CPUとGPUの利用率、フレームレート、メモリ帯域幅、およびユーザータスクの詳細を提供します（対応するメトリックが収集されている場合）。  


## Use-mode sampling: アプリケーションのみフォーカス
![image](https://github.com/user-attachments/assets/bea3f0e0-f6a8-45bc-b955-389146406b6f)

## Hardware event-basd sampling： 少ないオーバーヘッドでシステム全体を計測
![image](https://github.com/user-attachments/assets/511766cd-9ccf-498e-ad3c-af9159698d45)
