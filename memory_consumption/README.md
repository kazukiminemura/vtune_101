# メモリ消費量
デフォルトでは、分析結果は「メモリ消費量」ビューに表示されます。タイムラインペインでメモリ消費量のピークを特定し、ホットスポット関数の割り当てスタックを分析します。ホットスポット関数をダブルクリックすると、「ソース」ビューに切り替わり、大量のメモリを割り当てているソース行を分析できます。

# 使い方
```
vtune --collect memory-consumption -- ./a.out
```

結果例
```
Generating a report                              Elapsed Time: 8.100s
    Allocation Size: 503.2 MB
    Deallocation Size: 440.3 MB
    Allocations: 321,187
    Total Thread Count: 3
    Paused Time: 0s

Top Memory-Consuming Functions
Function                            Memory Consumption  Allocation/Deallocation Delta  Allocations  Module
----------------------------------  ------------------  -----------------------------  -----------  -----------------------------
L0HostMemoryProvider::allocateImpl           180.2 MB                         64.0 B           144  libur_adapter_level_zero.so.0
urProgramBuildExp                            127.0 MB                         2.6 MB       283,995  libur_adapter_level_zero.so.0
main                                          88.1 MB                          0.0 B            21  a.out
USMSharedAllocImpl                            45.6 MB                        12.6 MB            60  libur_adapter_level_zero.so.0
urPlatformGet                                 24.1 MB                        23.4 MB        12,143  libur_adapter_level_zero.so.0
[Others]                                      38.2 MB                        24.3 MB        24,824  N/A 
```

# GUI
Summary  
![image](https://github.com/user-attachments/assets/221df7b5-90ec-4162-a571-07284430a722)

Bottom-up  
![image](https://github.com/user-attachments/assets/039f504f-5e15-432d-8634-f944812247ed)

