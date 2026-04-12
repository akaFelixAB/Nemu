#include "Debugger.h"

Debugger::Debugger(Bus* bus,
                   const std::span<const RP2A03::INSTRUCTION, 256> lookup)
    : disasm(bus, lookup) {}

void Debugger::draw_window(const char* title) {
    Sizes s;
    s.ContentHeight = ImGui::GetContentRegionAvail().y;
    s.WindowWidth = ImGui::GetWindowWidth();
    s.ContentWidth = ImGui::GetContentRegionAvail().x;
    // calc_sizes(mem_size, base_display_addr);
    ImGui::SetNextWindowSize(ImVec2(s.WindowWidth, s.WindowWidth * 0.60f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, 0.0f), ImVec2(s.WindowWidth, FLT_MAX));

    open = true;
    if (ImGui::Begin(title, &open, ImGuiWindowFlags_NoScrollbar)) {
        std::string memory_layout = disasm.get_memory_layout(0, 256);
        ImGui::TextUnformatted(memory_layout.c_str());
    }
    ImGui::End();
}

void Debugger::draw_memory(uint16_t mem_size, uint16_t base_display_addr) {
    ImGui::BeginChild("Memory", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);
    
    ImGui::EndChild();
}