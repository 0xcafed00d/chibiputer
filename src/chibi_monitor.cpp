#include <Arduino.h>
#include "chibi_monitor.h"

namespace Chibi {

	void Monitor::init(IO* io, Core* core) {
		m_core = core;
		m_io = io;

		io->setKeyReceiver(this);
	}

	void Monitor::update() {
	}

	int pos = 0;

	void Monitor::onKey(uint8_t scancode, uint8_t value, bool pressed) {
		if (pressed) {
			m_io->displayDigit(pos, value, false);
			pos = (pos + 1) & 3;
		}
	}
}