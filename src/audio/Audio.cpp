#include "LuminaEngine/audio/Audio.h"
#include "LuminaEngine/utils/Logger.h"

// ============================================================================
// Audio Module - Placeholder Implementation
// ============================================================================
// TODO: Implement with SDL3_mixer

namespace Lumina {

void AudioManager::Init() {
    LUMINA_LOG_INFO("AudioManager: Not yet implemented");
}

void AudioManager::Shutdown() {
    // Cleanup
}

int AudioManager::LoadSound(const char* /*filename*/) {
    return -1; // Not implemented
}

void AudioManager::PlaySound(int /*soundId*/, float /*volume*/) {
    // Not implemented
}

int AudioManager::LoadMusic(const char* /*filename*/) {
    return -1; // Not implemented
}

void AudioManager::PlayMusic(int /*musicId*/, bool /*loop*/) {
    // Not implemented
}

void AudioManager::StopMusic() {
    // Not implemented
}

void AudioManager::SetMusicVolume(float /*volume*/) {
    // Not implemented
}

void AudioManager::SetMasterVolume(float /*volume*/) {
    // Not implemented
}

} // namespace Lumina
