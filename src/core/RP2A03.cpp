#include <cstdint>

#include "RP2A03.h"

#include "Bus.h"

RP2A03::RP2A03() {
    using a = RP2A03;
    using op = RP2A03::DEBUG_MNEMONIC;
    using am = RP2A03::DEBUG_ADDRESSING_MODE;
    // Initialize the instruction lookup table
    // Corresponding to the 256 opcodes table
    lookup = {{
		{ op::BRK, am::IMM, &a::BRK, &a::IMM, 7 },{ op::ORA, am::IZX, &a::ORA, &a::IZX, 6 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 8 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 3 },{ op::ORA, am::ZP0, &a::ORA, &a::ZP0, 3 },{ op::ASL, am::ZP0, &a::ASL, &a::ZP0, 5 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 5 },{ op::PHP, am::IMP, &a::PHP, &a::IMP, 3 },{ op::ORA, am::IMM, &a::ORA, &a::IMM, 2 },{ op::ASL, am::ACC, &a::ASL, &a::ACC, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 2 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 4 },{ op::ORA, am::ABS, &a::ORA, &a::ABS, 4 },{ op::ASL, am::ABS, &a::ASL, &a::ABS, 6 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 6 },
		{ op::BPL, am::REL, &a::BPL, &a::REL, 2 },{ op::ORA, am::IZY, &a::ORA, &a::IZY, 5 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 8 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 4 },{ op::ORA, am::ZPX, &a::ORA, &a::ZPX, 4 },{ op::ASL, am::ZPX, &a::ASL, &a::ZPX, 6 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 6 },{ op::CLC, am::IMP, &a::CLC, &a::IMP, 2 },{ op::ORA, am::ABY, &a::ORA, &a::ABY, 4 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 7 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 4 },{ op::ORA, am::ABX, &a::ORA, &a::ABX, 4 },{ op::ASL, am::ABX, &a::ASL, &a::ABX, 7 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 7 },
		{ op::JSR, am::ABS, &a::JSR, &a::ABS, 6 },{ op::AND, am::IZX, &a::AND, &a::IZX, 6 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 8 },{ op::BIT, am::ZP0, &a::BIT, &a::ZP0, 3 },{ op::AND, am::ZP0, &a::AND, &a::ZP0, 3 },{ op::ROL, am::ZP0, &a::ROL, &a::ZP0, 5 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 5 },{ op::PLP, am::IMP, &a::PLP, &a::IMP, 4 },{ op::AND, am::IMM, &a::AND, &a::IMM, 2 },{ op::ROL, am::ACC, &a::ROL, &a::ACC, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 2 },{ op::BIT, am::ABS, &a::BIT, &a::ABS, 4 },{ op::AND, am::ABS, &a::AND, &a::ABS, 4 },{ op::ROL, am::ABS, &a::ROL, &a::ABS, 6 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 6 },
		{ op::BMI, am::REL, &a::BMI, &a::REL, 2 },{ op::AND, am::IZY, &a::AND, &a::IZY, 5 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 8 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 4 },{ op::AND, am::ZPX, &a::AND, &a::ZPX, 4 },{ op::ROL, am::ZPX, &a::ROL, &a::ZPX, 6 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 6 },{ op::SEC, am::IMP, &a::SEC, &a::IMP, 2 },{ op::AND, am::ABY, &a::AND, &a::ABY, 4 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 7 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 4 },{ op::AND, am::ABX, &a::AND, &a::ABX, 4 },{ op::ROL, am::ABX, &a::ROL, &a::ABX, 7 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 7 },
		{ op::RTI, am::IMP, &a::RTI, &a::IMP, 6 },{ op::EOR, am::IZX, &a::EOR, &a::IZX, 6 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 8 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 3 },{ op::EOR, am::ZP0, &a::EOR, &a::ZP0, 3 },{ op::LSR, am::ZP0, &a::LSR, &a::ZP0, 5 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 5 },{ op::PHA, am::IMP, &a::PHA, &a::IMP, 3 },{ op::EOR, am::IMM, &a::EOR, &a::IMM, 2 },{ op::LSR, am::ACC, &a::LSR, &a::ACC, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 2 },{ op::JMP, am::ABS, &a::JMP, &a::ABS, 3 },{ op::EOR, am::ABS, &a::EOR, &a::ABS, 4 },{ op::LSR, am::ABS, &a::LSR, &a::ABS, 6 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 6 },
		{ op::BVC, am::REL, &a::BVC, &a::REL, 2 },{ op::EOR, am::IZY, &a::EOR, &a::IZY, 5 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 8 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 4 },{ op::EOR, am::ZPX, &a::EOR, &a::ZPX, 4 },{ op::LSR, am::ZPX, &a::LSR, &a::ZPX, 6 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 6 },{ op::CLI, am::IMP, &a::CLI, &a::IMP, 2 },{ op::EOR, am::ABY, &a::EOR, &a::ABY, 4 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 7 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 4 },{ op::EOR, am::ABX, &a::EOR, &a::ABX, 4 },{ op::LSR, am::ABX, &a::LSR, &a::ABX, 7 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 7 },
		{ op::RTS, am::IMP, &a::RTS, &a::IMP, 6 },{ op::ADC, am::IZX, &a::ADC, &a::IZX, 6 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 8 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 3 },{ op::ADC, am::ZP0, &a::ADC, &a::ZP0, 3 },{ op::ROR, am::ZP0, &a::ROR, &a::ZP0, 5 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 5 },{ op::PLA, am::IMP, &a::PLA, &a::IMP, 4 },{ op::ADC, am::IMM, &a::ADC, &a::IMM, 2 },{ op::ROR, am::ACC, &a::ROR, &a::ACC, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 2 },{ op::JMP, am::IND, &a::JMP, &a::IND, 5 },{ op::ADC, am::ABS, &a::ADC, &a::ABS, 4 },{ op::ROR, am::ABS, &a::ROR, &a::ABS, 6 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 6 },
		{ op::BVS, am::REL, &a::BVS, &a::REL, 2 },{ op::ADC, am::IZY, &a::ADC, &a::IZY, 5 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 8 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 4 },{ op::ADC, am::ZPX, &a::ADC, &a::ZPX, 4 },{ op::ROR, am::ZPX, &a::ROR, &a::ZPX, 6 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 6 },{ op::SEI, am::IMP, &a::SEI, &a::IMP, 2 },{ op::ADC, am::ABY, &a::ADC, &a::ABY, 4 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 7 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 4 },{ op::ADC, am::ABX, &a::ADC, &a::ABX, 4 },{ op::ROR, am::ABX, &a::ROR, &a::ABX, 7 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 7 },
		{ op::XXX, am::IMP, &a::NOP, &a::IMP, 2 },{ op::STA, am::IZX, &a::STA, &a::IZX, 6 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 6 },{ op::STY, am::ZP0, &a::STY, &a::ZP0, 3 },{ op::STA, am::ZP0, &a::STA, &a::ZP0, 3 },{ op::STX, am::ZP0, &a::STX, &a::ZP0, 3 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 3 },{ op::DEY, am::IMP, &a::DEY, &a::IMP, 2 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 2 },{ op::TXA, am::IMP, &a::TXA, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 2 },{ op::STY, am::ABS, &a::STY, &a::ABS, 4 },{ op::STA, am::ABS, &a::STA, &a::ABS, 4 },{ op::STX, am::ABS, &a::STX, &a::ABS, 4 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 4 },
		{ op::BCC, am::REL, &a::BCC, &a::REL, 2 },{ op::STA, am::IZY, &a::STA, &a::IZY, 6 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 6 },{ op::STY, am::ZPX, &a::STY, &a::ZPX, 4 },{ op::STA, am::ZPX, &a::STA, &a::ZPX, 4 },{ op::STX, am::ZPY, &a::STX, &a::ZPY, 4 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 4 },{ op::TYA, am::IMP, &a::TYA, &a::IMP, 2 },{ op::STA, am::ABY, &a::STA, &a::ABY, 5 },{ op::TXS, am::IMP, &a::TXS, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 5 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 5 },{ op::STA, am::ABX, &a::STA, &a::ABX, 5 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 5 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 5 },
		{ op::LDY, am::IMM, &a::LDY, &a::IMM, 2 },{ op::LDA, am::IZX, &a::LDA, &a::IZX, 6 },{ op::LDX, am::IMM, &a::LDX, &a::IMM, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 6 },{ op::LDY, am::ZP0, &a::LDY, &a::ZP0, 3 },{ op::LDA, am::ZP0, &a::LDA, &a::ZP0, 3 },{ op::LDX, am::ZP0, &a::LDX, &a::ZP0, 3 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 3 },{ op::TAY, am::IMP, &a::TAY, &a::IMP, 2 },{ op::LDA, am::IMM, &a::LDA, &a::IMM, 2 },{ op::TAX, am::IMP, &a::TAX, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 2 },{ op::LDY, am::ABS, &a::LDY, &a::ABS, 4 },{ op::LDA, am::ABS, &a::LDA, &a::ABS, 4 },{ op::LDX, am::ABS, &a::LDX, &a::ABS, 4 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 4 },
		{ op::BCS, am::REL, &a::BCS, &a::REL, 2 },{ op::LDA, am::IZY, &a::LDA, &a::IZY, 5 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 5 },{ op::LDY, am::ZPX, &a::LDY, &a::ZPX, 4 },{ op::LDA, am::ZPX, &a::LDA, &a::ZPX, 4 },{ op::LDX, am::ZPY, &a::LDX, &a::ZPY, 4 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 4 },{ op::CLV, am::IMP, &a::CLV, &a::IMP, 2 },{ op::LDA, am::ABY, &a::LDA, &a::ABY, 4 },{ op::TSX, am::IMP, &a::TSX, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 4 },{ op::LDY, am::ABX, &a::LDY, &a::ABX, 4 },{ op::LDA, am::ABX, &a::LDA, &a::ABX, 4 },{ op::LDX, am::ABY, &a::LDX, &a::ABY, 4 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 4 },
		{ op::CPY, am::IMM, &a::CPY, &a::IMM, 2 },{ op::CMP, am::IZX, &a::CMP, &a::IZX, 6 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 8 },{ op::CPY, am::ZP0, &a::CPY, &a::ZP0, 3 },{ op::CMP, am::ZP0, &a::CMP, &a::ZP0, 3 },{ op::DEC, am::ZP0, &a::DEC, &a::ZP0, 5 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 5 },{ op::INY, am::IMP, &a::INY, &a::IMP, 2 },{ op::CMP, am::IMM, &a::CMP, &a::IMM, 2 },{ op::DEX, am::IMP, &a::DEX, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 2 },{ op::CPY, am::ABS, &a::CPY, &a::ABS, 4 },{ op::CMP, am::ABS, &a::CMP, &a::ABS, 4 },{ op::DEC, am::ABS, &a::DEC, &a::ABS, 6 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 6 },
		{ op::BNE, am::REL, &a::BNE, &a::REL, 2 },{ op::CMP, am::IZY, &a::CMP, &a::IZY, 5 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 8 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 4 },{ op::CMP, am::ZPX, &a::CMP, &a::ZPX, 4 },{ op::DEC, am::ZPX, &a::DEC, &a::ZPX, 6 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 6 },{ op::CLD, am::IMP, &a::CLD, &a::IMP, 2 },{ op::CMP, am::ABY, &a::CMP, &a::ABY, 4 },{ op::NOP, am::IMP, &a::NOP, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 7 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 4 },{ op::CMP, am::ABX, &a::CMP, &a::ABX, 4 },{ op::DEC, am::ABX, &a::DEC, &a::ABX, 7 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 7 },
		{ op::CPX, am::IMM, &a::CPX, &a::IMM, 2 },{ op::SBC, am::IZX, &a::SBC, &a::IZX, 6 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 8 },{ op::CPX, am::ZP0, &a::CPX, &a::ZP0, 3 },{ op::SBC, am::ZP0, &a::SBC, &a::ZP0, 3 },{ op::INC, am::ZP0, &a::INC, &a::ZP0, 5 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 5 },{ op::INX, am::IMP, &a::INX, &a::IMP, 2 },{ op::SBC, am::IMM, &a::SBC, &a::IMM, 2 },{ op::NOP, am::IMP, &a::NOP, &a::IMP, 2 },{ op::XXX, am::IMP, &a::SBC, &a::IMP, 2 },{ op::CPX, am::ABS, &a::CPX, &a::ABS, 4 },{ op::SBC, am::ABS, &a::SBC, &a::ABS, 4 },{ op::INC, am::ABS, &a::INC, &a::ABS, 6 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 6 },
		{ op::BEQ, am::REL, &a::BEQ, &a::REL, 2 },{ op::SBC, am::IZY, &a::SBC, &a::IZY, 5 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 8 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 4 },{ op::SBC, am::ZPX, &a::SBC, &a::ZPX, 4 },{ op::INC, am::ZPX, &a::INC, &a::ZPX, 6 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 6 },{ op::SED, am::IMP, &a::SED, &a::IMP, 2 },{ op::SBC, am::ABY, &a::SBC, &a::ABY, 4 },{ op::NOP, am::IMP, &a::NOP, &a::IMP, 2 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 7 },{ op::XXX, am::IMP, &a::NOP, &a::IMP, 4 },{ op::SBC, am::ABX, &a::SBC, &a::ABX, 4 },{ op::INC, am::ABX, &a::INC, &a::ABX, 7 },{ op::XXX, am::IMP, &a::XXX, &a::IMP, 7 },
	}};
}

