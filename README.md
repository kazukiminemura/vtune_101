# vtune_101
ここではIntel VTuneのスタンドアロン版の使い方の紹介をしています。

## 前提条件
以下の Intel ソフトウェアツールを Linux システムにダウンロードしてください:
- Intel® oneAPI DPC++/C++ コンパイラー
- Intel® VTune™ Profiler バージョン 2025.0 以降
これらのツールは Intel® oneAPI Base Toolkit に含まれています。

## Ubuntu24.04LTS上でのセットアップ手順
oneAPI Base Toolkitのオフラインインストーラーをダウンロード(https://www.intel.com/content/www/us/en/developer/tools/oneapi/base-toolkit-download.html?packages=oneapi-toolkit&oneapi-toolkit-os=linux&oneapi-lin=offline)
```
$ sudo sh ./intel-oneapi-toolkit-2025.0.1.46_offline.sh -a --cli --silent --eula=accept
```
### Intel GPUのセットアップ
Intel GPUドライバーをインストール(https://dgpu-docs.intel.com/driver/client/overview.html)
sycl-lsコマンドでGPUデバイスのリストを表示
```
$ sycl-ls
[level_zero:gpu][level_zero:0] Intel(R) oneAPI Unified Runtime over Level-Zero, Intel(R) Graphics [0x7d55] 12.71.4 [1.6.32224+14]
[opencl:cpu][opencl:0] Intel(R) OpenCL, Intel(R) Core(TM) Ultra 7 155H OpenCL 3.0 (Build 0) [2024.18.12.0.05_160000]
[opencl:gpu][opencl:1] Intel(R) OpenCL Graphics, Intel(R) Graphics [0x7d55] OpenCL 3.0 NEO  [24.52.32224]
```
### Intel Metrics Discovery APIライブラリのインストール
libdrm-devパッケージをインストールします。
``` $ sudo apt install libdrm-dev ```
#### 権限の有効化
ビデオグループにユーザーを追加します。
``` $ groups | grep video ```
ユーザー名が表示されない場合：
``` $ sudo usermod -a -G video intel ```
###　カーネルのアクセス権限を変更
```
# sudo echo 0 | sudo tee /proc/sys/dev/i915/perf_stream_paranoid
$ sudo echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope
$ sudo echo 0 | sudo tee /proc/sys/kernel/kptr_restrict
```

### ドライバーの確認
VTune Profilerのドライバーを確認します。

```
/opt/intel/oneapi/vtune/latest/sepdk/src/insmod-sep -q
pax driver is not loaded.
socperf3 driver is not loaded.
sep5 driver is not loaded.
Warning: skipping SOCWATCH driver, not built
vtsspp driver is not loaded.
```
もしドライバーが上記のように見つからない場合
root-userでビルド＆インストール
```
apt install build-essential
cd /opt/intel/oneapi/vtune/latest/sepdk/src
./build-driver
./insmod-sep -r -g vtune
```
以下のようになれば成功
```
/opt/intel/oneapi/vtune/latest/sepdk/src/insmod-sep -q
pax driver is loaded and owned by group "vtune" with file permissions "660".
socperf3 driver is loaded and owned by group "vtune" with file permissions "660".
sep5 driver is loaded and owned by group "vtune" with file permissions "660".

socwatch2_15 driver is loaded and owned by group "vtune" with file permissions "660".

vtsspp driver is loaded and owned by group "vtune" with file permissions "660".
```

### VTuneの検証（rootユーザーで実行）
```
/opt/intel/oneapi/vtune/latest/bin64/vtune-self-checker.sh
```


## WSL2上のMeteor Lake-PでUbuntuをセットアップする手順 (機能が制限されています）
Windows機能で「Windows Subsystem for Linux」を有効にし、コンピュータを再起動
WSL2上でUbuntu 24.04.xをセットアップ
あとはUbuntuと同じ手順
