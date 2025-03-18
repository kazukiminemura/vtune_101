# Hotspots for CPU
```
User-Mode Sampling
ONEAPI_DEVICE_SELECTOR=opencl:cpu vtune --collect hotspots -- ./a.out

Hardware Even-based Sampling
ONEAPI_DEVICE_SELECTOR=opencl:cpu vtune --collect hotspots -konb sampling-mode
=hw -knob enable-stack-collection=true -- ./a.out

```


サンプルGUI
![image](https://github.com/user-attachments/assets/bea3f0e0-f6a8-45bc-b955-389146406b6f)



