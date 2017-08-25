#include <Arduino.h>
#include <EEPROM.h>
#include "chibi_monitor.h"

int getVcc(void)  // Returns actual value of Vcc (x 100)
{
	// For 168/328 boards
	const long InternalReferenceVoltage = 1078L;
	// Adjust this value to your boards specific internal BG voltage x1000
	// REFS1 REFS0          --> 0 1, AVcc internal ref. -Selects AVcc external reference
	// MUX3 MUX2 MUX1 MUX0  --> 1110 1.1V (VBG)         -Selects channel 14, bandgap voltage, to
	// measure
	ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << ADLAR) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) |
	        (0 << MUX0);

	delay(50);  // Let mux settle a little to get a more stable A/D conversion
	            // Start a conversion
	ADCSRA |= _BV(ADSC);
	// Wait for it to complete
	while (((ADCSRA & (1 << ADSC)) != 0))
		;
	// Scale the value
	int results = (((InternalReferenceVoltage * 1023L) / ADC) + 5L) /
	              10L;  // calculates for straight line value
	return results;
}

namespace Chibi {
	StateMachine<Monitor>::stateFunction_t Monitor::m_commands[16] = {
	    0,                               // 0
	    &Monitor::stateAddressInput,     // 1
	    &Monitor::stateDataInput,        // 2
	    0,                               // 3
	    &Monitor::stateRun,              // 4
	    &Monitor::stateStep,             // 5
	    0,                               // 6
	    0,                               // 7
	    &Monitor::stateLoadBegin,        // 8
	    &Monitor::stateSaveBegin,        // 9
	    &Monitor::stateSerialTrace,      // a
	    &Monitor::stateSerialDump,       // b
	    &Monitor::stateSoftReset,        // c
	    &Monitor::stateHardReset,        // d
	    0,                               // e
	    &Monitor::stateBatteryVoltage};  // f

	Monitor::Monitor()
	    : StateMachine(this),
	      m_cursorBlink(false),
	      m_keyPressed(0xff),
	      m_currentAddr(0x10),
	      m_saveLoadSlot(0) {
	}

