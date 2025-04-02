# MPIアプリケーションのプロファイリング
アプリケーション: [https://github.com/AlexeyMalkhanov/Cardiac_demo](heart_demo) サンプルアプリケーション
ツール:
- Intel® C++ コンパイラ
- Intel® MPI ライブラリ 2021.11
- Intel VTune Profiler 2024.0 以降
- Intel VTune Profiler - Application Performance Snapshot
最新バージョンの VTune Profiler は、製品ダウンロードページからダウンロードしてください。 オペレーティングシステム: Linux*

CPU: Intel® Xeon® Platinum 8480+ プロセッサ（コードネーム：Sapphire Rapids）

# 使い方
**ビルド**
```
source /opt/intel/oneapi/setvars.sh
git clone https://github.com/CardiacDemo/Cardiac_demo.git
cd Cardiac_demo
mkdir build
cd build
mpiicpx ../heart_demo.cpp ../luo_rudy_1991.cpp ../rcm.cpp ../mesh.cpp -g -o heart_demo -O3 -std=c++17 -qopenmp -parallel-source-info=2
```

**注意**
サンプルコードが古いので、以下のエラーが出ます。"class vector"を"class std::vector"に書き換えてください。
```
../mesh.cpp:9:16: error: explicit instantiation of 'std::vector' must occur in namespace 'std'
    9 | template class vector<Facet>;
```


**実行**
```
export OMP_NUM_THREADS=4
mpirun -np 16 -ppn 4 aps ./heart_demo -m ../mesh_mid -s ../setup_mid.txt -t 100
```

**結果**
```
MPI rank 7 has started on intel-Alienware-m16-R2
MPI rank 5 has started on intel-Alienware-m16-R2
MPI rank 1 has started on intel-Alienware-m16-R2
MPI rank 2 has started on intel-Alienware-m16-R2
MPI rank 4 has started on intel-Alienware-m16-R2
MPI rank 8 has started on intel-Alienware-m16-R2
MPI rank 3 has started on intel-Alienware-m16-R2
MPI rank 6 has started on intel-Alienware-m16-R2
MPI rank 13 has started on intel-Alienware-m16-R2
MPI rank 10 has started on intel-Alienware-m16-R2
MPI rank 11 has started on intel-Alienware-m16-R2
MPI rank 14 has started on intel-Alienware-m16-R2
MPI rank 12 has started on intel-Alienware-m16-R2
MPI rank 9 has started on intel-Alienware-m16-R2
MPI rank 15 has started on intel-Alienware-m16-R2
MPI rank 0 has started on intel-Alienware-m16-R2
Number of Nodes: 28187
Number of Cells: 112790
Root init took 0.643655 secs
rank 0: r_start=0 r_end=1761 r_len=1762
rank 1: r_start=1762 r_end=3523 r_len=1762
rank 2: r_start=3524 r_end=5285 r_len=1762
rank 3: r_start=5286 r_end=7047 r_len=1762
rank 4: r_start=7048 r_end=8809 r_len=1762
rank 5: r_start=8810 r_end=10571 r_len=1762
rank 6: r_start=10572 r_end=12333 r_len=1762
rank 7: r_start=12334 r_end=14095 r_len=1762
rank 8: r_start=14096 r_end=15857 r_len=1762
rank 9: r_start=15858 r_end=17619 r_len=1762
rank 10: r_start=17620 r_end=19381 r_len=1762
rank 11: r_start=19382 r_end=21142 r_len=1761
rank 12: r_start=21143 r_end=22903 r_len=1761
rank 13: r_start=22904 r_end=24664 r_len=1761
rank 14: r_start=24665 r_end=26425 r_len=1761
rank 15: r_start=26426 r_end=28186 r_len=1761
rank 0: non_local_ids.size(): 14787
rank 1: non_local_ids.size(): 13859
rank 2: non_local_ids.size(): 13381
rank 3: non_local_ids.size(): 13508
rank 4: non_local_ids.size(): 13314
rank 5: non_local_ids.size(): 12779
rank 6: non_local_ids.size(): 12766
rank 7: non_local_ids.size(): 12449
rank 8: non_local_ids.size(): 12310
rank 9: non_local_ids.size(): 11962
rank 10: non_local_ids.size(): 12027
rank 11: non_local_ids.size(): 12098
rank 12: non_local_ids.size(): 13454
rank 13: non_local_ids.size(): 13766
rank 14: non_local_ids.size(): 13952
rank 15: non_local_ids.size(): 13578
rank 0: non_loc_dependecies: (1:337) (2:565) (3:1171) (4:1276) (5:1290) (6:1226) (7:1226) (8:1153) (9:1137) (10:1055) (11:740) (12:911) (13:912) (14:944) (15:844)
rank 1: non_loc_dependecies: (0:324) (2:519) (3:1113) (4:1294) (5:1273) (6:1205) (7:1097) (8:1002) (9:886) (10:918) (11:707) (12:882) (13:903) (14:882) (15:854)
rank 2: non_loc_dependecies: (0:538) (1:537) (3:692) (4:933) (5:1034) (6:1099) (7:1118) (8:1128) (9:1119) (10:1048) (11:737) (12:850) (13:845) (14:870) (15:833)
rank 3: non_loc_dependecies: (0:1045) (1:1051) (2:675) (4:762) (5:892) (6:936) (7:974) (8:1021) (9:1003) (10:1016) (11:770) (12:859) (13:853) (14:854) (15:797)
rank 4: non_loc_dependecies: (0:1047) (1:1209) (2:925) (3:781) (5:781) (6:841) (7:890) (8:939) (9:943) (10:889) (11:722) (12:841) (13:844) (14:824) (15:838)
rank 5: non_loc_dependecies: (0:1023) (1:1157) (2:998) (3:870) (4:786) (6:730) (7:827) (8:869) (9:836) (10:875) (11:645) (12:804) (13:794) (14:789) (15:776)
rank 6: non_loc_dependecies: (0:1003) (1:1048) (2:1024) (3:928) (4:859) (5:747) (7:744) (8:779) (9:767) (10:854) (11:695) (12:870) (13:830) (14:828) (15:790)
rank 7: non_loc_dependecies: (0:1009) (1:980) (2:1052) (3:946) (4:863) (5:833) (6:752) (8:722) (9:721) (10:756) (11:667) (12:746) (13:843) (14:828) (15:731)
rank 8: non_loc_dependecies: (0:930) (1:930) (2:1046) (3:935) (4:902) (5:880) (6:780) (7:755) (9:668) (10:705) (11:634) (12:789) (13:776) (14:798) (15:782)
rank 9: non_loc_dependecies: (0:918) (1:844) (2:1019) (3:975) (4:929) (5:834) (6:772) (7:747) (8:681) (10:675) (11:597) (12:748) (13:767) (14:716) (15:740)
rank 10: non_loc_dependecies: (0:891) (1:857) (2:938) (3:1001) (4:875) (5:875) (6:841) (7:767) (8:699) (9:682) (11:620) (12:767) (13:730) (14:750) (15:734)
rank 11: non_loc_dependecies: (0:637) (1:611) (2:654) (3:667) (4:653) (5:611) (6:642) (7:601) (8:581) (9:558) (10:553) (12:1394) (13:1340) (14:1335) (15:1261)
rank 12: non_loc_dependecies: (0:789) (1:773) (2:729) (3:772) (4:743) (5:731) (6:746) (7:648) (8:710) (9:697) (10:692) (11:1348) (13:1386) (14:1350) (15:1340)
rank 13: non_loc_dependecies: (0:795) (1:808) (2:753) (3:800) (4:779) (5:770) (6:774) (7:747) (8:720) (9:757) (10:703) (11:1353) (12:1413) (14:1305) (15:1289)
rank 14: non_loc_dependecies: (0:848) (1:818) (2:800) (3:810) (4:827) (5:757) (6:789) (7:802) (8:781) (9:732) (10:713) (11:1343) (12:1379) (13:1333) (15:1220)
rank 15: non_loc_dependecies: (0:785) (1:805) (2:806) (3:794) (4:771) (5:762) (6:767) (7:733) (8:769) (9:731) (10:699) (11:1257) (12:1372) (13:1302) (14:1225)
rank 0: num_remote_dependees: [0:0] [1:324] [2:538] [3:1045] [4:1047] [5:1023] [6:1003] [7:1009] [8:930] [9:918] [10:891] [11:637] [12:789] [13:795] [14:848] [15:785]
rank 1: num_remote_dependees: [0:337] [1:0] [2:537] [3:1051] [4:1209] [5:1157] [6:1048] [7:980] [8:930] [9:844] [10:857] [11:611] [12:773] [13:808] [14:818] [15:805]
rank 2: num_remote_dependees: [0:565] [1:519] [2:0] [3:675] [4:925] [5:998] [6:1024] [7:1052] [8:1046] [9:1019] [10:938] [11:654] [12:729] [13:753] [14:800] [15:806]
rank 3: num_remote_dependees: [0:1171] [1:1113] [2:692] [3:0] [4:781] [5:870] [6:928] [7:946] [8:935] [9:975] [10:1001] [11:667] [12:772] [13:800] [14:810] [15:794]
rank 4: num_remote_dependees: [0:1276] [1:1294] [2:933] [3:762] [4:0] [5:786] [6:859] [7:863] [8:902] [9:929] [10:875] [11:653] [12:743] [13:779] [14:827] [15:771]
rank 5: num_remote_dependees: [0:1290] [1:1273] [2:1034] [3:892] [4:781] [5:0] [6:747] [7:833] [8:880] [9:834] [10:875] [11:611] [12:731] [13:770] [14:757] [15:762]
rank 6: num_remote_dependees: [0:1226] [1:1205] [2:1099] [3:936] [4:841] [5:730] [6:0] [7:752] [8:780] [9:772] [10:841] [11:642] [12:746] [13:774] [14:789] [15:767]
rank 7: num_remote_dependees: [0:1226] [1:1097] [2:1118] [3:974] [4:890] [5:827] [6:744] [7:0] [8:755] [9:747] [10:767] [11:601] [12:648] [13:747] [14:802] [15:733]
rank 8: num_remote_dependees: [0:1153] [1:1002] [2:1128] [3:1021] [4:939] [5:869] [6:779] [7:722] [8:0] [9:681] [10:699] [11:581] [12:710] [13:720] [14:781] [15:769]
rank 9: num_remote_dependees: [0:1137] [1:886] [2:1119] [3:1003] [4:943] [5:836] [6:767] [7:721] [8:668] [9:0] [10:682] [11:558] [12:697] [13:757] [14:732] [15:731]
rank 10: num_remote_dependees: [0:1055] [1:918] [2:1048] [3:1016] [4:889] [5:875] [6:854] [7:756] [8:705] [9:675] [10:0] [11:553] [12:692] [13:703] [14:713] [15:699]
rank 11: num_remote_dependees: [0:740] [1:707] [2:737] [3:770] [4:722] [5:645] [6:695] [7:667] [8:634] [9:597] [10:620] [11:0] [12:1348] [13:1353] [14:1343] [15:1257]
rank 12: num_remote_dependees: [0:911] [1:882] [2:850] [3:859] [4:841] [5:804] [6:870] [7:746] [8:789] [9:748] [10:767] [11:1394] [12:0] [13:1413] [14:1379] [15:1372]
rank 13: num_remote_dependees: [0:912] [1:903] [2:845] [3:853] [4:844] [5:794] [6:830] [7:843] [8:776] [9:767] [10:730] [11:1340] [12:1386] [13:0] [14:1333] [15:1302]
rank 14: num_remote_dependees: [0:944] [1:882] [2:870] [3:854] [4:824] [5:789] [6:828] [7:828] [8:798] [9:716] [10:750] [11:1335] [12:1350] [13:1305] [14:0] [15:1225]
rank 15: num_remote_dependees: [0:844] [1:854] [2:833] [3:797] [4:838] [5:776] [6:790] [7:731] [8:782] [9:740] [10:734] [11:1261] [12:1340] [13:1289] [14:1220] [15:0]
Task initialized & distributed
Solve time: 100 ms with solve step 0.003
starting solution...
RANK 4 is using 4 OMP threads
RANK 5 is using 4 OMP threads
RANK 3 is using 4 OMP threads
RANK 11 is using 4 OMP threads
RANK 6 is using 4 OMP threads
RANK 12 is using 4 OMP threads
RANK 1 is using 4 OMP threads
RANK 2 is using 4 OMP threads
RANK 8 is using 4 OMP threads
RANK 13 is using 4 OMP threads
RANK 7 is using 4 OMP threads
RANK 14 is using 4 OMP threads
RANK 9 is using 4 OMP threads
RANK 10 is using 4 OMP threads
RANK 15 is using 4 OMP threads
RANK 0 is using 4 OMP threads
  progress: 100%
solution speed: 0.591346 [model_ms/real_s]
wall time: 169.109
Done
Intel(R) VTune(TM) Profiler 2025.1.0 collection completed successfully. Use the "aps --report /home/intel/projects/Cardiac_demo/build/aps_result_20250402_96120000-d1ee-a222-c731-0600c392f516" command to generate textual and HTML reports for the profiling session.
```
