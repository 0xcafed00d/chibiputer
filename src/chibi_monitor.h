#ifndef CHIBI_MONITOR_H
#define CHIBI_MONITOR_H

#include <Arduino.h>
#include "chibi_io.h"
#include "chibi_core.h"
#include "timeout.h"

template <typename objType>
struct StateMachine {
  protected:
	typedef void (objType::*stateFunction_t)();
	objType* m_objPtr;

	void stateUpdate() {
		if (m_currentState != NULL) {
			(m_objPtr->*m_currentState)();
		}
	}

	void stateGoto(stateFunction_t state) {
		if (m_currentState != NULL) {
			m_stateLeave = true;
			(m_objPtr->*m_currentState)();
			m_stateLeave = false;
		}
		m_currentState = state;
		if (m_currentState != NULL) {
			m_stateEnter = true;
			(m_objPtr->*m_currentState)();
			m_stateEnter = false;
		}
	}

  private:
	stateFunction_t m_currentState;
	bool m_stateEnter;
	bool m_stateLeave;
};

namespace Chibi {

	class Monitor : public KeyReceiver, public StateMachine<Monitor> {
	  public:
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
