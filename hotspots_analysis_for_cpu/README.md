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

## 最も負荷の高い関数を特定
ホットスポット関数をクリックして、その呼び出しフローやその他の関連メトリクスをBottom-upビューで探索  
デフォルトでは、Bottom-upビューはCPU時間を降順で表示し、最も時間を消費する関数から始まります。  
CPU時間が最も多い関数から最適化を開始します。  

## フレームグラフウィンドウ (ホットコードパスの特定)
アプリケーション内の最も負荷の高いコードパスを迅速に特定します。  
各プログラムユニットとその関連する呼び出し関数に費やされたCPU時間を分析します。  
フレームグラフは、スタックプロファイルの分布（アルファベット順に並べ替え）を横軸にプロットします。  
縦軸はスタックの深さを示し、下からゼロで始まります。フレームグラフの各要素の幅は、関数（およびその呼び出し関数）のCPU時間の割合を示します。  
![image](https://github.com/user-attachments/assets/59d410a9-60c6-4962-bd9b-83b00f3b3125)

## アルゴリズムの問題特定
### Top-down Treeペイン
ホットスポット関数の呼び出し元と呼び出し先の総時間と自己時間データを分析して、この時間が最適化できるかどうかを理解します。  
### Call Stackペイン
Bottom-upまたはTop-down Treeペインで選択されたプログラムユニットの最高の寄与スタックを特定します。  
寄与バーは、現在表示されているスタックが選択されたプログラムユニットに費やされた総時間に対する寄与を示します。  
また、Call Stackペインのドロップダウンリストを使用して、異なる種類のスタックのデータを表示することもできます。  
![image](https://github.com/user-attachments/assets/44e1c612-0f50-44df-b009-fc8a3bfc099d)
