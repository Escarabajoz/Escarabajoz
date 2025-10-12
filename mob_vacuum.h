#pragma once

#include <cstdint>
#include <immintrin.h>
#include <string>

/**
 * @brief Mob Vacuum Feature - Game modification for entity management
 * 
 * This class provides functionality for automatically collecting nearby entities
 * with configurable range and distance parameters.
 */
class MobVacuum {
public:
    /**
     * @brief Input types for the vacuum system
     */
    enum class InputType {
        DISABLED = 0,
        ENABLED = 1
    };

    /**
     * @brief Status of the vacuum system
     */
    enum class Status {
        INACTIVE = 0,
        ACTIVE = 1
    };

    /**
     * @brief Configuration structure for the mob vacuum
     */
    struct Config {
        bool enabled = false;           // Enable/disable flag
        InputType input_type = InputType::DISABLED;
        float range = 100.0f;          // Detection range
        float distance = 50.0f;        // Action distance
        Status status = Status::INACTIVE;
    };

private:
    Config m_config;
    
    // UI and rendering functions
    void SetupUI();
    void RenderUI();
    void UpdateStatusDisplay();
    
    // Core vacuum logic
    bool IsValidTarget(uint32_t entity_type, uint32_t input_type) const;
    void ProcessVacuumLogic();
    
    // Utility functions
    void LogError(int level, const char* message) const;
    void LogInfo(int level, const char* message) const;

public:
    MobVacuum();
    ~MobVacuum() = default;

    /**
     * @brief Main render function for the UI
     * @return Success status
     */
    int64_t Render();
    
    /**
     * @brief Initialize the vacuum system
     * @return Success status
     */
    int64_t Initialize();
    
    /**
     * @brief Update function called each frame
     * @return Success status
     */
    int64_t Update();

    /**
     * @brief Get current configuration
     * @return Reference to current config
     */
    const Config& GetConfig() const { return m_config; }
    
    /**
     * @brief Set vacuum enabled state
     * @param enabled New enabled state
     */
    void SetEnabled(bool enabled) { m_config.enabled = enabled; }
    
    /**
     * @brief Set vacuum range
     * @param range New range value
     */
    void SetRange(float range) { m_config.range = range; }
    
    /**
     * @brief Set vacuum distance
     * @param distance New distance value
     */
    void SetDistance(float distance) { m_config.distance = distance; }
};

// Global configuration data (equivalent to original memory locations)
extern alignas(16) uint8_t g_mob_vacuum_config[16];    // xmmword_180062150
extern alignas(16) uint8_t g_mob_vacuum_settings[16];  // xmmword_180062160

// Color/Style constants (equivalent to xmmword_180058E30/E20)
extern alignas(16) __m128i g_active_color;
extern alignas(16) __m128i g_inactive_color;