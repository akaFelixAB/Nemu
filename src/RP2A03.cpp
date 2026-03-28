#include <cstdint>
#include "RP2A03.h"
#include "Bus.h"

RP2A03::RP2A03() {
    using a = RP2A03;
    // Initialize the instruction lookup table
    // Corresponding to the 256 opcodes table
    lookup = {
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
}

RP2A03::~RP2A03() = default;

// #######################
// #### BUS INTERFACE ####
// #######################

inline uint8_t RP2A03::read(uint16_t addr) noexcept {
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
void RP2A03::clock() noexcept {
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

void RP2A03::reset() noexcept {
    // Reset the CPU to a known state
    a = 0;
    x = 0;
    y = 0;
    sp = 0xFD;          // Stack Pointer starts at 0xFD after reset
    status = 0x00 | U;  // Clear all flags except for the unused flag which is
                        // always set to 1

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

void RP2A03::irq() noexcept {
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

void RP2A03::nmi() noexcept {
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
    fetched = a; // ???
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

uint8_t RP2A03::fetch() noexcept { // Helper function to fetch data
    if (lookup[opcode].addrmode == &RP2A03::IMP) fetched = read(addr_abs);
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
    SetFlag(B, 0);
    SetFlag(U, 0);
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
    status &= ~B;                   // Clear the Break flag
    status |= U;                    // Set the unused flag to 1 (it is always set to 1)
    
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

    SetFlag(I, 1);
    write(STACK_BASE + sp, (pc >> 8) & 0x00FF);
    sp--;
    write(STACK_BASE + sp, pc & 0x00FF);
    sp--;

    SetFlag(B, 1);
    write(STACK_BASE + sp, status);
    sp--;
    SetFlag(B, 0);

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
