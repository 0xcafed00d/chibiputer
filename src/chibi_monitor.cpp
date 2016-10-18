#include <Arduino.h>
#include "chibi_monitor.h"

namespace Chibi {

	StateMachine<Monitor>::stateFunction_t Monitor::m_commands[16] = {0,
	                                                                  &Monitor::stateAddressInput,
	                                                                  &Monitor::stateDataInput,
	                                                                  0,
	                                                                  &Monitor::stateRun,
	                                                                  &Monitor::stateStep,
	                                                                  0,
	                                                                  0,
	                                                                  &Monitor::stateLoad,
	                                                                  &Monitor::stateSave,
	                                                                  &Monitor::stateSerialTrace,
	                                                                  &Monitor::stateSerialDump,
	                                                                  &Monitor::stateSoftReset,
	                                                                  &Monitor::stateHardReset,
	                                                                  0,
	                                                                  0};

	Monitor::Monitor()
	    : StateMachine(this), m_cursorBlink(false), m_keyPressed(0xff), m_currentAddr(0x10) {
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
			m_keyPressed = value;
		}
	}

	uint8_t Monitor::getKey() {
		uint8_t key = m_keyPressed;
		m_keyPressed = 0xff;
		return key;
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

	void Monitor::handleCommonKeys(uint8_t key) {
		if (key == KEY_CMD) {
			stateGoto(&Monitor::stateCommand);
		}
		if (key == KEY_UP) {
			m_currentAddr++;
		}
		if (key == KEY_DOWN) {
			m_currentAddr--;
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
			m_io->displayDigit(0, command);
			updateCursor();

			uint8_t key = getKey();
			if (key != 0xff) {
				if (key < 0x10) {
					m_io->displayDigit(0, key);
					command = key;
				}
				if (key == KEY_ENTER) {
					if (m_commands[command]) {
						stateGoto(m_commands[command]);
					}
				}
			}
		}
		if (p == Leave) {
		}
	}

	static uint8_t byteInputHelper(uint8_t pos, uint8_t value, uint8_t inp) {
		if (pos == 0) {
			value &= 0xf0;
			value |= inp;
		} else {
			value &= 0x0f;
			value |= (inp << 4);
		}
		return value;
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

			uint8_t key = getKey();
			if (key != 0xff) {
				if (key < 0x10) {
					m_currentAddr = byteInputHelper(m_cursorPos, m_currentAddr, key);
					m_cursorPos = (m_cursorPos - 1) & 1;
				}
				handleCommonKeys(key);
				if (key == KEY_ENTER) {
					stateGoto(&Monitor::stateCommand);
				}
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

			uint8_t key = getKey();
			if (key != 0xff) {
				if (key < 0x10) {
					uint8_t data = m_core->peek(m_currentAddr);
					data = byteInputHelper(m_cursorPos, data, key);
					m_core->poke(m_currentAddr, data);
					m_cursorPos = (m_cursorPos - 1) & 1;
				}
				handleCommonKeys(key);
				if (key == KEY_ENTER) {
					m_currentAddr++;
				}
			}
		}
		if (p == Leave) {
		}
	}

	void Monitor::stateSoftReset(Phase_t p) {
		if (p == Enter) {
		}
		if (p == Update) {
		}
		if (p == Leave) {
		}
	}

	void Monitor::stateHardReset(Phase_t p) {
		if (p == Enter) {
		}
		if (p == Update) {
		}
		if (p == Leave) {
		}
	}

	void Monitor::stateRun(Phase_t p) {
		if (p == Enter) {
		}
		if (p == Update) {
		}
		if (p == Leave) {
		}
	}

	void Monitor::stateStep(Phase_t p) {
		if (p == Enter) {
		}
		if (p == Update) {
		}
		if (p == Leave) {
		}
	}

	void Monitor::stateSave(Phase_t p) {
		if (p == Enter) {
		}
		if (p == Update) {
		}
		if (p == Leave) {
		}
	}

	void Monitor::stateLoad(Phase_t p) {
		if (p == Enter) {
		}
		if (p == Update) {
		}
		if (p == Leave) {
		}
	}

	void Monitor::stateSerialTrace(Phase_t p) {
		if (p == Enter) {
		}
		if (p == Update) {
		}
		if (p == Leave) {
		}
	}

	void Monitor::stateSerialDump(Phase_t p) {
		if (p == Enter) {
		}
		if (p == Update) {
		}
		if (p == Leave) {
		}
	}
}
