# RasPiDS3
* RasPiでDualShock3の信号を読んでC++で使えるようにしてくれるすごいやつだよ
* **RasPiにBluetoothのドングル等が刺さっていること, sixadがインストールされて動いていることが前提です**
* GCCとかでコンパイルするときは　-std=c++11 -pthread のオプションをつける
	* C++11に対応したコンパイラでつかおうね
* DualShock3クラスのインスタンスを作って使ってください。
* [RobotUtilityforRaspberryPi](https://github.com/Owl8/RobotUtilityforRaspberryPi)に移行しました。いろいろ一緒になってるのでそっちを使うようにしてください。
	* クラスの名前とか変わってますが使い方は変わってません。

---
## RasPiとDualShock3のペアリング方法
1. ```sudo apt-get install bluetooth bluez-utils bluez-compat bluez-hcidump libusb-dev libbluetooth-dev```のようにして必要なものををインストールする
	* 必要に応じてこの前に```sudo apt-get update```しておく
2. コマンドラインに```/etc/init.d/bluetooth```と打ち込んで"bluetooth in running."のメッセージが返ってくるのを確認する
	* このあとsixadをインストールして起動するとここが"bluetooth is not running ..."になるが気にしなくてよい
3. ```wget "http://www.pabr.org/sixlinux/sixpair.c" -O sixpair.c``` などしてペアリングツールをダウンロード, ```gcc -o sixpair sixpair.c -lusb```でビルド
4. ビルドが通ったら,USBケーブルでDualShock3とRasPiを接続して```sudo ./sixpair```としてペアリング 済んだらケーブルをはずす
5. ```wget "https://sourceforge.net/projects/qtsixa/files/QtSixA%201.5.1/QtSixA-1.5.1-src.tar.gz/download" -O QtSixA-src.tar.gz```などしてQtSixAをダウンロード, ```tar zxvf QtSixA-src.tar.gz```などして展開する
6. QtSixA-1.5.1/sixadに移動する
7. ここでshared.hを編集し、```#define SHARED_H```のあと、20行目あたりに```#include <unistd.h>```と追記する。こうしないと後のビルドが通らない
8. ```make```でビルド```sudo make install```でインストール
9. ```sudo sixad -start```で起動し、さきほどペアリングしたDualShock3のPSボタンを押して電源を入れ、ペアリングされるのを確認する

### 繋がらない場合
* ```lsusb```などしてRasPiからBluetoothドングルが認識されているか確認してください
* ```hciconfig```などしてドングルが"UP RUNNING PSCAN ISCAN"となっていることを確認してください
	* "DOWN"と表示された場合, ```sudo hciconfig hci0 up piscan```などコマンドラインに打ちこむ
	* "UP RUNNNING"とだけ表示された場合, ```sudo hciconfig hci0 piscan```などコマンドラインに打ちこむ  

 参考 [工作と小物のがらくた部屋: Raspberry Pi で Dualshock3 (Bluetooth接続ゲームコントローラ）]( http://junkroom2cyberrobotics.blogspot.jp/2013/03/raspberry-pi-dualshock3-bluetooth.html)

---
## 名状しがたいリファレンスのようなもの
```cpp
RPDS3::DualShock3::DualShock3(const char* devFileName = "/dev/input/js0", bool precision = false, int timeout = 0)
```
* コンストラクタです.
* コントローラが接続されるまでtimeout秒待ちます.
	* timeoutが0だとずっと待ちます
* precisionをtrueにすると精密モードになり、アナログ値が65536段階の数値で返ってくるようになります. 
	* おそらく精密モードにしても1024段階くらいでしか変化しないです
	* falseだと256段階です.

```cpp
bool RPDS3::Dualshock3::connectedCheck()
```
* コントローラと接続出来たかどうかを返します.

```cpp
void RPDS3::Dualshock3::precisionMode(bool precision = true)
```
* 精密モードの切り替えを行います.

```cpp
void RPDS3::DualShock3::yReverseSet(bool setVar = true)
```
* スティックのY軸の値の符号を反転させます.
    * デフォルトだと奥が負で手前が正なのでそれだとちょっと使いにくい人向け.
    * **値は非精密モードで-127~128になります**

```cpp
void RPDS3::DualShock3::update()
```
* ボタン, スティック, 加速度の情報が更新されます。
    * このメソッドを呼ばないと更新されません.
* プログラムのメインループに書くなどしてこまめに呼んでやってください。

```cpp
void RPDS3::DualShock3::precisionMode(bool precision = true)
```
* 精密モードの切り替えを行います.

```cpp
bool RPDS3::DualShock3::button(ButtonsNum button, bool onlyFlag = false)
```
* 0~15, または SELECT, LEFT_STICK, RIGHT_STICK, START, UP, RIGHT, DOWN, LEFT, L2, R2, L1, R1, TRIANGLE, CIRCLE, CROSS, SQUARE でボタンを指定するとそのボタンが押されてるかどうかを返します(押されてるとtrue).
* 第二引数にtrueを入れると第一引数で指定されたボタンだけが押されている時のみtrueを返します。

```cpp
int RPDS3::DualShock3::stick(SticksNum stick)
```
* 0~5, またはLEFT_X, LEFT_Y, RIGHT_X, RIGHT_Y, LEFT_T, RIGHT_T でスティックの軸またはL2かR2を指定するとその軸の傾きやボタンがどのくらい押し込まれているかが返ってきます.
* スティックは非精密モードで-128~127の範囲ですが, RT, LTは0~255です.

```cpp
int RPDS3::DualShock3::acceleration(AxisNum axis)
```
* 0~2またはX_AXIS, Y_AXIS, Z_AXISで加速度の軸を指定するとその軸の加速度が返ってきます

```cpp
bool RPDS3::DualShock3::press(ButtonsNum button)
```
* 引数はbutton()の第一引数と同じです. 指定されたボタンが前回のupdate()では押されておらず, 今回のupdate()で押されている状態に変化した場合のみ, trueを返します.
* つまりボタンが押されたフレームだけを判定するのに使えます.

```cpp
bool RPDS3::DualShock3::release(ButtonsNum button)
```
* 使い方はpress()と同じで、こちらは押されている状態から離された場合にtrueを返します。
