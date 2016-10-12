#include <Arduino.h>
#include "chibi_monitor.h"

namespace Chibi {

	Monitor::Monitor() : StateMachine(this) {
	}

	void Monitor::init(IO* io, Core* core) {
		m_core = core;
		m_io = io;
		m_cursorPos = 0;
		m_cursorTime = TimeOutms(500);

		io->setKeyReceiver(this);
		stateGoto(&Monitor::stateWaitCmd);
	}

	void Monitor::update() {
		updateCursor();
	}

	int pos = 0;

	void Monitor::onKey(uint8_t scancode, uint8_t value, bool pressed) {
		if (pressed) {
			m_io->displayDigit(pos, value, false);
			pos = (pos + 1) & 3;
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

	void Monitor::stateWaitCmd() {
	}
	void Monitor::stateAddressInput() {
	}
	void Monitor::stateDataInput() {
	}
}
