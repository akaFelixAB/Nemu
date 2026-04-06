#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "../core/Bus.h"
#include "../core/RP2A03.h"

class Disassembler {
public:
    explicit Disassembler(
        Bus* bus, const std::span<const RP2A03::INSTRUCTION, 256> lookup);
    ~Disassembler() = default;

    struct DisasmLine {
        uint16_t addr{0x0000};  // instruction address
        uint8_t opcode{0x00};   // opcode byte
        uint8_t len{0};         // instruction length in bytes
        std::array<uint8_t, 3> bytes{{0, 0, 0}};   // Original bytes of the instruction (up to 3 bytes)

        uint16_t next{0x0000};  // addr + len

        std::string text;       // Formatted disassembly text (e.g. "LDA #$01")

        RP2A03::DEBUG_MNEMONIC mnemonic{};
        RP2A03::DEBUG_ADDRESSING_MODE addrmode{};
    };

    static constexpr uint8_t instrLen(
        RP2A03::DEBUG_ADDRESSING_MODE m) noexcept {
        using M = RP2A03::DEBUG_ADDRESSING_MODE;
        switch (m) {
            case M::IMP:
            case M::ACC:
                return 1; // e.g. "CLC" or "ASL A"

            case M::IMM:
            case M::ZP0:
            case M::ZPX:
            case M::ZPY:
            case M::REL:
            case M::IZX:
            case M::IZY:
                return 2; // e.g. "LDA #$01" or "STA $02,X"

            case M::ABS:
            case M::ABX:
            case M::ABY:
            case M::IND:
                return 3;  // e.g. "JMP $1234" or "LDA $1234,Y"

            default:
                return 1; // Unknown/invalid addressing mode
                // Treat as opcode-only to ensure forward progress
        }
    }

public:
    std::vector<DisasmLine> disassemble(uint16_t start, size_t count) const;
    DisasmLine disassemble(uint16_t addr) const;

    // Format the disassembly text based on the instruction and its addressing mode
    std::string format(const DisasmLine& line) const;

    // Helper functions to format hexadecimal values
    static inline void appendHex8(std::string& out, uint8_t v) {
        static constexpr char hex[] = "0123456789ABCDEF";
        out.push_back(hex[(v >> 4) & 0x0F]);
        out.push_back(hex[v & 0x0F]);
    }

    static inline void appendHex16(std::string& out, uint16_t v) {
        appendHex8(out, static_cast<uint8_t>((v >> 8) & 0xFF));
        appendHex8(out, static_cast<uint8_t>(v & 0xFF));
    }

    static inline void appendDollarHex8(std::string& out, uint8_t v) {
        out.push_back('$');
        appendHex8(out, v);
    }

    static inline void appendDollarHex16(std::string& out, uint16_t v) {
        out.push_back('$');
        appendHex16(out, v);
    }

private:
    Bus* bus;
    const std::span<const RP2A03::INSTRUCTION, 256> lookup;

    static constexpr std::array<std::string_view, static_cast<size_t>(RP2A03::DEBUG_MNEMONIC::COUNT)> 
    mnemonics {
        "ADC", "AND", "ASL", "BCC", "BCS", "BEQ", "BIT", "BMI", "BNE", "BPL",
        "BRK", "BVC", "BVS", "CLC", "CLD", "CLI", "CLV", "CMP", "CPX", "CPY",
        "DEC", "DEX", "DEY", "EOR", "INC", "INX", "INY", "JMP", "JSR", "LDA",
        "LDX", "LDY", "LSR", "NOP", "ORA", "PHA", "PHP", "PLA", "PLP", "ROL",
        "ROR", "RTI", "RTS", "SBC", "SEC", "SED", "SEI", "STA", "STX", "STY",
        "TAX", "TAY", "TSX", "TXA", "TXS", "TYA", "XXX"
    };

    uint8_t read(uint16_t addr) const noexcept;
};
