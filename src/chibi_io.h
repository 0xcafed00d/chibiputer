#ifndef CHIBI_IO_H
#define CHIBI_IO_H

#include <Arduino.h>
#include "timeout.h"
/*
Keylayout:
0 1 2 3   Ext0
4 5 6 7	  Ext1
8 9 A B	  Ext2
C D E F   Ext3
*/

namespace Chibi {

	const uint8_t SEG_T = 0b10000000;
	const uint8_t SEG_M = 0b00000010;
	const uint8_t SEG_B = 0b00010000;
	const uint8_t SEG_TL = 0b00000100;
	const uint8_t SEG_TR = 0b01000000;
	const uint8_t SEG_BL = 0b00001000;
	const uint8_t SEG_BR = 0b00100000;
	const uint8_t SEG_DP = 0b00000001;

	const uint8_t LET_A = SEG_T | SEG_M | SEG_BR | SEG_BL | SEG_TL | SEG_TR;
	const uint8_t LET_B = SEG_M | SEG_B | SEG_TL | SEG_BL | SEG_BR;
	const uint8_t LET_C = SEG_M | SEG_B | SEG_BL;
	const uint8_t LET_D = SEG_M | SEG_B | SEG_BL | SEG_TR | SEG_BR;
	const uint8_t LET_E = SEG_T | SEG_M | SEG_B | SEG_BL | SEG_TL;
	const uint8_t LET_F = SEG_T | SEG_M | SEG_BL | SEG_TL;
	const uint8_t LET_G = SEG_T | SEG_B | SEG_BL | SEG_TL | SEG_BR;
	const uint8_t LET_H = SEG_M | SEG_TR | SEG_BL | SEG_TL | SEG_BR;
	const uint8_t LET_I = SEG_BR;
	const uint8_t LET_J = SEG_B | SEG_TR | SEG_BL | SEG_BR;
	const uint8_t LET_K = SEG_M | SEG_T | SEG_BL | SEG_TL | SEG_BR;
	const uint8_t LET_L = SEG_B | SEG_BL | SEG_TL;
	const uint8_t LET_M = SEG_T | SEG_BL | SEG_TL | SEG_BR | SEG_TR;
	const uint8_t LET_N = SEG_M | SEG_BL | SEG_BR;
	const uint8_t LET_O = SEG_M | SEG_B | SEG_BL | SEG_BR;
	const uint8_t LET_P = SEG_M | SEG_T | SEG_BL | SEG_TR | SEG_TL;
	const uint8_t LET_Q = SEG_M | SEG_T | SEG_BR | SEG_TR | SEG_TL;
	const uint8_t LET_R = SEG_M | SEG_BL;
	const uint8_t LET_S = SEG_M | SEG_B | SEG_BR | SEG_TL;
	const uint8_t LET_T = SEG_M | SEG_B | SEG_BL | SEG_TL;
	const uint8_t LET_U = SEG_B | SEG_BL | SEG_BR;
	const uint8_t LET_V = SEG_B | SEG_BR | SEG_TL | SEG_BL | SEG_TR;
	const uint8_t LET_W = SEG_M | SEG_B | SEG_BR | SEG_TL | SEG_BL | SEG_TR;
	const uint8_t LET_X = SEG_M | SEG_BR | SEG_TL | SEG_BL | SEG_TR;
	const uint8_t LET_Y = SEG_M | SEG_B | SEG_BR | SEG_TL | SEG_TR;
	const uint8_t LET_Z = SEG_T | SEG_B | SEG_TR | SEG_BL;

	const uint8_t KEY_0 = 19;
	const uint8_t KEY_1 = 18;
	const uint8_t KEY_2 = 17;
	const uint8_t KEY_3 = 16;
	const uint8_t KEY_4 = 14;
	const uint8_t KEY_5 = 15;
	const uint8_t KEY_6 = 16;
	const uint8_t KEY_7 = 17;
	const uint8_t KEY_8 = 9;
	const uint8_t KEY_9 = 8;
	const uint8_t KEY_A = 7;
	const uint8_t KEY_B = 6;
	const uint8_t KEY_C = 4;
	const uint8_t KEY_D = 3;
	const uint8_t KEY_E = 2;
	const uint8_t KEY_F = 1;

	const uint8_t KEY_EX0 = 15;
	const uint8_t KEY_EX1 = 10;
	const uint8_t KEY_EX2 = 5;
	const uint8_t KEY_EX3 = 0;

	struct KeyReceiver {
		virtual void onKey(uint8_t scancode, uint8_t value, bool pressed) = 0;
	};

	class IO {
	  public:
		void init(int* commons, int* segments, int* padcols);
		void setKeyReceiver(KeyReceiver* kr);
		void update();

		// display functions
		void clearDisplay();
		void displayDigit(int digit, int value, bool dp = false);
		void setDP(int digit, bool on);
		void displayHexValue(int value);
		void displayByte(int index, uint8_t value);
		void displayPattern(int index, uint8_t pattern);

	  private:
		void setup();
		void selectDigit(int d);
		void selectSegments(int s);
		uint8_t readPad();
		void processPadRow(int row, int pressed);
		void processPad();
		void handleKeyRepeat();
		void keyEvent(uint8_t scancode, uint8_t value, bool pressed);

		int* m_commons;
		int* m_segments;
		int* m_padcols;
		int m_currentDigit;
		uint8_t m_digits[4];
		uint8_t m_keymap[4];
		uint8_t m_newKeymap[4];
		KeyReceiver* m_keyreceiver;
		TimeOutus m_scanTimer;
		TimeOutms m_repeatStartTimer;
		TimeOutms m_repeatSpeedTimer;
		uint8_t m_repeatingKey;
	};
}

#endif /* CHIBI_IO_H */
