#include <Arduino.h>
#include "chibi_monitor.h"

namespace Chibi {

	StateMachine<Monitor>::stateFunction_t Monitor::m_commands[16] = {
	    0, &Monitor::stateAddressInput, &Monitor::stateDataInput};

	Monitor::Monitor()
	    : StateMachine(this), m_cursorBlink(false), m_lastKey(0xff), m_currentAddr(0x10) {
	}

	void Monitor::init(IO* io, Core* core) {
		m_core = core;
		m_io = io;
		m_cursorPos = 0;
		m_cursorTime = TimeOutms(250);

		io->setKeyReceiver(this);
		stateGoto(&Monitor::stateCommand);
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
			m_io->setDP(m_cursorPos & 3, m_cursorBlink);
			if (m_cursorTime.hasTimedOut()) {
				m_cursorTime.reset();
				m_cursorBlink = !m_cursorBlink;
			}
		} else {
			m_io->setDP(0, false);
		}
	}

	void Monitor::stateCommand(Phase_t p) {
		static uint8_t command;
		if (p == Enter) {
			m_io->clearDisplay();
			m_io->displayDigit(3, 0xc);
			m_cursorPos = 0;
			command = 0;
		}
		if (p == Update) {
			updateCursor();
			if (m_lastKey != 0xff) {
				if (m_lastKey < 0x10) {
					m_io->displayDigit(0, m_lastKey);
					command = m_lastKey;
				}
				if (m_lastKey == KEY_ENTER) {
					if (m_commands[command]) {
						stateGoto(m_commands[command]);
					}
				}
				m_lastKey = 0xff;
			}
		}
		if (p == Leave) {
		}
	}

	void Monitor::stateAddressInput(Phase_t p) {
		if (p == Enter) {
			m_io->clearDisplay();
			m_io->displayDigit(3, 0xa);
			m_cursorPos = 1;
		}
		if (p == Update) {
			m_io->displayByte(0, m_currentAddr);
			updateCursor();

			if (m_lastKey != 0xff) {
				if (m_lastKey < 0x10) {
					if (m_cursorPos == 0) {
						m_currentAddr &= 0xf0;
						m_currentAddr |= m_lastKey;
					} else {
						m_currentAddr &= 0x0f;
						m_currentAddr |= (m_lastKey << 4);
					}
					m_cursorPos = (m_cursorPos - 1) & 1;
				}
				if (m_lastKey == KEY_ENTER || m_lastKey == KEY_CMD) {
					stateGoto(&Monitor::stateCommand);
				}
				if (m_lastKey == KEY_UP) {
					m_currentAddr++;
				}
				if (m_lastKey == KEY_DOWN) {
					m_currentAddr--;
				}
				m_lastKey = 0xff;
			}
		}
		if (p == Leave) {
		}
	}

	void Monitor::stateDataInput(Phase_t p) {
		if (p == Enter) {
			m_io->clearDisplay();
			m_cursorPos = 1;
		}
		if (p == Update) {
			m_io->displayByte(1, m_currentAddr);
			m_io->displayByte(0, m_core->peek(m_currentAddr));
			updateCursor();

			if (m_lastKey != 0xff) {
				if (m_lastKey < 0x10) {
					uint8_t data = m_core->peek(m_currentAddr);
					if (m_cursorPos == 0) {
						data &= 0xf0;
						data |= m_lastKey;
					} else {
						data &= 0x0f;
						data |= (m_lastKey << 4);
					}
					m_core->poke(m_currentAddr, data);
					m_cursorPos = (m_cursorPos - 1) & 1;
				}
				if (m_lastKey == KEY_CMD) {
					stateGoto(&Monitor::stateCommand);
				}
				if (m_lastKey == KEY_UP || m_lastKey == KEY_ENTER) {
					m_currentAddr++;
				}
				if (m_lastKey == KEY_DOWN) {
					m_currentAddr--;
				}
				m_lastKey = 0xff;
			}
		}
		if (p == Leave) {
		}
	}
}