// #######################
// #### BUS INTERFACE ####
// #######################

inline uint8_t RP2A03::read(uint16_t addr) const noexcept {
    return bus->read(addr, false);
}

inline void RP2A03::write(uint16_t addr, uint8_t data) noexcept {
    bus->write(addr, data);
}

inline uint8_t RP2A03::GetFlag(FLAGS6502 flag) const noexcept { return (status & flag) > 0 ? 1 : 0; }
inline void RP2A03::SetFlag(FLAGS6502 flag, bool value) noexcept {
    if (value) {
        status |= flag;
    } else {
        status &= ~flag;
    }
}

// #####################
// #### CPU CONTROL ####
// #####################

// The main clock function which executes instructions
void RP2A03::clock() {
    if (cycles == 0) {
        // Read next opcode
        opcode = read(pc++);

        // Get number of cycles
        cycles = lookup[opcode].cycles;

        // Perform the opcode
        // on the addressing mode
        // and operation
        uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();
        uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();

        // Some instructions add additional clock cycles
        cycles += (additional_cycle1 & additional_cycle2);
    }
    cycles--;
}

void RP2A03::reset() {
    // Reset the CPU to a known state
    a = 0;
    x = 0;
    y = 0;
    sp = 0xFD;          // Stack Pointer starts at 0xFD after reset
    status = 0x00 | U | I;  // Clear all flags except for the unused flag and
                            // the interrupt disable flag, which are set after reset

    addr_abs =
        RESET_VECTOR;  // The reset vector is located at 0xFFFC and 0xFFFD
    uint16_t lo = read(addr_abs + 0);  // Read the low byte of the reset vector
    uint16_t hi = read(addr_abs + 1);  // Read the high byte of the reset vector

    pc = (hi << 8) | lo;  // Set the program counter to the address specified by
                          // the reset vector

    addr_rel = 0x0000;
    addr_abs = 0x0000;
    fetched = 0x00;

    cycles = 8;  // Reset takes time to complete, 7/8 cycles?
}

