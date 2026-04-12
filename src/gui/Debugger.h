#include <cstddef>

#include "../core/Bus.h"
#include "../core/RP2A03.h"
#include "../utils/Disassembler.h"

#include "Window.h"

class Debugger : public Window {
public: 
    Bus* bus;
    Disassembler disasm;
    const std::span<uint8_t> memory;

    RP2A03::CPUState cpu_state;

   public:
    Debugger(Bus* bus, const std::span<const RP2A03::INSTRUCTION, 256> lookup);
    ~Debugger() = default;

    void draw_window(const char* title);

    // Treat the memory as a byte array and display it in a hex viewer style
    void draw_memory(uint16_t start, uint16_t count);

    // Treat the memory as 6502 instructions and display the disassembly
    void draw_disassembly(uint16_t start, uint16_t count);

    void draw_cpu_state();

    void calc_sizes(size_t mem_size, size_t base_display_addr);
};