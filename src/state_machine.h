#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

template <typename objType>
struct StateMachine {
  public:
	enum Phase_t { None, Enter, Update, Leave };

	StateMachine(objType* objPtr) : m_currentState(0), m_objPtr(objPtr) {
	}

	typedef void (objType::*stateFunction_t)(Phase_t phase);

	void stateUpdate() {
		if (m_currentState) {
			(m_objPtr->*m_currentState)(Update);
		}
	}

	void stateGoto(stateFunction_t state) {
		if (m_currentState) {
			(m_objPtr->*m_currentState)(Leave);
		}

		m_currentState = state;

		if (m_currentState) {
			(m_objPtr->*m_currentState)(Enter);
		}
	}

  private:
	stateFunction_t m_currentState;
	objType* m_objPtr;
};

#endif /* STATE_MACHINE_H */
