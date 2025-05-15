# 使い方
```
icpx bandwith_cache_test.cpp
vtune --collect memory-access -- ./a.out
```

# プロファイル結果 1GB with 112 threads (9.3MB per threads)
25.6％のCPUがL3にデータがないためストール
<img width="401" alt="image" src="https://github.com/user-attachments/assets/30784da2-6e4b-4398-966e-f6e268693224" />


# プロファイル結果 100MB with 112 threads (0.9MB per threads)
ストールなし
![image](https://github.com/user-attachments/assets/4bc00414-2287-4207-ac31-79186985f455)