void RP2A03::irq() {
    // Interrupt Request (IRQ)
    // This is a maskable interrupt, meaning it can be ignored if the Interrupt
    // Disable flag is set

    if (GetFlag(I) == 0) {  // Only process the interrupt if the Interrupt
                            // Disable flag is clear
        write(STACK_BASE + sp,
              (pc >> 8) & 0x00FF);  // Push the high byte of the program counter
                                    // onto the stack
        sp--;
        write(STACK_BASE + sp, pc & 0x00FF);  // Push the low byte of the
                                              // program counter onto the stack
        sp--;

        // When an interrupt occurs, the processor automatically pushes the
        // current program counter and status register onto the stack
        SetFlag(B, 0);
        SetFlag(U, 1);
        SetFlag(I, 1);  // Disable further interrupts
        write(STACK_BASE + sp,
              status);  // Push the processor status onto the stack
        sp--;

        addr_abs =
            IRQ_VECTOR;  // The interrupt vector is located at 0xFFFE and 0xFFFF
        uint16_t lo =
            read(addr_abs + 0);  // Read the low byte of the interrupt vector
        uint16_t hi =
            read(addr_abs + 1);  // Read the high byte of the interrupt vector
        pc = (hi << 8) | lo;     // Set the program counter to the address
                                 // specified by the interrupt vector

        cycles = 7;  // IRQ takes time to complete, 7 cycles?
    }
}

