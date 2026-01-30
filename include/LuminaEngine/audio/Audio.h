#pragma once

// ============================================================================
// Audio Module - Placeholder for future implementation
// ============================================================================
// TODO: Implement audio system with SDL3_mixer or similar

namespace Lumina {

class AudioManager {
public:
    static void Init();
    static void Shutdown();
    
    // Sound effects
    static int LoadSound(const char* filename);
    static void PlaySound(int soundId, float volume = 1.0f);
    
    // Music
    static int LoadMusic(const char* filename);
    static void PlayMusic(int musicId, bool loop = true);
    static void StopMusic();
    static void SetMusicVolume(float volume);
    
    // Master volume
    static void SetMasterVolume(float volume);
};

} // namespace Lumina
