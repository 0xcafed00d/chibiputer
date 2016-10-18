#ifndef CHIBI_MONITOR_H
#define CHIBI_MONITOR_H

#include <Arduino.h>
#include "chibi_io.h"
#include "chibi_core.h"
#include "timeout.h"
#include "state_machine.h"

namespace Chibi {

	class Monitor : public KeyReceiver, public StateMachine<Monitor> {
	  public:
		Monitor();
		void init(IO* io, Core* core);
		void update();
		virtual void onKey(uint8_t scancode, uint8_t value, bool pressed);

	  private:
		static const uint8_t KEY_CMD = 0x80;
		static const uint8_t KEY_UP = 0x81;
		static const uint8_t KEY_DOWN = 0x82;
		static const uint8_t KEY_ENTER = 0x83;

		void updateCursor();

		uint8_t getKey();
		void handleCommonKeys(uint8_t key);

		void stateCommand(Phase_t p);
		void stateAddressInput(Phase_t p);
		void stateDataInput(Phase_t p);

		Core* m_core;
		IO* m_io;
		int m_cursorPos;
		TimeOutms m_cursorTime;
		bool m_cursorBlink;
		uint8_t m_keyPressed;
		uint8_t m_currentAddr;
		static StateMachine::stateFunction_t m_commands[16];
	};
}

#endif /* CHIBI_MONITOR_H */