void RP2A03::nmi() {
    // Non-Maskable Interrupt (NMI)
    // This interrupt cannot be ignored and will always be processed

    write(STACK_BASE + sp,
          (pc >> 8) & 0x00FF);  // Push the high byte of the program counter
                                // onto the stack
    sp--;
    write(
        STACK_BASE + sp,
        pc &
            0x00FF);  // Push the low byte of the program counter onto the stack
    sp--;

    SetFlag(B, 0);
    SetFlag(U, 1);
    SetFlag(I, 1);                   // Disable further interrupts
    write(STACK_BASE + sp, status);  // Push the processor status onto the stack
    sp--;

    addr_abs = NMI_VECTOR;  // The non-maskable interrupt vector is located at
                            // 0xFFFA and 0xFFFB
    uint16_t lo =
        read(addr_abs +
             0);  // Read the low byte of the non-maskable interrupt vector
    uint16_t hi =
        read(addr_abs +
             1);  // Read the high byte of the non-maskable interrupt vector
    pc = (hi << 8) | lo;  // Set the program counter to the address specified by
                          // the non-maskable interrupt vector

    cycles = 8;  // NMI takes time to complete, 8 cycles?
}

// ##########################
// #### ADDRESSING MODES ####
// ##########################

// Each returns an additional clock cycle if page boundary is crossed

uint8_t RP2A03::IMP() noexcept {
    // (Implied) Addressing Mode
    // There is no additional data required for this instruction
    // The operand is implied by the instruction itself
    // e.g. CLC, SEC, INX, NOP, etc.

    // The instruction operates on the accumulator or
    // on the processor status register
    // so there is no need to fetch any additional data
    fetched = a;
    return 0;
}

uint8_t RP2A03::ACC() noexcept {
    // (Accumulator) Addressing Mode
    // The instruction operates directly on the accumulator
    // There is no additional data required for this instruction

    // This is used by instructions that specifically operate on the
    // accumulator, such as ASL A, ROL A, LSR A, ROR A, etc.
    fetched = a;
    return 0;
}

uint8_t RP2A03::IMM() noexcept {
    // (Immediate) Addressing Mode
    // The operand is the next byte in memory
    addr_abs = pc++;
    return 0;
}

