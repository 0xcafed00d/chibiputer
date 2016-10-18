#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

template <typename objType>
struct StateMachine {
  public:
	enum Phase_t { None, Enter, Update, Leave };

	StateMachine(objType* objPtr)
	    : m_currentState(0), m_gotoState(0), m_objPtr(objPtr), m_inUpdate(false) {
	}

	typedef void (objType::*stateFunction_t)(Phase_t phase);

	void stateUpdate() {
		if (m_currentState) {
			m_inUpdate = true;
			(m_objPtr->*m_currentState)(Update);
			m_inUpdate = false;

			if (m_gotoState) {
				stateGoto(m_gotoState);
				m_gotoState = 0;
			}
		}
	}

	void stateGoto(stateFunction_t state) {
		if (m_inUpdate) {
			m_gotoState = state;
			return;
		}

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
	stateFunction_t m_gotoState;
	objType* m_objPtr;
	bool m_inUpdate;
};

#endif /* STATE_MACHINE_H */
