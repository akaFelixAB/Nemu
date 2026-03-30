#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Bus;

class RP2A03 {
public:
    explicit RP2A03();
    ~RP2A03() = default;

public:
    enum FLAGS6502 {
        // The NES uses a variant of the 6502 (the 2A03) which has no
        // hardware support for decimal mode, so we won't emulate that
        // flag here (it would be a useless flag in the context of the NES)
        C = (1 << 0), // Carry Bit
        Z = (1 << 1), // Zero
        I = (1 << 2), // Disable Interrupts
        D = (1 << 3), // Decimal Mode (unused in this implementation)
        B = (1 << 4), // Break
        U = (1 << 5), // Unused
        V = (1 << 6), // Overflow
        N = (1 << 7), // Negative
    };

    uint8_t  x = 0x00; // X Register
    uint8_t  y = 0x00; // Y Register
    uint8_t  a = 0x00; // Accumulator Register
    uint8_t  sp = 0x00; // Stack Pointer (points to location on bus)
    uint16_t pc = 0x0000; // Program Counter
    uint8_t  status = 0x00; // Status Register

    constexpr static uint16_t STACK_BASE = 0x0100; // Stack starts at 0x0100 and grows downwards
    constexpr static uint16_t RESET_VECTOR = 0xFFFC; // Reset vector is located at 0xFFFC and 0xFFFD
    constexpr static uint16_t NMI_VECTOR = 0xFFFA; // Non-maskable interrupt vector is located at 0xFFFA and 0xFFFB
    constexpr static uint16_t IRQ_VECTOR = 0xFFFE; // Interrupt request vector is located at 0xFFFE and 0xFFFF

    void ConnectBus(Bus* n) { bus = n; }

    // Addressing modes
    uint8_t IMP() noexcept; // Implied
    uint8_t IMM() noexcept; // Immediate
    uint8_t ZP0() noexcept; // Zero Page
    uint8_t ZPX() noexcept; // Zero Page,X
    uint8_t ZPY() noexcept; // Zero Page,Y
    uint8_t REL() noexcept; // Relative
    uint8_t ABS() noexcept; // Absolute
    uint8_t ABX() noexcept; // Absolute,X
    uint8_t ABY() noexcept; // Absolute,Y
    uint8_t IND() noexcept; // Indirect
    uint8_t IZX() noexcept; // Indirect,X
    uint8_t IZY() noexcept; // Indirect,Y

    // Opcodes
    uint8_t ADC() noexcept;  uint8_t AND() noexcept;
    uint8_t ASL() noexcept;  uint8_t BCC() noexcept;
    uint8_t BCS() noexcept;  uint8_t BEQ() noexcept;
    uint8_t BIT() noexcept;  uint8_t BMI() noexcept;
    uint8_t BNE() noexcept;  uint8_t BPL() noexcept;
    uint8_t BRK() noexcept;  uint8_t BVC() noexcept;
    uint8_t BVS() noexcept;  uint8_t CLC() noexcept;
    uint8_t CLD() noexcept;  uint8_t CLI() noexcept;
    uint8_t CLV() noexcept;  uint8_t CMP() noexcept;
    uint8_t CPX() noexcept;  uint8_t CPY() noexcept;
    uint8_t DEC() noexcept;  uint8_t DEX() noexcept;
    uint8_t DEY() noexcept;  uint8_t EOR() noexcept;
    uint8_t INC() noexcept;  uint8_t INX() noexcept;
    uint8_t INY() noexcept;  uint8_t JMP() noexcept;
    uint8_t JSR() noexcept;  uint8_t LDA() noexcept;
    uint8_t LDX() noexcept;  uint8_t LDY() noexcept;
    uint8_t LSR() noexcept;  uint8_t NOP() noexcept;
    uint8_t ORA() noexcept;  uint8_t PHA() noexcept;
    uint8_t PHP() noexcept;  uint8_t PLA() noexcept;
    uint8_t PLP() noexcept;  uint8_t ROL() noexcept;
    uint8_t ROR() noexcept;  uint8_t RTI() noexcept;
    uint8_t RTS() noexcept;  uint8_t SBC() noexcept;
    uint8_t SEC() noexcept;  uint8_t SED() noexcept;
    uint8_t SEI() noexcept;  uint8_t STA() noexcept;
    uint8_t STX() noexcept;  uint8_t STY() noexcept;
    uint8_t TAX() noexcept;  uint8_t TAY() noexcept;
    uint8_t TSX() noexcept;  uint8_t TXA() noexcept;
    uint8_t TXS() noexcept;  uint8_t TYA() noexcept;

    uint8_t XXX() noexcept; // Illegal opcode

    void    clock(); // Perform one clock cycle
    void    reset(); // Reset the CPU to initial state
    void    irq();   // Interrupt request
    void    nmi();   // Non-maskable interrupt request

    uint8_t     fetch();           // Fetch data from memory (based on addressing mode)
    uint8_t     fetched = 0x00;    // Represents the working input value to the ALU
    uint16_t    temp    = 0x0000;  // Convenience variable used everywhere

    uint16_t    addr_abs = 0x0000; // Absolute address after all calculations
    uint16_t    addr_rel = 0x0000; // Relative address for branch instructions
    uint8_t     opcode   = 0x00;   // Current opcode
    uint8_t     cycles   = 0;      // Cycles remaining for current instruction

private:
    Bus* bus = nullptr;

    // Private methods
    inline uint8_t  read(uint16_t addr) noexcept;
    inline void     write(uint16_t addr, uint8_t data) noexcept;

    // get flag & set flag (just for convenience)
    inline uint8_t  GetFlag(FLAGS6502 flag) const noexcept;
    inline void     SetFlag(FLAGS6502 flag, bool value) noexcept;

    struct INSTRUCTION {
        std::string name;
        uint8_t (RP2A03::*operate)(void) = nullptr;
        uint8_t (RP2A03::*addrmode)(void) = nullptr;
        uint8_t cycles = 0;
    };

    std::vector<INSTRUCTION> lookup;
};