uint8_t RP2A03::ZP0() noexcept {
    // (Zero Page) Addressing Mode
    // The address is always in the first 256 bytes of memory

    // Address 0x0055 can be split into two parts
    // 0x00 and 0x55
    // 0x00 is the page number
    // 0x55 is the offset in that page

    // The memory is made up of 256 pages
    // each page has 256 bytes

    // Zero Page Addressing Mode means
    // the address is always in page 0
    // so the high byte is always 0x00

    // 6502 tend to have its working data
    // in the first 256 bytes of memory
    addr_abs = read(pc++);
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t RP2A03::ZPX() noexcept {
    // (Zero Page,X) Addressing Mode
    // The effective address is the sum of
    // the zero page address and the X register
    // This sum wraps around within the zero page

    // This is useful for accessing arrays
    // or structures in the zero page
    // which can be indexed by the values in the X register
    addr_abs = (read(pc++) + x);
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t RP2A03::ZPY() noexcept {
    // (Zero Page,Y) Addressing Mode
    // The same as Zero Page,X but uses the Y register instead

    // This is less common than Zero Page,X
    // but can be useful for certain instructions
    addr_abs = (read(pc++) + y);
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t RP2A03::ABS() noexcept {
    // (Absolute) Addressing Mode
    // The address is specified by the next two bytes
    // in little-endian format (low byte first)
    uint16_t lo = read(pc++);
    uint16_t hi = read(pc++);
    addr_abs = (hi << 8) | lo; // Combine high and low bytes to form the full address
    return 0;
}

uint8_t RP2A03::ABX() noexcept {
    // (Absolute,X) Addressing Mode
    // The effective address is the sum of
    // the absolute address and the X register
    // (absolute address with X offset)

    // This is useful for accessing arrays or structures
    // in memory which can be indexed by the values in the X register
    uint16_t lo = read(pc++);
    uint16_t hi = read(pc++);
    addr_abs = (hi << 8) | lo;
    addr_abs += x;

    // Check if page boundary is crossed
    if ((addr_abs & 0xFF00) != (hi << 8))
        return 1; // Add an additional clock cycle if page boundary is crossed
    else
        return 0;
}

uint8_t RP2A03::ABY() noexcept {
    // (Absolute,Y) Addressing Mode
    // Same as Absolute,X but uses the Y register instead

    // This is useful for accessing arrays or structures
    // in memory which can be indexed by the values in the Y register
    uint16_t lo = read(pc++);
    uint16_t hi = read(pc++);
    addr_abs = (hi << 8) | lo;
    addr_abs += y;

    // Check if page boundary is crossed
    if ((addr_abs & 0xFF00) != (hi << 8))
        return 1; // Add an additional clock cycle if page boundary is crossed
    else
        return 0;
}

uint8_t RP2A03::IND() noexcept {
    // (Indirect) Addressing Mode
    // The address of the operand is found in memory
    // at the location specified by the next two bytes

    // This is used by the JMP instruction to jump to a new location
    uint16_t ptr_lo = read(pc++);
    uint16_t ptr_hi = read(pc++);
    uint16_t ptr = (ptr_hi << 8) | ptr_lo;

    // Simulate page boundary hardware bug
    if (ptr_lo == 0x00FF) {                                   // If the low byte is 0xFF
        addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0); // Wrap around to beginning of page
    } else {                                                  // Normal behavior
        addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
    }
    return 0;
}

uint8_t RP2A03::IZX() noexcept {
    // (Indirect Indexed) Addressing Mode (Indirect,X)
    // The effective address is found by adding the X register
    // to the zero page address specified by the next byte
    // The result is a pointer to the actual 16-bit address

    // This is useful for accessing arrays of pointers in the
    // zero page which can be indexed by the values in the X register
    uint16_t t = read(pc++);                                      // Get the zero page address
    uint16_t lo = read((uint16_t)(t + (uint16_t)x) & 0x00FF);     // Read low byte of effective address
    uint16_t hi = read((uint16_t)(t + (uint16_t)x + 1) & 0x00FF); // Read high byte of effective address

    addr_abs = (hi << 8) | lo;
    
    return 0;
}

uint8_t RP2A03::IZY() noexcept {
    // (Indirect Indexed) Addressing Mode (Indirect,Y)
    // The effective address is found by taking the zero page address
    // specified by the next byte as a pointer to a 16-bit address
    // and then adding the Y register to this address

    // This is useful for accessing an object or structure in memory
    // which can be indexed by the values in the Y register
    uint16_t t = read(pc++);              // Get the zero page address
    uint16_t lo = read(t & 0x00FF);       // Read low byte of base address
    uint16_t hi = read((t + 1) & 0x00FF); // Read high byte of base address

    addr_abs = (hi << 8) | lo;
    addr_abs += y;                        // Add Y register to get effective address

    if ((addr_abs & 0xFF00) != (hi << 8))
        return 1;                         // Add an additional clock cycle if page boundary is crossed
    else
        return 0;
}

uint8_t RP2A03::REL() noexcept {
    // (Relative) Addressing Mode
    // Used by branch instructions to calculate the target address
    // The operand is a signed 8-bit value representing the offset

    // This offset is added to the program counter (PC)
    // to determine the target address for the branch
    addr_rel = read(pc++);
    if (addr_rel & 0x80) // 0b10000000     // If the sign bit (highest bit) is set to 1
        addr_rel |= 0xFF00;                // Extend the sign to 16 bits
    return 0;
}

// ######################
// #### INSTRUCTIONS ####
// ######################

uint8_t RP2A03::fetch() { // Helper function to fetch data
    if (lookup[opcode].addrmode != &RP2A03::IMP)
        fetched = read(addr_abs);
    return fetched;  // Return just in case...
}

// Each returns an additional clock cycle if the operation requires it
// Depends on the addressing mode and whether a page boundary is crossed

// **LOAD/STORE INSTRUCTIONS**

uint8_t RP2A03::LDA() noexcept { // Load Accumulator
    fetch();
    a = fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 1;
}

uint8_t RP2A03::LDX() noexcept { // Load X Register
    fetch();
    x = fetched;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 1;
}

uint8_t RP2A03::LDY() noexcept { // Load Y Register
    fetch();
    y = fetched;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);
    return 1;
}

uint8_t RP2A03::STA() noexcept { // Store Accumulator
    write(addr_abs, a);
    return 0;
}

uint8_t RP2A03::STX() noexcept { // Store X Register
    write(addr_abs, x);
    return 0;
}

uint8_t RP2A03::STY() noexcept { // Store Y Register
    write(addr_abs, y);
    return 0;
}

uint8_t RP2A03::TAX() noexcept { // Transfer Accumulator to X
    x = a;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 0;
}

uint8_t RP2A03::TAY() noexcept { // Transfer Accumulator to Y
    y = a;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);
    return 0;
}

