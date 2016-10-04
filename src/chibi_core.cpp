#include "chibi_core.h"
#include <string.h>

namespace Chibi {

	void Core::reset(bool clearMem) {
		if (clearMem) {
			memset(m_ram, 0, 256);
		} else {
			memset(m_ram, 0, 16);
		}
		m_ram[ADDR_PC] = 0x10;
		m_ram[ADDR_SP] = 0xff;

		for (int n = 0; n < 8; n++)
			m_ram[n + 8] = n;
	}

	void Core::exec() {
		uint8_t& pc = m_ram[ADDR_PC];
		uint8_t& sp = m_ram[ADDR_SP];
		uint8_t opcode = m_ram[pc] >> 4;
		uint8_t index = m_ram[pc] & 0x0f;

		pc++;

		switch (opcode) {
			case OPCODE_LOADC:
				m_ram[index] = m_ram[pc];
				pc++;
				break;
			case OPCODE_PUSH:
				m_ram[sp] = flagZ(m_ram[m_ram[index]]);
				sp--;
				break;
			case OPCODE_POP:
				sp++;
				m_ram[m_ram[index]] = flagZ(m_ram[sp]);
				break;
			case OPCODE_INC:
				m_ram[m_ram[index]] = flagOV(uint16_t(m_ram[m_ram[index]]) + 1);
				break;
			case OPCODE_DEC:
				m_ram[m_ram[index]] = flagOV(uint16_t(m_ram[m_ram[index]]) - 1);
				break;
			case OPCODE_ADD:
				m_ram[ADDR_A] = flagOV(uint16_t(m_ram[m_ram[index]]) + uint16_t(m_ram[ADDR_A]));
				break;
			case OPCODE_SUB:
				m_ram[ADDR_A] = flagOV(uint16_t(m_ram[m_ram[index]]) - uint16_t(m_ram[ADDR_A]));
				break;
			case OPCODE_AND:
				m_ram[ADDR_A] = flagOV(uint16_t(m_ram[m_ram[index]]) & uint16_t(m_ram[ADDR_A]));
				break;
			case OPCODE_OR:
				m_ram[ADDR_A] = flagOV(uint16_t(m_ram[m_ram[index]]) | uint16_t(m_ram[ADDR_A]));
				break;
			case OPCODE_NOT:
				m_ram[m_ram[index]] = flagZ(~m_ram[m_ram[index]]);
				break;
			case OPCODE_SETA:
				m_ram[ADDR_A] = flagZ(m_ram[m_ram[index]]);
				break;
			case OPCODE_PUTA:
				m_ram[m_ram[index]] = flagZ(m_ram[ADDR_A]);
				break;
			case OPCODE_CALL:
				break;
		}
	}

	uint8_t Core::flagOV(uint16_t v) {
		m_ram[ADDR_FLAGS] = uint8_t((v & 0xff00) != 0);
		return (uint8_t)v;
	}

	uint8_t Core::flagZ(uint8_t v) {
		m_ram[ADDR_FLAGS] = uint8_t(v == 0);
		return v;
	}

	uint8_t Core::peek(uint8_t addr) {
		return m_ram[addr];
	}

	void Core::poke(uint8_t addr, uint8_t val) {
		m_ram[addr] = val;
	}
}