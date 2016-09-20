#include <Arduino.h>
#include "chibi_monitor.h"
#include "chibi_io.h"
#include "chibi_core.h"

namespace Chibi {

	void Monitor::init(IO* io, Core* core) {
		m_core = core;
		m_io = io;

		io->setKeyReceiver(this);
	}

	void Monitor::update() {
	}

	void Monitor::onKey(uint8_t scancode, bool pressed) {
		Serial.print(scancode);
		Serial.print("->");
		Serial.println(pressed ? "down" : "up");
	}
}