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
		void updateCursor();

		void stateWaitCmd();
		void stateAddressInput();
		void stateDataInput();

		Core* m_core;
		IO* m_io;
		int m_cursorPos;
		TimeOutms m_cursorTime;
	};
}

#endif /* CHIBI_MONITOR_H */
