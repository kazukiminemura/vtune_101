# vtune_101
ここではIntel VTuneのスタンドアロン版の使い方の紹介をしています。

## 前提条件
以下の Intel ソフトウェアツールを Linux システムにダウンロードしてください:
- Intel® oneAPI DPC++/C++ コンパイラー
- Intel® VTune™ Profiler バージョン 2025.0 以降
これらのツールは Intel® oneAPI Base Toolkit に含まれています。

## Ubuntu24.04LTS上でのセットアップ手順　****
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
### システム設定を変更します。
```
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
### 適切なドライバーを検証します（rootユーザーで実行）。

/opt/intel/oneapi/vtune/latest/bin64/vtune-self-checker.sh


## WSL2上のMeteor Lake-PでUbuntuをセットアップする手順 (機能が制限されています）
Windows機能で「Windows Subsystem for Linux」を有効にし、コンピュータを再起動
WSL2上でUbuntu 24.04.xをセットアップ
あとはUbuntuと同じ手順



# 紹介
VTune Profilerを使用して以下のことを確認または特定します：
アプリケーションおよび/またはシステム全体で最も時間のかかる（ホット）関数 • 利用可能なプロセッサ時間を効果的に利用していないコードのセクション

順次パフォーマンスおよびスレッドパフォーマンスの最適化に最適なコードセクション • アプリケーションのパフォーマンスに影響を与える同期オブジェクト

アプリケーションが入出力操作に費やす時間の場所と理由

アプリケーションがCPUまたはGPUに依存しているか、GPUへのコードオフロードがどれだけ効果的か

異なる同期方法、異なるスレッド数、または異なるアルゴリズムのパフォーマンス影響

スレッドのアクティビティと移行

データ共有、キャッシュミス、分岐予測ミスなどのコード内のハードウェア関連の問題

# 使用モデル
WindowsまたはLinuxプラットフォームにVTune Profilerをインストールします

グラフィカルユーザーインターフェイスを使用してVTune Profilerを操作するか、コマンドラインから分析を実行します

マルチユーザー環境では、VTune ProfilerをWebサーバーとして使用します。

スタンドアロンのGUIクライアントをインストールするか、Microsoft Visual StudioやEclipseなどのIDEにVTune Profilerを統合します。
