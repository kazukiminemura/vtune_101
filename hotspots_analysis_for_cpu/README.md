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

## Use-mode sampling 例: アプリケーションのみフォーカス
![image](https://github.com/user-attachments/assets/bea3f0e0-f6a8-45bc-b955-389146406b6f)

## Hardware event-basd sampling 例： 少ないオーバーヘッドでシステム全体を計測
![image](https://github.com/user-attachments/assets/511766cd-9ccf-498e-ad3c-af9159698d45)
