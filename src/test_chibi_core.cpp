
#include "chibi_core.cpp"

#include <iostream>
#include <assert.h>

using namespace Chibi;

void writeOpcode(Core& core, uint8_t addr, uint8_t opcode, uint8_t index) {
	core.poke(addr, (opcode << 4) | (index & 0xf));
}

void test_chibi_core() {
	std::cout << "Start Chibi Core Test...." << std::endl;

	Core core;

	core.reset(true);

	assert(core.peek(core.ADDR_PC) == 0x10);
	assert(core.peek(core.ADDR_SP) == 0xFF);
	assert(core.peek(core.ADDR_FLAGS) == 0);

	std::cout << "Done Chibi Core Test" << std::endl;
}

int main(int argc, char** argv) {
	test_chibi_core();
}
