
#include "chibi_core.cpp"

#include <iostream>
#include <assert.h>

using namespace Chibi;

bool equals(const uint8_t& got, const uint8_t& expected) {
	if (got != expected) {
		std::cerr << std::hex << "expected: " << (uint16_t)expected << "  got: " << (uint16_t)got
		          << "  : ";
		return false;
	}
	return true;
}

void writeOpcode(Core& core, uint8_t addr, uint8_t opcode, uint8_t index) {
	core.poke(addr, (opcode << 4) | (index & 0xf));
}

void test_chibi_core() {
	std::cout << "Start Chibi Core Test 1...." << std::endl;

	Core core;

	core.reset(true);

	assert(core.peek(core.ADDR_PC) == 0x10);
	assert(core.peek(core.ADDR_SP) == 0xFF);
	assert(core.peek(core.ADDR_FLAGS) == 0);

	uint8_t addr = 0x10;
	// LOADC A
	// DB 0xc5
	// XOR XA
	// INC XA
	// DEC XA
	// DEC XA

	writeOpcode(core, addr++, core.OPCODE_LOADC, core.ADDR_A);
	core.poke(addr++, 0xc5);
	writeOpcode(core, addr++, core.OPCODE_XOR, core.X_ADDR_A);
	writeOpcode(core, addr++, core.OPCODE_INC, core.X_ADDR_A);
	writeOpcode(core, addr++, core.OPCODE_DEC, core.X_ADDR_A);
	writeOpcode(core, addr++, core.OPCODE_DEC, core.X_ADDR_A);

	assert(equals(core.peek(0x10), 0x02));

	core.exec();
	assert(equals(core.peek(core.ADDR_PC), 0x12));
	assert(equals(core.peek(core.ADDR_A), 0xc5));
	assert(equals(core.peek(core.ADDR_FLAGS), 0));

	core.exec();
	assert(equals(core.peek(core.ADDR_A), 0));
	assert(equals(core.peek(core.ADDR_FLAGS), 0));

	core.exec();
	assert(equals(core.peek(core.ADDR_A), 1));
	assert(equals(core.peek(core.ADDR_FLAGS), 0));

	core.exec();
	assert(equals(core.peek(core.ADDR_A), 0));
	assert(equals(core.peek(core.ADDR_FLAGS), 0));

	core.exec();
	assert(equals(core.peek(core.ADDR_A), 0xff));
	assert(equals(core.peek(core.ADDR_FLAGS), 1));

	std::cout << "Done Chibi Core Test 1" << std::endl;
}

int main(int argc, char** argv) {
	test_chibi_core();
}