uint8_t RP2A03::TSX() noexcept { // Transfer Stack Pointer to X
    x = sp;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 0;
}

uint8_t RP2A03::TXA() noexcept { // Transfer X to Accumulator
    a = x;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 0;
}

uint8_t RP2A03::TXS() noexcept { // Transfer X to Stack Pointer
    sp = x;
    return 0;
}

uint8_t RP2A03::TYA() noexcept { // Transfer Y to Accumulator
    a = y;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 0;
}

// **ARITHMETIC INSTRUCTIONS**

uint8_t RP2A03::ADC() noexcept {
    /*
    truth table for ADC (Add with Carry)
    A = Accumulator
    M = Memory (fetched value)
    R = Result (A + M + C)
    V = Overflow Flag

    | A7 | M7 | R7 | V | Description
    |----|----|----|---|-----------------------------
    | 0  | 0  | 0  | 0 | No overflow
    | 0  | 0  | 1  | 1 | Overflow (positive + positive = negative)
    | 0  | 1  | 0  | 0 | No overflow
    | 0  | 1  | 1  | 0 | No overflow
    | 1  | 0  | 0  | 0 | No overflow
    | 1  | 0  | 1  | 0 | No overflow
    | 1  | 1  | 0  | 1 | Overflow (negative + negative = positive)
    | 1  | 1  | 1  | 0 | No overflow

    Therefore, overflow occurs when:
        1. The sign bits of A and M are the same (both 0 or both 1)
        2. The sign bit of R is different from A and M
    */

    fetch();
    temp = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(C);
    SetFlag(C, temp > 0x00FF);        // Set Carry flag if result exceeds 8 bits
    SetFlag(Z, (temp & 0x00FF) == 0); // Set Zero flag if result is zero
    SetFlag(N, temp & 0x0080);          // Set Negative flag if result's highest bit is set to 1
    SetFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080);
                                      // Set Overflow flag based on the conditions described above
    a = temp & 0x00FF;                // Store the result back in the accumulator (only the lower 8 bits)
    return 1;  // Potentially add an additional clock cycle if page boundary is crossed
}

uint8_t RP2A03::SBC() noexcept {
    // Subtract with Carry
    // A = A - M - (1 - C)
    // This can be rewritten as A = A - M - (1 - C)
    //                            = A + (-M) - 1 + C
    //                            = A + (~M + 1) - 1 + C
    //                            = A + (~M) + C
    fetch();
    uint16_t value = ((uint16_t)fetched) ^ 0x00FF; // Invert the bits of the fetched value for subtraction
    temp = (uint16_t)a + value + (uint16_t)GetFlag(C); // Add the inverted value and the carry flag to the accumulator
    SetFlag(C, temp > 0x00FF);        // Set Carry flag if result exceeds 8 bits (indicates no borrow)
    SetFlag(Z, (temp & 0x00FF) == 0); // Set Zero flag if result is zero
    SetFlag(N, temp & 0x0080);          // Set Negative flag if result's highest bit is set to 1
    SetFlag(V, (((uint16_t)a ^ value) & ((uint16_t)a ^ temp)) & 0x0080);
                                      // Set Overflow flag based on the conditions for subtraction
    a = temp & 0x00FF;                // Store the result back in the accumulator (only the lower 8 bits)
    return 1;  // Potentially add an additional clock cycle if page boundary is crossed
}

uint8_t RP2A03::ASL() noexcept { // Arithmetic Shift Left
    fetch();
    temp = (uint16_t)fetched << 1;
    SetFlag(C, (temp & 0xFF00) > 0);     // Set Carry flag if the highest bit of the original value is 1 (indicates a shift out of the byte)
    SetFlag(Z, (temp & 0x00FF) == 0x00); // Set Zero flag if result is zero
    SetFlag(N, temp & 0x0080);           // Set Negative flag if result's highest bit is set to 1
    if (lookup[opcode].addrmode == &RP2A03::IMP)
        a = temp & 0x00FF;               // If the instruction operates on the accumulator, store the result back in the accumulator
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
}

uint8_t RP2A03::LSR() noexcept { // Logical Shift Right
    fetch();
    SetFlag(C, fetched & 0x0001);
    temp = fetched >> 1;
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    if (lookup[opcode].addrmode == &RP2A03::IMP)
        a = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
}

