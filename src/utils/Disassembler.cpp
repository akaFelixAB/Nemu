#include "Disassembler.h"

#include <cstdint>

#include "../core/Bus.h"
#include "../core/RP2A03.h"

Disassembler::Disassembler(
    Bus* bus, const std::span<const RP2A03::INSTRUCTION, 256> lookup)
    : bus(bus), lookup(lookup) {}

uint8_t Disassembler::read(uint16_t addr) const noexcept {
    return bus->cpu_read(addr, true);
}

Disassembler::DisasmLine Disassembler::disassemble(uint16_t addr) const {
    DisasmLine line;
    line.addr = addr;
    line.opcode = read(addr);
    const auto& instr = lookup[line.opcode];
    line.mnemonic = instr.debug_mnemonic;
    line.addrmode = instr.debug_addrmode;
    line.len = instr_len(line.addrmode);
    for (size_t i = 0; i < line.len; i++) {
        line.bytes[i] = read(addr + i);
    }
    line.next = static_cast<uint16_t>(addr + line.len);

    line.text = format(line);  // Format the disassembly text (e.g. "LDA #$01")

    return line;
}

std::vector<Disassembler::DisasmLine> Disassembler::disassemble(
    uint16_t start, size_t count) const {
    std::vector<DisasmLine> lines;
    lines.reserve(count);

    uint16_t addr = start;
    for (size_t i = 0; i < count; i++) {
        auto line = disassemble(addr);
        addr = line.next;  // Move to the next instruction address
        lines.push_back(std::move(line));
    }
    return lines;
}

std::string Disassembler::get_memory_layout(uint16_t start, size_t count) const {
    std::string out;
    // Each byte will take up to 3 chars (e.g. "FF ") and we want a space
    // between bytes, so reserve 3 chars per byte for efficiency
    out.reserve(count * 3);

    for (size_t i = 0; i < count; i++) {
        uint8_t byte = read(static_cast<uint16_t>(start + i));
        append_hex_8(out, byte);
        out.push_back(' ');
    }
    return out;
}

std::string Disassembler::format(const DisasmLine& line) const {
    using AM = RP2A03::DEBUG_ADDRESSING_MODE;

    std::string out;
    out.reserve(32);

    // Append mnemonic
    const auto mi = static_cast<size_t>(line.mnemonic);
    if (mi < mnemonics.size())
        out.append(mnemonics[mi]);
    else
        out.append("???");

    // Append operand
    std::string operand;
    operand.reserve(20);

    const uint8_t op1 = line.bytes[1];
    const uint8_t op2 = line.bytes[2];

    switch (line.addrmode) {
        case AM::IMP:
            // no operand
            break;

        case AM::ACC:
            operand.push_back('A');
            break;

        case AM::IMM:
            // e.g. "LDA #$01"
            operand.push_back('#');
            append_dollar_hex_8(operand, op1);
            break;

        case AM::ZP0:
            // e.g. "LDA $02"
            append_dollar_hex_8(operand, op1);
            break;

        case AM::ZPX:
            // e.g. "LDA $02,X"
            append_dollar_hex_8(operand, op1);
            operand.append(",X");
            break;

        case AM::ZPY:
            // e.g. "LDA $02,Y"
            append_dollar_hex_8(operand, op1);
            operand.append(",Y");
            break;

        case AM::ABS: {
            // e.g. "JMP $1234"
            const uint16_t a =
                static_cast<uint16_t>(op1) | (static_cast<uint16_t>(op2) << 8);
            append_dollar_hex_16(operand, a);
            break;
        }

        case AM::ABX: {
            // e.g. "LDA $1234,X"
            const uint16_t a =
                static_cast<uint16_t>(op1) | (static_cast<uint16_t>(op2) << 8);
            append_dollar_hex_16(operand, a);
            operand.append(",X");
            break;
        }

        case AM::ABY: {
            // e.g. "LDA $1234,Y"
            const uint16_t a =
                static_cast<uint16_t>(op1) | (static_cast<uint16_t>(op2) << 8);
            append_dollar_hex_16(operand, a);
            operand.append(",Y");
            break;
        }

        case AM::IND: {
            // e.g. "JMP ($1234)"
            const uint16_t a =
                static_cast<uint16_t>(op1) | (static_cast<uint16_t>(op2) << 8);
            operand.push_back('(');
            append_dollar_hex_16(operand, a);
            operand.push_back(')');
            break;
        }

        case AM::IZX:
            // e.g. "LDA ($20,X)"
            operand.push_back('(');
            append_dollar_hex_8(operand, op1);
            operand.append(",X)");
            break;

        case AM::IZY:
            // e.g. "LDA ($20),Y"
            operand.push_back('(');
            append_dollar_hex_8(operand, op1);
            operand.append("),Y");
            break;

        case AM::REL: {
            // e.g. "BEQ $1234" (branch target)
            // The operand is a signed offset from the next instruction address
            // (line.next) branch target = line.next + signed offset
            const int8_t rel = static_cast<int8_t>(op1);
            const uint16_t target =
                static_cast<uint16_t>(static_cast<int16_t>(line.next) + rel);
            append_dollar_hex_16(operand, target);
            break;
        }

        default:
            // unknown
            break;
    }

    if (!operand.empty()) {
        out.push_back(' ');
        out.append(operand);
    }

    return out;
}