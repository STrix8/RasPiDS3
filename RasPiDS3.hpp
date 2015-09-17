// RaspberryPiでDualShock3の信号を読取る 
#pragma once

#include <fstream>
#include <thread>

#define UPDATELOOP(c, x) for((c).update(); (x); (c).update())	// コントローラcの状態をupdateしながら条件xでループ 

namespace RPDS3{
	enum ButtonsNum {SELECT, LEFT_STICK, RIGHT_STICK, START, UP, RIGHT, DOWN, LEFT, L2, R2, L1, R1, TRIANGLE, CIRCLE, CROSS, SQUARE, NumButtons};
	enum SticksNum {LEFT_X, LEFT_Y, RIGHT_X, RIGHT_Y, NumSticks};
	class DualShock3 {
	public:
		DualShock3();
		DualShock3(const char*);	// 必要ならファイル名を入れる デフォルトは/dev/input/js0 
		void init(const char*);
		void read();
		void readLoop();
		void update();		// コントローラの状態を更新 
		void yReverseSet(bool setVar = true);
		bool button(ButtonsNum, bool onlyFlag = false);	// 指定されたボタンが押されているか返す 第2引数がtrueだとそのボタンだけが押されている場合のみtrueを返す 
		bool press(ButtonsNum);
		bool release(ButtonsNum);
		int stick(SticksNum);	// 指定されたスティックの状態を返す 
		virtual ~DualShock3();
	private:
		std::fstream JoyStick;
		bool loopFlag;
		bool yReverse;
		static bool threadFlag;
		std::thread readThread;
		static bool readButtonData[NumButtons];
		static int readStickData[NumSticks];
		static bool buttonData[NumButtons];
		static int stickData[NumSticks];
		static bool beforeButtonData[NumButtons];
	};
}
