
#include "chibi_core.cpp"
#include "state_machine.h"

#include <iostream>
#include <assert.h>
#include <string>

using namespace Chibi;

bool equals(uint8_t got, uint8_t expected) {
	if (got != expected) {
		std::cerr << std::hex << "expected: " << (uint16_t)expected << "  got: " << (uint16_t)got
		          << "  : ";
		return false;
	}
	return true;
}

bool equals(std::string got, std::string expected) {
	if (got != expected) {
		std::cerr << std::hex << "expected: " << expected << "  got: " << got << "  : ";
		return false;
	}
	return true;
}

void writeOpcode(Core& core, uint8_t addr, uint8_t opcode, uint8_t index) {
	core.poke(addr, (opcode << 4) | (index & 0xf));
}

struct TestStateMachine : public StateMachine<TestStateMachine> {
	TestStateMachine() : StateMachine(this) {
	}

	void state1(Phase_t p) {
		if (p == Enter) {
			std::cout << " state1::enter" << std::endl;
			m_trace += "s1e ";
		}
		if (p == Update) {
			std::cout << " state1::update" << std::endl;
			m_trace += "s1u ";
		}
		if (p == Leave) {
			std::cout << " state1::leave" << std::endl;
			m_trace += "s1l ";
		}
	}

	void state2(Phase_t p) {
		if (p == Enter) {
			std::cout << " state2::enter" << std::endl;
			m_trace += "s2e ";
		}
		if (p == Update) {
			std::cout << " state2::update" << std::endl;
			m_trace += "s2u ";
		}
		if (p == Leave) {
			std::cout << " state2::leave" << std::endl;
			m_trace += "s2l ";
		}
	}

	std::string m_trace;
};

void test_state_machine() {
	std::cout << "Start StateMachine Test" << std::endl;
	TestStateMachine t;
	t.stateUpdate();
	t.stateGoto(&TestStateMachine::state1);
	t.stateUpdate();
	t.stateGoto(&TestStateMachine::state2);
	t.stateUpdate();
	t.stateGoto(&TestStateMachine::state1);
	t.stateUpdate();
	t.stateGoto(0);
	t.stateUpdate();

	assert(equals(t.m_trace, "s1e s1u s1l s2e s2u s2l s1e s1u s1l "));

	std::cout << "End StateMachine Test" << std::endl;
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
	test_state_machine();
}
