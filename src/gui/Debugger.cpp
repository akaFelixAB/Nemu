#include "Debugger.h"

Debugger::Debugger(
    Bus* bus,
    const std::span<const RP2A03::INSTRUCTION, 256> lookup
) : bus(bus),
    disasm(bus, lookup) {}

void Debugger::draw_window(const char* title) {
    Sizes s;
    s.ContentWidth = ImGui::GetContentRegionAvail().x;
    s.ContentHeight = ImGui::GetContentRegionAvail().y;
    s.WindowWidth = ImGui::GetWindowSize().x;
    s.windowHeight = ImGui::GetWindowSize().y;
    // calc_sizes(mem_size, base_display_addr);
    ImGui::SetNextWindowSize(ImVec2(s.WindowWidth, s.windowHeight), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);

    open = true;
    if (ImGui::Begin(
        title, &open,
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings
    )) {
        cpu_state = bus->cpu.get_state();

        bool cpu_state_open = ImGui::BeginChild("CPU State", ImVec2(508, 50),
            ImGuiChildFlags_FrameStyle |
            ImGuiChildFlags_AlwaysUseWindowPadding);
        if (cpu_state_open) {
            draw_cpu_state();
        }
        ImGui::EndChild();

        bool mem_open = ImGui::BeginChild("Memory", ImVec2(400, 350),
            ImGuiChildFlags_FrameStyle | ImGuiChildFlags_AlwaysUseWindowPadding,
            ImGuiWindowFlags_NoScrollbar);
        if (mem_open) {
            draw_memory(0, 256);
        }
        ImGui::EndChild();

        ImGui::SameLine();
        bool disasm_open = ImGui::BeginChild("Disassembly", ImVec2(100, 150),
            ImGuiChildFlags_FrameStyle | 
            ImGuiChildFlags_AlwaysUseWindowPadding, 
            ImGuiWindowFlags_NoScrollbar);
        if (disasm_open) {
            draw_disassembly(cpu_state.pc, 6);
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

void Debugger::draw_memory(uint16_t start, uint16_t count) {
    std::string memory_layout = disasm.get_memory_layout(start, count);
    ImGui::TextUnformatted(memory_layout.c_str());
}

void Debugger::draw_disassembly(uint16_t start, uint16_t count) {
    std::vector<Disassembler::DisasmLine> lines = disasm.disassemble(start, count);
    for (const auto& line : lines) {
        std::string disasm_text = disasm.format(line);
        ImGui::TextUnformatted(disasm_text.c_str());
    }
}

void Debugger::draw_cpu_state() {
    ImGui::Text("A: $%02X\t\tX: $%02X\t\tY: $%02X\t\tSP: $%02X\t\tPC: $%04X",
        cpu_state.a, cpu_state.x, cpu_state.y, cpu_state.sp, cpu_state.pc);
    ImGui::Text("Status: %s%s%s%s%s%s%s%s",
        (cpu_state.status & RP2A03::C) ? "C" : "-",
        (cpu_state.status & RP2A03::Z) ? "Z" : "-",
        (cpu_state.status & RP2A03::I) ? "I" : "-",
        (cpu_state.status & RP2A03::D) ? "D" : "-",
        (cpu_state.status & RP2A03::B) ? "B" : "-",
        (cpu_state.status & RP2A03::U) ? "U" : "-",
        (cpu_state.status & RP2A03::V) ? "V" : "-",
        (cpu_state.status & RP2A03::N) ? "N" : "-");
}