# RasPiDS3
* RasPiでDualShock3の信号を読んでC++で使えるようにしてくれるすごいやつだよ
* **sixadがインストールされて動いていることが前提です**
* GCCとかでコンパイルするときは　-std=c++11 -pthread のオプションをつける
    * C++11に対応したコンパイラでつかおうね
* DualShock3クラスのインスタンスを作って使ってください。
* RasPiとDualShock3のペアリングは[工作と小物のがらくた部屋: Raspberry Pi で Dualshock3 (Bluetooth接続ゲームコントローラ）]( http://junkroom2cyberrobotics.blogspot.jp/2013/03/raspberry-pi-dualshock3-bluetooth.html)を参考にするといいかもしれない.
* Raspberry Pi2だとRT, LTの値, 加速度の値がおかしくなることがあるのを確認しています.
    * 値の変化の仕方は問題なさそうな感じなので頑張れば使えるかもしれない.

## 名状しがたいリファレンスのようなもの
```cpp
RPDS3::DualShock3::DualShock3(const char* devFileName = "/dev/input/js0", bool precision = false, int timeout = 0)
```
* コンストラクタです.
* コントローラが接続されるまでtimeout秒待ちます.
    * timeoutが0だとずっと待ちます
* precisionをtrueにすると精密モードになり、アナログ値が65536段階で見られるようになります.
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
* こまめに呼んでやってください。

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
