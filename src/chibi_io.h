#ifndef CHIBI_IO_H
#define CHIBI_IO_H

#include <Arduino.h>

namespace Chibi {
	class IO {
	  public:
		void init(int* commons, int* segments, int* padcols);
		void clearDisplay();
		void displayDigit(int digit, int value, bool dp = false);
		void setDP(int digit, bool on);
		void displayHexValue(int value);
		void displayByte(int index, uint8_t value);
		void displayPattern(int index, uint8_t pattern);
		void update();

	  private:
		void setup();
		void selectDigit(int d);
		void selectSegments(int s);
		int readPad();
		void processPad(int row, int pressed);

		int* m_commons;
		int* m_segments;
		int* m_padcols;
		int m_currentDigit;
		uint8_t m_digits[4];
		uint8_t m_keymap[5];
		uint8_t m_prevKeyMap[5];
	};
}

#endif /* CHIBI_IO_H */
