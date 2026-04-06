// test_main.cpp
// Example unit test using Catch2 (if enabled)
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "../src/utils/Disassembler.h"
#include "../src/core/RP2A03.h"
#include "../src/core/Bus.h"

TEST_CASE("Basic test", "[utils]") {
    // Create a bus and CPU instance for testing
    Bus bus;
    RP2A03 cpu;

    // Connect the CPU to the bus
    cpu.ConnectBus(&bus);

    // Write some test data to the bus
    bus.write(0x0000, 0xA9); // LDA Immediate opcode
    bus.write(0x0001, 0x01); // Operand for LDA

    // Create a disassembler instance
    Disassembler disasm(&bus, cpu.getLookup());

    // Disassemble the instruction at address 0x0000
    auto line = disasm.disassemble(0x0000);

    // Check that the disassembly is correct
    REQUIRE(line.opcode == 0xA9);
    REQUIRE(line.mnemonic == RP2A03::DEBUG_MNEMONIC::LDA);
    REQUIRE(line.addrmode == RP2A03::DEBUG_ADDRESSING_MODE::IMM);
    REQUIRE(line.text == "LDA #$01");
}
