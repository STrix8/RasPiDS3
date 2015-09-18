# RasPiDS3
RasPiでDualShock3の信号を読む

多分ちゃんと動くようになった

GCCとかでコンパイルするときは　-std=c++11 -pthread のオプションをつける

C++11に対応したコンパイラでつかおうね

DualShock3クラスのインスタンスを作って使ってください。

RasPiとDualShock3のペアリングは http://junkroom2cyberrobotics.blogspot.jp/2013/03/raspberry-pi-dualshock3-bluetooth.html を参考にするといいかもしれない


RPDS3::DualShock3::DualShock3(const char *devFileName = "/dev/input/js0", bool precision = false);  コンストラクタ precisionをtrueにすると精密モードになり、アナログ値が256段階から65536段階になります.

void RPDS3::DualShock3::update(); ボタン, スティック, 加速度の情報が更新されます。こまめに呼んでやってください。

void RPDS3::DualShock3::precisionMode(bool precision = true); 精密モードの切り替えを行います

bool RPDS3::DualShock3::button(ButtonsNum button, bool onlyFlag = false); 0~15, または SELECT, LEFT_STICK, RIGHT_STICK, START, UP, RIGHT, DOWN, LEFT, L2, R2, L1, R1, TRIANGLE, CIRCLE, CROSS, SQUARE でボタンを指定するとそのボタンが押されてるかどうかを返します(押されてるとtrue) 第二引数にtrueを入れると第一引数で指定されたボタンだけが押されている時のみtrueを返します。

int RPDS3::DualShock3::stick(SticksNum stick); 0~5, またはLEFT_X, LEFT_Y, RIGHT_X, RIGHT_Y, LEFT_T, RIGHT_T でスティックの軸またはL2かR2を指定するとその軸の傾きやボタンがどのくらい押し込まれているかが返ってきます。

int RPDS3::DualShock3::acceleration(AxisNum axis); 0~2またはX_AXIS, Y_AXIS, Z_AXISで加速度の軸を指定するとその軸の加速度が返ってきます

bool RPDS3::DualShock3::press(ButtonsNum button); 引数はbutton()の第一引数と同じです。指定されたボタンが前回のupdate()では押されておらず、今回のupdate()で押されている状態に変化した場合、trueを返します。

bool RPDS3::DualShock3::release(ButtonsNum button); 使い方はpress()と同じで、こちらは押されている状態から離されたらtrueを返します。

