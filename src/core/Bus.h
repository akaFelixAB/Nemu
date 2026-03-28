#pragma once

#include <cstdint>
#include <array>

#include "RP2A03.h"

class Bus {
public:
    Bus();
    ~Bus();

public: // Devices on the bus
    RP2A03 cpu;
    std::array<uint8_t, 64 * 1024> ram;

public:
    void write(uint16_t addr, uint8_t data) noexcept;
    uint8_t read(uint16_t addr, bool bReadOnly = false) noexcept;
};