	void Monitor::init(IO* io, Core* core) {
		m_core = core;
		m_io = io;
		m_cursorPos = 0;
		m_cursorTime = TimeOutms(250);

		io->setKeyReceiver(this);
		stateGoto(&Monitor::stateDisplayCheck);
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
		uint8_t& command = m_stateValue;
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

	void Monitor::stateDisplayCheck(Phase_t p) {
		uint8_t& bits = m_stateValue;
		if (p == Enter) {
			m_io->clearDisplay();
			m_stateTimer = TimeOutms(200);
		}
		if (p == Update) {
			for (int n = 0; n < 4; n++) {
				m_io->displayPattern(n, bits);
			}
			if (m_stateTimer.hasTimedOut()) {
				if (bits == 0xff) {
					stateGoto(&Monitor::stateCommand);
				}
				m_stateTimer.reset();
				bits = (bits << 1) | 1;
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
			m_io->clearDisplay();
			m_io->displayDigit(3, 5);
			m_io->displayPattern(2, LET_R);
		}
		if (p == Update) {
			uint8_t key = getKey();
			if (key == KEY_ENTER) {
				stateGoto(&Monitor::stateCommand);
				m_core->reset(false);
			}
			if (key == KEY_CMD) {
				stateGoto(&Monitor::stateCommand);
			}
		}
		if (p == Leave) {
		}
	}

	void Monitor::stateHardReset(Phase_t p) {
		if (p == Enter) {
			m_io->clearDisplay();
			m_io->displayPattern(3, LET_H);
			m_io->displayPattern(2, LET_R);
		}
		if (p == Update) {
			uint8_t key = getKey();
			if (key == KEY_ENTER) {
				stateGoto(&Monitor::stateCommand);
				m_core->reset(true);
			}
			if (key == KEY_CMD) {
				stateGoto(&Monitor::stateCommand);
			}
		}
		if (p == Leave) {
		}
	}

	void Monitor::stateRun(Phase_t p) {
		if (p == Enter) {
			m_io->clearDisplay();
		}
		if (p == Update) {
			m_core->exec();
			uint8_t key = getKey();
			if (key == KEY_CMD) {
				stateGoto(&Monitor::stateCommand);
			}
		}
		if (p == Leave) {
		}
	}

	void Monitor::stateStep(Phase_t p) {
		if (p == Enter) {
			m_io->clearDisplay();
		}
		if (p == Update) {
			m_io->displayByte(1, m_core->peek(Core::ADDR_PC));
			m_io->displayByte(0, m_core->peek(Core::ADDR_A));
			m_io->setDP(0, m_core->peek(Core::ADDR_FLAGS) != 0);

			uint8_t key = getKey();
			if (key == KEY_ENTER) {
				m_core->exec();
			}
			if (key == KEY_CMD) {
				stateGoto(&Monitor::stateCommand);
			}
		}
		if (p == Leave) {
		}
	}

	void Monitor::stateSaveBegin(Phase_t p) {
		if (p == Update) {
			stateGoto(&Monitor::stateSaveLoadSlot, 'S');
		}
	}

	void Monitor::stateLoadBegin(Phase_t p) {
		if (p == Update) {
			stateGoto(&Monitor::stateSaveLoadSlot, 'L');
		}
	}

	void Monitor::stateSaveLoadSlot(Phase_t p) {
		if (p == Enter) {
			m_io->clearDisplay();
			if (stateParam() == 'S') {
				m_io->displayDigit(3, 5);
			} else {
				m_io->displayPattern(3, LET_L);
			}
			m_cursorPos = 0;
			m_io->displayDigit(0, m_saveLoadSlot);
		}
		if (p == Update) {
			updateCursor();
			uint8_t key = getKey();
			if (key < 4) {
				m_saveLoadSlot = key;
				m_io->displayDigit(0, m_saveLoadSlot);
			}
			if (key == KEY_ENTER) {
				stateGoto(&Monitor::stateDoSaveLoad, stateParam());
			}
			if (key == KEY_CMD) {
				stateGoto(&Monitor::stateCommand);
			}
		}
	}

	void Monitor::stateDoSaveLoad(Phase_t p) {
		uint8_t& addr = m_stateValue;
		if (p == Enter) {
			addr = 0;
		}
		if (p == Update) {
			m_io->displayByte(0, addr);

			if (stateParam() == 'S') {
				EEPROM.write(addr, m_core->peek(addr));
			} else {
				m_core->poke(addr, EEPROM.read(addr));
			}
			delay(2);
			if (addr == 0xff) {
				stateGoto(&Monitor::stateCommand);
			} else {
				addr++;
			}
		}
	}

	void Monitor::stateSerialTrace(Phase_t p) {
		if (p == Enter) {
		}
		if (p == Update) {
			stateGoto(&Monitor::stateCommand);
		}
		if (p == Leave) {
		}
	}

	void printHex(uint8_t val) {
		Serial.print(val >> 4, HEX);
		Serial.print(val & 0xf, HEX);
	}

	void Monitor::stateSerialDump(Phase_t p) {
		uint8_t& addr = m_stateValue;
		if (p == Enter) {
			m_io->clearDisplay();
			addr = 0;
		}
		if (p == Update) {
			m_io->displayByte(0, addr);
			if ((addr & 0x0f) == 0) {
				Serial.println("");
				printHex(addr);
				Serial.print(": ");
			}

			printHex(m_core->peek(addr));
			Serial.print(' ');

			if (addr == 0xff) {
				Serial.println("");
				stateGoto(&Monitor::stateCommand);
			} else {
				addr++;
			}
		}
		if (p == Leave) {
		}
	}

	void Monitor::stateBatteryVoltage(Phase_t p) {
		if (p == Enter) {
			m_io->clearDisplay();
			m_stateTimer = TimeOutms(1);
		}
		if (p == Update) {
			if (m_stateTimer.hasTimedOut()) {
				m_stateTimer = TimeOutms(1000);

				int vcc = getVcc();
				m_io->displayDigit(0, vcc % 10);
				m_io->displayDigit(1, (vcc / 10) % 10);
				m_io->displayDigit(2, (vcc / 100) % 10, true);
				m_io->displayDigit(3, (vcc / 1000) % 10);
			}

			uint8_t key = getKey();
			if (key == KEY_CMD) {
				stateGoto(&Monitor::stateCommand);
			}
		}
		if (p == Leave) {
		}
	}
}
