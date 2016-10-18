#include <Arduino.h>
#include "chibi_monitor.h"

namespace Chibi {

	Monitor::Monitor() : StateMachine(this), m_lastKey(0xff), m_currentAddr(0x10) {
	}

	void Monitor::init(IO* io, Core* core) {
		m_core = core;
		m_io = io;
		m_cursorPos = 0;
		m_cursorTime = TimeOutms(250);

		io->setKeyReceiver(this);
		stateGoto(&Monitor::stateWaitCmd);
	}

	void Monitor::update() {
		stateUpdate();
	}

	void Monitor::onKey(uint8_t scancode, uint8_t value, bool pressed) {
		if (pressed) {
			m_lastKey = value;
		}
	}

	void Monitor::updateCursor() {
		if (m_cursorPos != -1) {
			if (m_cursorTime.hasTimedOut()) {
				m_cursorTime.reset();
				m_io->setDP(m_cursorPos & 3, (m_cursorPos & 0x80) == 0);
				m_cursorPos ^= 0x80;
			}
		} else {
			m_io->setDP(0, false);
		}
	}

	void Monitor::stateWaitCmd(Phase_t p) {
		if (p == Enter) {
			m_io->clearDisplay();
			m_io->displayDigit(3, 0xc);
			m_cursorPos = 0;
		}
		if (p == Update) {
			updateCursor();
			if (m_lastKey != 0xff) {
				if (m_lastKey < 0x10) {
					m_io->displayDigit(0, m_lastKey);
				}
				if (m_lastKey == 0x83) {
					stateGoto(&Monitor::stateAddressInput);
				}
				m_lastKey = 0xff;
			}
		}
		if (p == Leave) {
			m_io->clearDisplay();
		}
	}

	void Monitor::stateAddressInput(Phase_t p) {
		if (p == Enter) {
			m_io->clearDisplay();
			m_io->displayDigit(3, 0xa);
			m_cursorPos = 1;
		}
		if (p == Update) {
			updateCursor();

			if (m_lastKey != 0xff) {
				if (m_lastKey < 0x10) {
					m_io->displayDigit(m_cursorPos, m_lastKey);
					m_cursorPos = (m_cursorPos - 1) & 1;
				}
				if (m_lastKey == 0x83) {
					stateGoto(&Monitor::stateWaitCmd);
				}
				m_lastKey = 0xff;
			}
		}
		if (p == Leave) {
			m_io->clearDisplay();
		}
	}

	void Monitor::stateDataInput(Phase_t p) {
		if (p == Enter) {
			m_io->clearDisplay();
			m_cursorPos = 1;
		}
		if (p == Update) {
			updateCursor();
			m_io->displayByte(1, m_currentAddr);

			if (m_lastKey != 0xff) {
				if (m_lastKey < 0x10) {
					m_io->displayDigit(m_cursorPos, m_lastKey);
					m_cursorPos = (m_cursorPos - 1) & 1;
				}
				m_lastKey = 0xff;
			}
		}
		if (p == Leave) {
			m_io->clearDisplay();
		}
	}
}