uint8_t RP2A03::ROL() noexcept { // Rotate Left
    // Rotate Left shifts all bits to the left
    // The lowest bit is filled with the value of the Carry flag
    // The highest bit is moved into the Carry flag
    // E.g. 0b10110011 C = 0 ==> ROL ==> 0b01100110 C = 1
    fetch();
    temp = (uint16_t)(fetched << 1) | GetFlag(C);
    SetFlag(C, temp & 0xFF00);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    if (lookup[opcode].addrmode == &RP2A03::IMP)
        a = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
}

uint8_t RP2A03::ROR() noexcept { // Rotate Right
    // E.g. 0b10110011 C = 1 ==> ROR ==> 0b11011001 C = 1
    fetch();
    temp = (uint16_t)(GetFlag(C) << 7) | (fetched >> 1);
    SetFlag(C, fetched & 0x01);
    SetFlag(Z, (temp & 0x00FF) == 0x00);
    SetFlag(N, temp & 0x0080);
    if (lookup[opcode].addrmode == &RP2A03::IMP)
        a = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
}

uint8_t RP2A03::CMP() noexcept { // Compare Accumulator
    fetch();
    temp = (uint16_t)a - (uint16_t)fetched; // Perform the subtraction to compare the values
    SetFlag(C, a >= fetched);               // Set Carry flag if the accumulator is greater than or equal to the fetched value (indicates no borrow)
    SetFlag(Z, (temp & 0x00FF) == 0x0000);  // Set Zero flag if the result of the comparison is zero (indicates equality)
    SetFlag(N, temp & 0x0080); // Set Negative flag if the result's highest bit is set to 1 (indicates the accumulator is less than the fetched value)
    return 1;
}

uint8_t RP2A03::CPX() noexcept { // Compare X Register
    fetch();
    temp = (uint16_t)x - (uint16_t)fetched;
    SetFlag(C, x >= fetched);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 0;
}

uint8_t RP2A03::CPY() noexcept { // Compare Y Register
    fetch();
    temp = (uint16_t)y - (uint16_t)fetched;
    SetFlag(C, y >= fetched);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 0;
}

uint8_t RP2A03::DEC() noexcept { // Decrement Memory
    fetch();
    temp = fetched - 1;
    write(addr_abs, temp & 0x00FF);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 0;
}

uint8_t RP2A03::DEX() noexcept { // Decrement X Register
    x--;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 0;
}

uint8_t RP2A03::DEY() noexcept { // Decrement Y Register
    y--;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);
    return 0;
}

uint8_t RP2A03::AND() noexcept {  // Logical AND
    fetch();
    a &= fetched;
    SetFlag(Z, a == 0x00);  // Set Zero flag if result is zero
    SetFlag(N,
            a & 0x80);  // Set Negative flag if result's highest bit is set to 1
    return 1;  // Potentially add an additional clock cycle if page boundary is
               // crossed
}

uint8_t RP2A03::ORA() noexcept {  // Inclusive OR
    fetch();
    a = a | fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 1;
}

uint8_t RP2A03::EOR() noexcept { // Exclusive OR
    fetch();
    a = a ^ fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 1;
}

uint8_t RP2A03::INC() noexcept { // Increment Memory
    fetch();
    temp = fetched + 1;
    write(addr_abs, temp & 0x00FF);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 0;
}

uint8_t RP2A03::INX() noexcept { // Increment X Register
    x++;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 0;
}

uint8_t RP2A03::INY() noexcept { // Increment Y Register
    y++;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);
    return 0;
}

// **BRANCH INSTRUCTIONS**
// Each returns an additional clock cycle if branch is taken

uint8_t RP2A03::BCS() noexcept { // Branch if Carry Set
    if (GetFlag(C) == 1) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;   // Add an additional clock cycle if page boundary is crossed

        pc = addr_abs;
    }
    return 0;
}

uint8_t RP2A03::BCC() noexcept { // Branch if Carry Clear
    if (GetFlag(C) == 0) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;  

        pc = addr_abs;
    }
    return 0;
}

uint8_t RP2A03::BEQ() noexcept { // Branch if Equal (Zero Set)
    if (GetFlag(Z) == 1) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}

uint8_t RP2A03::BNE() noexcept { // Branch if Not Equal (Zero Clear)
    if (GetFlag(Z) == 0) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}

uint8_t RP2A03::BMI() noexcept { // Branch if Minus (Negative Set)
    if (GetFlag(N) == 1) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}

uint8_t RP2A03::BPL() noexcept { // Branch if Plus (Negative Clear)
    if (GetFlag(N) == 0) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}

uint8_t RP2A03::BVS() noexcept { // Branch if Overflow Set
    if (GetFlag(V) == 1) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}

uint8_t RP2A03::BVC() noexcept { // Branch if Overflow Clear
    if (GetFlag(V) == 0) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}

uint8_t RP2A03::JMP() noexcept { // Jump to New Location 
    pc = addr_abs;
    return 0;
}

uint8_t RP2A03::JSR() noexcept { // Jump to Subroutine
    // When a JSR instruction is executed, the processor pushes the address of the 
    // next instruction (the return address) onto the stack before jumping to the subroutine.
    pc--;

    write(STACK_BASE + sp, (pc >> 8) & 0x00FF);
    sp--;
    write(STACK_BASE + sp, pc & 0x00FF);
    sp--;

    pc = addr_abs;
    return 0;
}

