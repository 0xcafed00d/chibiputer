#ifndef CHIBI_MONITOR_H
#define CHIBI_MONITOR_H

#include <Arduino.h>
#include "chibi_io.h"
#include "chibi_core.h"

namespace Chibi {

	class Monitor : public KeyReceiver {
	  public:
		void init(IO* io, Core* core);
		void update();

		virtual void onKey(uint8_t scancode, uint8_t value, bool pressed);

	  private:
		Core* m_core;
		IO* m_io;
	};
}

#endif /* CHIBI_MONITOR_H */
