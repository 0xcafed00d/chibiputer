#include <Arduino.h>

#include "chibi_io.h"

static const uint8_t hexPatterns[] = {
    0b11111100,  // 0
    0b01100000,  // 1
    0b11011010,  // 2
    0b11110010,  // 3
    0b01100110,  // 4
    0b10110110,  // 5
    0b10111110,  // 6
    0b11100000,  // 7
    0b11111110,  // 8
    0b11110110,  // 9
    0b11101110,  // a
    0b00111110,  // b
    0b10011100,  // c
    0b01111010,  // d
    0b10011110,  // e
    0b10001110   // f
};

namespace Chibi {

	void IO::init(int* commons, int* segments, int* padcols) {
		m_commons = commons;
		m_segments = segments;
		m_padcols = padcols;
		m_currentDigit = 0;
		clearDisplay();
		setup();
	}

	void IO::clearDisplay() {
		memset(m_digits, 0, sizeof(m_digits));
	}

	void IO::displayDigit(int index, int value, bool dp) {
		if (value >= 0 && value <= 16) {
			m_digits[index & 3] = hexPatterns[value & 0xf];
		} else {
			m_digits[index & 3] = 7;  // underscore invalid digit.
		}
	}

	void IO::displayHexValue(int val) {
		for (int n = 0; n < 4; n++) {
			displayDigit(n, (val >> 4 * n) & 0xf, false);
		}
	}

	void IO::setDP(int index, bool on) {
		if (on) {
			m_digits[index & 3] |= 0x80;
		} else {
			m_digits[index & 3] &= 0x7f;
		}
	}

	void IO::displayByte(int index, uint8_t value) {
		displayDigit(index * 2 + 1, value >> 4, false);
		displayDigit(index * 2, value & 0xf, false);
	}

	void IO::displayPattern(int index, uint8_t pattern) {
		m_digits[index & 3] = pattern;
	}

	void IO::update() {
		selectDigit(-1);
		selectSegments(m_digits[m_currentDigit]);
		selectDigit(m_currentDigit);
		processPadRow(m_currentDigit, readPad());

		m_currentDigit = (m_currentDigit + 1) & 3;
		if (m_currentDigit == 0) {
			processPad();
		}
	}

	void IO::setup() {
		// set all common select lines to high inpedence inputs
		for (int n = 0; n < 4; n++) {
			pinMode(m_commons[n], INPUT);
		}

		// set all segment lines to lo output
		for (int n = 0; n < 8; n++) {
			pinMode(m_segments[n], OUTPUT);
			digitalWrite(m_segments[n], 0);
		}

		// set all keypad column lines to high inpedence inputs
		for (int n = 0; n < 5; n++) {
			pinMode(m_padcols[n], INPUT_PULLUP);
		}
	}

	void IO::selectDigit(int d) {
		for (int n = 0; n < 4; n++) {
			if (n == d) {
				pinMode(m_commons[n], OUTPUT);
				digitalWrite(m_commons[n], 0);
			} else {
				pinMode(m_commons[n], INPUT);
			}
		}
	}

	void IO::selectSegments(int s) {
		for (int n = 0; n < 8; n++) {
			if (s & 0x80) {
				digitalWrite(m_segments[n], 1);
			} else {
				digitalWrite(m_segments[n], 0);
			}
			s = s << 1;
		}
	}

	int IO::readPad() {
		int res = 0;
		for (int n = 0; n < 5; n++) {
			if (digitalRead(m_padcols[n]) == 0) {
				res = res | (1 << n);
			}
		}
		return res;
	}

	bool testKeyState(uint8_t scanCode) {
		uint8_t i = scanCode % 5;
	}

	void IO::processPadRow(int row, int pressed) {
	}

	void IO::processPad() {
	}
}
