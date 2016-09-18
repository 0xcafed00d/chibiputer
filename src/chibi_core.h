#ifndef CHIBI_CORE_H
#define CHIBI_CORE_H

#include <Arduino.h>

/*
    MEMORY MAP:
    +-------------------------------+
    | 00:BANK0 REGISTERS            |
    +-------------------------------+
    | 0F:PROGRAM START              |
    |								|
    +-------------------------------+
    | FF:TOP OF STACK               |
    +-------------------------------+

    ON RUN:
    BANK0 = {0}
    PC = 0F
    SP = FF
    BANK0[8-F] = {0 ... 7}

    OPCODE  INSTR   DESC
    0x0?	LOADC	LOAD CONST 		BANK0[?] = [PC+1]; PC += 2
    0x1?	LOADA	LOAD A			A = BANK0[?]; FLAG = (A == 0); PC++
    0x2?	STORA	STORE A			BANK0[?] = A; FLAG = (A == 0); PC++
    0x3?	INC						[BANK0[?]]++; FLAG = OVERFLOW; PC++
    0x4?	DEC						[BANK0[?]]--; FLAG = UNDERFLOW; PC++
    0x5?	ADD						A += [BANK0[?]]; FLAG = OVERFLOW; PC++
    0x6?	SUB						A -= [BANK0[?]]; FLAG = UNDERFLOW; PC++
    0x7?	AND						A &= [BANK0[?]]; FLAG = (A == 0); PC++
    0x8?	OR						A |= [BANK0[?]]; FLAG = (A == 0); PC++
    0x9?	NOT						A = ![BANK0[?]]; FLAG = (A == 0); PC++
    0xA?	SETA   	SET A			A = [BANK0[?]]; FLAG = (A == 0); PC++
    0xB?	PUTA	PUT A			[BANK0[?]] = A; FLAG = (A == 0); PC++
    0xC?	??
    0xD?	INCT	INC IF TRUE		IF (FLAG) PC++; PC++
    0xE?	INCF    INC IF FALSE	IF (!FLAG) PC++; PC++
    0xF?	CALL 	Call builtin
*/

namespace Chibi {
	class Core {
	  public:
		static const uint8_t ADDR_A = 0;
		static const uint8_t ADDR_FLAGS = 1;
		static const uint8_t ADDR_PC = 2;
		static const uint8_t ADDR_SP = 3;
		static const uint8_t ADDR_W = 4;
		static const uint8_t ADDR_X = 5;
		static const uint8_t ADDR_Y = 6;
		static const uint8_t ADDR_Z = 7;

		static const uint8_t OPCODE_LOADC = 0x0;
		static const uint8_t OPCODE_LOADA = 0x1;
		static const uint8_t OPCODE_STORA = 0x2;
		static const uint8_t OPCODE_INC = 0x3;
		static const uint8_t OPCODE_DEC = 0x4;
		static const uint8_t OPCODE_ADD = 0x5;
		static const uint8_t OPCODE_SUB = 0x6;
		static const uint8_t OPCODE_AND = 0x7;
		static const uint8_t OPCODE_OR = 0x8;
		static const uint8_t OPCODE_NOT = 0x9;
		static const uint8_t OPCODE_SETA = 0xa;
		static const uint8_t OPCODE_PUTA = 0xb;
		static const uint8_t OPCODE_NONE = 0xc;
		static const uint8_t OPCODE_INCT = 0xd;
		static const uint8_t OPCODE_INCF = 0xe;
		static const uint8_t OPCODE_CALL = 0xf;

		void reset(bool clearMem = false);
		void exec();
		uint8_t peek(uint8_t addr);
		void poke(uint8_t addr, uint8_t);

	  private:
		uint8_t m_ram[256];
	};
}

#endif /* CHIBI_CORE_H */
