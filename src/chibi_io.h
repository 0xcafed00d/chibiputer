#ifndef CHIBI_IO_H
#define CHIBI_IO_H

#include <Arduino.h>

namespace Chibi {

	const uint8_t KEY_NONE = 0;
	const uint8_t KEY_0 = 1;
	const uint8_t KEY_1 = 2;
	const uint8_t KEY_2 = 3;
	const uint8_t KEY_3 = 4;
	const uint8_t KEY_4 = 5;
	const uint8_t KEY_5 = 6;
	const uint8_t KEY_6 = 7;
	const uint8_t KEY_7 = 8;
	const uint8_t KEY_8 = 9;
	const uint8_t KEY_9 = 10;
	const uint8_t KEY_A = 11;
	const uint8_t KEY_B = 12;
	const uint8_t KEY_C = 13;
	const uint8_t KEY_D = 14;
	const uint8_t KEY_E = 15;
	const uint8_t KEY_F = 16;

	const uint8_t KEY_EX1 = 17;
	const uint8_t KEY_EX2 = 18;
	const uint8_t KEY_EX3 = 19;
	const uint8_t KEY_EX4 = 20;

	class IO {
	  public:
		void init(int* commons, int* segments, int* padcols);
		void update();

		// display functions
		void clearDisplay();
		void displayDigit(int digit, int value, bool dp = false);
		void setDP(int digit, bool on);
		void displayHexValue(int value);
		void displayByte(int index, uint8_t value);
		void displayPattern(int index, uint8_t pattern);

		// keypad functions
		bool testKeyState(uint8_t scanCode);

	  private:
		void setup();
		void selectDigit(int d);
		void selectSegments(int s);
		int readPad();
		void processPadRow(int row, int pressed);
		void processPad();

		int* m_commons;
		int* m_segments;
		int* m_padcols;
		int m_currentDigit;
		uint8_t m_digits[4];
		uint8_t m_keymap[4];
		uint8_t m_newKeymap[4];
	};
}

#endif /* CHIBI_IO_H */
