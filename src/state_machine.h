#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

template <typename objType>
struct StateMachine {
  public:
	StateMachine(objType* objPtr)
	    : m_currentState(0), m_objPtr(objPtr), m_stateEnter(false), m_stateLeave(false), m_stateUpdate(false) {
	}

	typedef void (objType::*stateFunction_t)();

	void stateUpdate() {
		if (m_currentState) {
			m_stateUpdate = true;
			(m_objPtr->*m_currentState)();
			m_stateUpdate = false;
		}
	}

	bool entering() const {
		return m_stateEnter;
	}

	bool leaving() const {
		return m_stateLeave;
	}

	bool updating() const {
		return m_stateUpdate;
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
	objType* m_objPtr;
	bool m_stateEnter;
	bool m_stateLeave;
	bool m_stateUpdate;
};

#endif /* STATE_MACHINE_H */
