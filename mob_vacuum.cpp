#include "mob_vacuum.h"
#include <cstring>
#include <stdexcept>

// Global configuration data (equivalent to original memory locations)
alignas(16) uint8_t g_mob_vacuum_config[16] = {0};    // xmmword_180062150
alignas(16) uint8_t g_mob_vacuum_settings[16] = {0};  // xmmword_180062160

// Color/Style constants (equivalent to xmmword_180058E30/E20)
alignas(16) __m128i g_active_color = {0};     // Will be loaded with actual colors
alignas(16) __m128i g_inactive_color = {0};   // Will be loaded with actual colors

// Forward declarations for external functions (these would need to be implemented)
extern void SetWindowTitle(const char* title);
extern void CreateToggleButton(const char* label, bool* value_ptr);
extern void BeginFrame();
extern void EndRendering();
extern void CreateInputSelector(const char* label, int* current_item, int register_val, void* item_list);
extern bool CheckTargetValid(uint32_t entity_type, uint32_t input_type);
extern void SetTextColor(__m128i color);
extern void DisplayText(__m128i* color, const char* format, ...);
extern void CreateSlider(const char* label, int data_type, float* value_ptr, int* min_val, int64_t* max_val, const char* format);
extern int64_t EndUIFrame();
extern void LogMessage(int level, const char* message);

MobVacuum::MobVacuum() {
    // Initialize default configuration
    m_config.enabled = false;
    m_config.input_type = InputType::DISABLED;
    m_config.range = 100.0f;
    m_config.distance = 50.0f;
    m_config.status = Status::INACTIVE;
}

int64_t MobVacuum::Render() {
    try {
        // Set up the main UI window
        SetWindowTitle("Mob Vacuum");
        
        // Create the enable/disable toggle button
        // This maps to the original BYTE4(xmmword_180062150)
        bool* enabled_ptr = reinterpret_cast<bool*>(&g_mob_vacuum_config[4]);
        CreateToggleButton("Enable Mob Vacuum", enabled_ptr);
        m_config.enabled = *enabled_ptr;
        
        // Begin UI frame
        BeginFrame();
        
        // Set up additional UI controls based on config offset +8
        uint32_t* config_offset8_ptr = reinterpret_cast<uint32_t*>(&g_mob_vacuum_config[8]);
        *config_offset8_ptr = static_cast<uint32_t>(m_config.input_type);
        
        // Only show extended controls if vacuum is enabled
        if (m_config.enabled) {
            // Input type selector
            // Maps to original sub_180035D50 call
            int* input_type_ptr = reinterpret_cast<int*>(&g_mob_vacuum_settings[16]);
            CreateInputSelector("Input Type", input_type_ptr, 0, nullptr);
            m_config.input_type = static_cast<InputType>(*input_type_ptr);
            
            // Determine if vacuum should be active based on conditions
            bool is_active = false;
            __m128i status_color;
            
            if (m_config.enabled && CheckTargetValid(
                *reinterpret_cast<uint32_t*>(&g_mob_vacuum_config[8]),
                static_cast<uint32_t>(*input_type_ptr))) {
                
                is_active = true;
                status_color = _mm_load_si128(&g_active_color);
                m_config.status = Status::ACTIVE;
            } else {
                is_active = false;
                status_color = _mm_load_si128(&g_inactive_color);
                m_config.status = Status::INACTIVE;
            }
            
            // Display status
            const char* status_text = is_active ? "Active" : "Inactive";
            DisplayText(&status_color, "Status: %s", status_text);
            
            // Range slider (maps to original float value 1176256512 = 900.0f in hex)
            int64_t range_max = 3212836864LL;  // Original constant from decompiled code
            int range_min = 0;
            float* range_ptr = reinterpret_cast<float*>(&g_mob_vacuum_config[12]);
            CreateSlider("Range", 8, range_ptr, &range_min, &range_max, "%.1f");
            m_config.range = *range_ptr;
            
            // Distance slider (maps to original float value 1140457472 = 100.0f in hex)
            int64_t distance_max = 1140457472;  // Original constant
            int distance_min = 0;
            float* distance_ptr = reinterpret_cast<float*>(&g_mob_vacuum_settings[0]);
            CreateSlider("Distance", 8, distance_ptr, &distance_min, &distance_max, "%.1f");
            m_config.distance = *distance_ptr;
        }
        
        // End the UI frame and return result
        return EndUIFrame();
        
    } catch (...) {
        // Handle any exceptions during rendering
        LogMessage(3, "Mob Vacuum Render failed");
        return 0;
    }
}

int64_t MobVacuum::Initialize() {
    try {
        LogMessage(0, "Mob Vacuum started safely");
        return 1;
    } catch (...) {
        LogMessage(3, "Mob Vacuum initialization failed");
        return 0;
    }
}

int64_t MobVacuum::Update() {
    try {
        if (m_config.enabled && m_config.status == Status::ACTIVE) {
            ProcessVacuumLogic();
        }
        return 1;
    } catch (...) {
        LogMessage(3, "Mob Vacuum Update failed");
        return 0;
    }
}

// Private helper methods
void MobVacuum::SetupUI() {
    // Initialize UI colors and styles
    // This would load the actual color values into g_active_color and g_inactive_color
}

void MobVacuum::RenderUI() {
    // Additional UI rendering logic if needed
}

void MobVacuum::UpdateStatusDisplay() {
    // Update the status display based on current configuration
}

bool MobVacuum::IsValidTarget(uint32_t entity_type, uint32_t input_type) const {
    // This maps to the original sub_180044FD0 function call
    return CheckTargetValid(entity_type, input_type);
}

void MobVacuum::ProcessVacuumLogic() {
    // Core vacuum logic would be implemented here
    // This would handle the actual entity collection based on range and distance
}

void MobVacuum::LogError(int level, const char* message) const {
    LogMessage(level, message);
}

void MobVacuum::LogInfo(int level, const char* message) const {
    LogMessage(level, message);
}

// Wrapper functions that maintain compatibility with original decompiled function signatures
extern "C" {
    int64_t sub_180047570() {
        static MobVacuum vacuum_instance;
        return vacuum_instance.Render();
    }
    
    int64_t sub_180047E90() {
        static MobVacuum vacuum_instance;
        return vacuum_instance.Initialize();
    }
    
    int64_t sub_18004E436() {
        LogMessage(3, "Mob Vacuum Update failed");
        return 0;
    }
    
    int64_t sub_18004E2A0() {
        LogMessage(3, "Mob Vacuum Render failed");
        return 0;
    }
}