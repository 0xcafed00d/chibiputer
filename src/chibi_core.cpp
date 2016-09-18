#include <Arduino.h>
#include "chibi_core.h"

namespace Chibi {

	void Core::reset(bool clearMem) {
		if (clearMem) {
			memset(m_ram, 0, 256);
		} else {
			memset(m_ram, 0, 16);
		}
		m_ram[ADDR_PC] = 0x0f;
		m_ram[ADDR_SP] = 0xff;

		for (int n = 0; n < 8; n++)
			m_ram[n + 8] = n;
	}

	void Core::exec() {
		uint8_t& pc = m_ram[ADDR_PC];
		uint8_t opcode = m_ram[pc] >> 4;
		uint8_t index = m_ram[pc] & 0x0f;

		switch (opcode) {
			case OPCODE_LOADC:
				m_ram[index] = m_ram[pc + 1];
				pc++;
				break;
			case OPCODE_LOADA:
				m_ram[ADDR_A] = m_ram[index];
				break;
			case OPCODE_STORA:
				m_ram[index] = m_ram[ADDR_A];
				break;
			case OPCODE_INC:
				break;
			case OPCODE_DEC:
				break;
			case OPCODE_ADD:
				break;
			case OPCODE_SUB:
				break;
			case OPCODE_AND:
				break;
			case OPCODE_OR:
				break;
			case OPCODE_NOT:
				break;
			case OPCODE_SETA:
				break;
			case OPCODE_PUTA:
				break;
			case OPCODE_NONE:
				break;
			case OPCODE_INCT:
				break;
			case OPCODE_INCF:
				break;
			case OPCODE_CALL:
				break;
		}
		pc++;
	}

	uint8_t Core::peek(uint8_t addr) {
		return m_ram[addr];
	}

	void Core::poke(uint8_t addr, uint8_t val) {
		m_ram[addr] = val;
	}
}