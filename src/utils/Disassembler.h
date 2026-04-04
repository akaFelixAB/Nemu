#pragma once

#include "../core/RP2A03.h"

#include <array>
#include <cstdint>
#include <string>
#include <vector>
#include <functional>

class Disassembler {
public:
    using ReadFn = std::function<uint8_t(uint16_t)>;

    explicit Disassembler(Bus* bus,
                        const std::array<RP2A03::INSTRUCTION, 256>& lookup);
    ~Disassembler() = default;

    struct DisasmLine {
        uint16_t    addr{0x0000};             // instruction address
        uint8_t     opcode{0x00};             // opcode byte
        uint8_t     len{0};                   // instruction length in bytes
        std::array<uint8_t, 3>bytes{{0, 0, 0}}; // Original bytes of the instruction (up to 3 bytes)
        std::string_view text{""};            // e.x. "LDA #$01"
        
        uint16_t    next{0x0000};             // addr + len

        RP2A03::DEBUG_NMEMONIC nmemonic{};
        RP2A03::DEBUG_ADDRESSING_MODE mode{};
    };

    static constexpr uint8_t instrLen(RP2A03::DEBUG_ADDRESSING_MODE m) noexcept {
        using M = RP2A03::DEBUG_ADDRESSING_MODE;
        switch (m) {
            case M::IMP:
            case M::ACC:
                return 1;
            case M::IMM:
            case M::ZP0:
            case M::ZPX:
            case M::ZPY:
            case M::REL:
            case M::IZX:
            case M::IZY:
                return 2;
            case M::ABS:
            case M::ABX:
            case M::ABY:
            case M::IND:
                return 3;
        }
        return 1;
    }

public:
    std::vector<DisasmLine> disassemble(uint16_t start, size_t count) const;
    DisasmLine disassemble(uint16_t addr) const;

private:
    Bus* bus;
    const std::array<RP2A03::INSTRUCTION, 256>& lookup;

    static constexpr std::array<std::string_view, 57> nmemonics {
        "ADC", "AND", "ASL", 
        "BCC", "BCS", "BEQ", "BIT", "BMI", "BNE", "BPL", "BRK", "BVC", "BVS", 
        "CLC", "CLD", "CLI", "CLV", "CMP", "CPX", "CPY", 
        "DEC", "DEX", "DEY", 
        "EOR", 
        "INC", "INX", "INY", 
        "JMP", "JSR", 
        "LDA", "LDX", "LDY", "LSR", 
        "NOP", 
        "ORA", 
        "PHA", "PHP", "PLA", "PLP", 
        "ROL", "ROR", "RTI", "RTS", 
        "SBC", "SEC", "SED", "SEI", "STA", "STX", "STY", 
        "TAX", "TAY", "TSX", "TXA", "TXS", "TYA", 
        "XXX"
    };

    uint8_t read(uint16_t addr) const noexcept;
};
