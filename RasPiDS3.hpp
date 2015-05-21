// -*- coding:utf-8 -*- 
// RaspberryPiでDualShock3の信号を読取る 
#pragma once

#include <fstream>
#include <string>
#include <thread>

namespace DS3{
	enum ButtonsNum {SELECT, LEFT_STICK, RIGHT_STICK, START, UP, RIGHT, DOWN, LEFT, L2, R2, L1, R1, TRIANGLE, CIRCLE, CROSS, SQUARE, NumButtons};
	enum SticksNum {LEFT_X, LEFT_Y, RIGHT_X, RIGHT_Y, NumSticks};
	class RasPiDS3 {
	public:
//		RasPiDS3(string);	// 必要ならファイル名を入れる デフォルトは/dev/input/js0 
		RasPiDS3(const char*);	// 必要ならファイル名を入れる デフォルトは/dev/input/js0 
		void read();
		void readLoop();
		void update();		// コントローラの状態を更新 
		static bool button(ButtonNum, bool);	// 指定されたボタンが押されているか返す 第2引数がtrueだとそのボタンだけが押されている場合のみtrueを返す 
		static int stick(StickNum);	// 指定されたスティックの状態を返す 
		virtual ~RasPiDS3();
	private:
		fstream JoyStick;
		bool loopFlag;
		static bool threadFlag = false;
		thread readThread;
		bool readButtonData[NumButtons];
		static int readStickData[NumSticks] = {};
		bool buttonData[NumButtons];
		static int stickData[NumSticks] = {};
	};
}
