#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

template <typename objType>
struct StateMachine {
  public:
	StateMachine(objType* objPtr) : m_objPtr(objPtr) {
	}

	typedef void (objType::*stateFunction_t)();

	void stateUpdate() {
		if (m_currentState) {
			(m_objPtr->*m_currentState)();
		}
	}

	bool enter() const {
		return m_stateEnter;
	}

	bool leave() const {
		return m_stateLeave;
	}

	void stateGoto(stateFunction_t state) {
		if (m_currentState) {
			m_stateLeave = true;
			(m_objPtr->*m_currentState)();
			m_stateLeave = false;
		}

		m_currentState = state;

		if (m_currentState) {
			m_stateEnter = true;
			(m_objPtr->*m_currentState)();
			m_stateEnter = false;
		}
	}

  private:
	stateFunction_t m_currentState;
	bool m_stateEnter;
	bool m_stateLeave;
	objType* m_objPtr;
};

#endif /* STATE_MACHINE_H */