// **FLAG INSTRUCTIONS**
// Each of these instructions set or clear a specific processor status flag

uint8_t RP2A03::CLC() noexcept { // Clear Carry Flag
    SetFlag(C, false);
    return 0;
}

uint8_t RP2A03::CLD() noexcept { // Clear Decimal Mode
    SetFlag(D, false);
    return 0;
}

uint8_t RP2A03::CLI() noexcept { // Clear Interrupt Disable
    SetFlag(I, false);
    return 0;
}

uint8_t RP2A03::CLV() noexcept { // Clear Overflow Flag
    SetFlag(V, false);
    return 0;
}

uint8_t RP2A03::SEC() noexcept { // Set Carry Flag
    SetFlag(C, true);
    return 0;
}

uint8_t RP2A03::SED() noexcept { // Set Decimal Flag
    SetFlag(D, true);
    return 0;
}

uint8_t RP2A03::SEI() noexcept { // Set Interrupt Disable
    SetFlag(I, true);
    return 0;
}

uint8_t RP2A03::BIT() noexcept { // Test Bits in Memory with Accumulator
    fetch();
    temp = a & fetched;
    SetFlag(Z, (temp & 0x00FF) == 0x00); // Set Zero flag if result is zero
    SetFlag(N, fetched & (1 << 7));      // Set Negative flag based on the highest bit of the fetched value
    SetFlag(V, fetched & (1 << 6));      // Set Overflow flag based on the 6th bit of the fetched value
    return 0;
}

// **STACK INSTRUCTIONS**
// Each of these instructions manipulate the stack pointer and/or the stack

uint8_t RP2A03::PHA() noexcept { // Push Accumulator
    write(STACK_BASE + sp, a); // The stack is located in the memory range 0x0100 to 0x01FF
    sp--;
    return 0;
}

uint8_t RP2A03::PLA() noexcept { // Pop Accumulator
    sp++;
    a = read(STACK_BASE + sp);
    SetFlag(Z, a == 0x00);  // Set Zero flag if result is zero
    SetFlag(N, a & 0x0080); // Set Negative flag if result's highest bit is set to 1
    return 0;
}

uint8_t RP2A03::PHP() noexcept { // Push Processor Status
    write(STACK_BASE + sp, status | B | U);
    SetFlag(B, 1);
    SetFlag(U, 1);
    sp--;
    return 0;
}

uint8_t RP2A03::PLP() noexcept { // Pop Processor Status
    sp++;
    status = read(STACK_BASE + sp);
    SetFlag(U, 1);
    return 0;
}

// **INTERRUPT INSTRUCTIONS**

uint8_t RP2A03::RTI() noexcept { // Return from Interrupt
    sp++;
    status = read(STACK_BASE + sp); // Pull the processor status from the stack
    SetFlag(B, 0);                  // Clear the Break flag
    SetFlag(U, 1);                  // Set the unused flag to 1 (it is always set to 1)
    
    sp++;
    uint16_t lo = read(STACK_BASE + sp); // Pull the low byte of the program counter from the stack
    sp++;
    uint16_t hi = read(STACK_BASE + sp); // Pull the high byte of the program counter from the stack
    pc = (hi << 8) | lo;
    
    return 0;
}

uint8_t RP2A03::RTS() noexcept { // Return from Subroutine
    sp++;
    uint16_t lo = read(STACK_BASE + sp); // Pull the low byte of the return address from the stack
    sp++;
    uint16_t hi = read(STACK_BASE + sp); // Pull the high byte of the return address from the stack
    pc = ((hi << 8) | lo) + 1;           // Set the program counter to the return address (add 1 to point to the next instruction after the JSR)
    
    return 0;
}

uint8_t RP2A03::BRK() noexcept { // Force Interrupt
    pc++;

    SetFlag(I, 1); // Set Interrupt Disable flag to prevent further interrupts
    write(STACK_BASE + sp, (pc >> 8) & 0x00FF);
    sp--;
    write(STACK_BASE + sp, pc & 0x00FF);
    sp--;

    // When a BRK instruction is executed, the processor pushes the 
    // status of the processor onto the stack with the Break flag set to 1
    // (not the actual Break flag in the status register, but a copy of the status with the Break flag set)
    temp = status | B | U;
    write(STACK_BASE + sp, temp);
    sp--;

    addr_abs = IRQ_VECTOR;
    uint16_t lo = read(addr_abs + 0);
    uint16_t hi = read(addr_abs + 1);
    pc = (hi << 8) | lo;

    return 0;
}

// **NOP - No Operation**
// NOP instructions do nothing and are often used for timing purposes or to create space for future code modifications
uint8_t RP2A03::NOP() noexcept {
    // TODO: Some NOP instructions have different addressing modes and may require additional clock cycles
    return 0;
}

// **XXX - Illegal/Undocumented Opcode**
// These are opcodes that do not correspond to any official instruction in the 6502 instruction set
// They may have been used for testing or debugging purposes, or may have been unintended consequences
uint8_t RP2A03::XXX() noexcept {
    return 0;
}
