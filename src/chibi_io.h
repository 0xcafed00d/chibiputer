#ifndef CHIBI_IO_H
#define CHIBI_IO_H

#include <Arduino.h>

namespace Chibi {
	class IO {
	  public:
		void init(int* commons, int* segments, int* padcols);
		void clearDisplay();
		void displayDigit(int digit, int value, bool dp);
		void displayHexValue(int value);
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
	};
}

#endif /* CHIBI_IO_H */
