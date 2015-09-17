# RasPiDS3
RasPiでDualShock3の信号を読む

多分ちゃんと動くようになった

GCCとかでコンパイルするときは　-std=c++11 -pthread のオプションをつける

C++11に対応したコンパイラでつかおうね

RasPiDS3クラスのインスタンスを作って使ってください。

RasPiとDualShock3のペアリングは http://junkroom2cyberrobotics.blogspot.jp/2013/03/raspberry-pi-dualshock3-bluetooth.html を参考にするといいかもしれない


void RPDS3::RasPiDS3::update(); ボタン、スティックの情報が更新されます。こまめに呼んでやってください。

bool RPDS3::RasPiDS3::button(ButtonsNum num, bool onlyFlag = false); 0~15, または SELECT, LEFT_STICK, RIGHT_STICK, START, UP, RIGHT, DOWN, LEFT, L2, R2, L1, R1, TRIANGLE, CIRCLE, CROSS, SQUARE でボタンを指定するとそのボタンが押されてるかどうかを返します(押されてるとtrue) 第二引数にtrueを入れると第一引数で指定されたボタンだけが押されている時のみtrueを返します。

int RPDS3::RasPiDS3::stick(SticksNum num); 0~3, またはLEFT_X, LEFT_Y, RIGHT_X, RIGHT_Y でスティックの軸を指定するとその軸の傾きが返ってきます。
