#include "Bus.h"

Bus::Bus() {
    // Initialize RAM to zero
    ram.fill(0x00);

    // Connect CPU to this bus
    cpu.connect_bus(this);
}

Bus::~Bus() = default;

void Bus::write(uint16_t addr, uint8_t data) noexcept {
    if (addr >= 0x0000 && addr <= 0xFFFF) {
        // Address is within RAM range

        // Write data to RAM at the specified address
        ram[addr] = data;
    } else {
        // Handle out-of-bounds address if necessary
        return;
    }
}

uint8_t Bus::read(uint16_t addr, bool bReadOnly) noexcept {
    if (addr >= 0x0000 && addr <= 0xFFFF) {
        // Address is within RAM range

        // Read data from RAM at the specified address
        return ram[addr];
    } else {
        // Handle out-of-bounds address if necessary
        return 0x00;
    }
}
