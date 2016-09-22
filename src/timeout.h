#ifndef TIMEOUT_H
#define TIMEOUT_H

#include <Arduino.h>

class TimeOutus {
	uint32_t m_start;
	uint32_t m_period;

  public:
	TimeOutus(uint32_t timeoutus = 0) {
		m_start = (uint32_t)micros();
		m_period = timeoutus;
	}

	void reset() {
		m_start = (uint32_t)micros();
	}

	bool hasTimedOut() const {
		return (micros() - m_start) > m_period;
	}
};

class TimeOutms {
	uint32_t m_start;
	uint32_t m_period;

  public:
	TimeOutms(uint32_t timeoutms = 0) {
		m_start = (uint32_t)millis();
		m_period = timeoutms;
	}

	void reset() {
		m_start = (uint32_t)millis();
	}

	bool hasTimedOut() const {
		return (millis() - m_start) > m_period;
	}
};

#endif /* TIMEOUT_H */
