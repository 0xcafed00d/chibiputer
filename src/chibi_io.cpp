#include <Arduino.h>

#include "chibi_io.h"

namespace Chibi {

	void IO::init(int* commons, int* segments, int* padcols) {
		m_commons = commons;
		m_segments = segments;
		m_padcols = padcols;
		m_currentDigit = 0;
	}

	void IO::displayChar(int digit, char value, bool dp) {
	}

	void IO::update() {
		selectDigit(-1);
		selectSegments(m_digits[m_currentDigit]);
		selectDigit(m_currentDigit);
		processPad(m_currentDigit, readPad());

		m_currentDigit = (m_currentDigit + 1) & 3;
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
		s++;
		for (int n = 0; n < 8; n++) {
			if (s & 1) {
				digitalWrite(m_segments[n], 1);
			} else {
				digitalWrite(m_segments[n], 0);
			}
			s = s >> 1;
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

	void IO::processPad(int row, int pressed) {
	}
}
