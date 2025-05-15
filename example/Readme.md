# 使い方
```
icpx bandwith_cache_test.cpp
vtune --collect memory-access -- ./a.out
```

# プロファイル結果 1GB with 112 threads (9.3MB per threads)
25.6％のCPUがL3にデータがないためストール
![image](https://github.com/user-attachments/assets/4bc00414-2287-4207-ac31-79186985f455)
