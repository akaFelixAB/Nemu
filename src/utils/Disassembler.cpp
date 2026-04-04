#include "Disassembler.h"

#include "../core/Bus.h"
#include "../core/RP2A03.h"

#include <cstdint>

Disassembler::Disassembler(Bus* bus,
    const std::array<RP2A03::INSTRUCTION, 256>& lookup)
    : bus(bus), lookup(lookup) {
}

uint8_t Disassembler::read(uint16_t addr) const noexcept {
    return bus->read(addr, true);
}

Disassembler::DisasmLine Disassembler::disassemble(uint16_t addr) const {
    DisasmLine line;
    line.addr = addr;
    line.opcode = read(addr);
    const auto& instr = lookup[line.opcode];
    line.nmemonic = instr.debug_nmemonic;
    line.mode = instr.debug_addrmode;
    line.len = instrLen(line.mode);
    for (size_t i = 0; i < line.len; i++) {
        line.bytes[i] = read(addr + i);
    }
    return line;
}

std::vector<Disassembler::DisasmLine> Disassembler::disassemble(uint16_t start, size_t count) const {
    std::vector<DisasmLine> lines;
    for (size_t i = 0; i < count; i++) {
        lines.push_back(disassemble(start + i));
    }
    return lines;
}