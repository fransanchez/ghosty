#pragma once

#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>

enum class MusicType
{
    MainMenu,
    Game,
    GameOver,
    Victory
};

enum class SoundType
{
    ButtonClick,
    Jump,
};

class AudioManager
{
    public:
        static AudioManager* getInstance();

        bool loadMusic(MusicType type, const std::string& filePath);
        bool loadSoundEffect(SoundType type, const std::string& filePath);

        void playMusic(MusicType type, bool loop = true);
        void stopCurrentMusic();
        void playSoundEffect(SoundType type);
        void stopAllSounds();
        void clear();
        void loadAllSounds();

    private:
        AudioManager() = default;
        ~AudioManager();

        MusicType m_currentMusicType = MusicType::MainMenu;
        std::unordered_map<MusicType, sf::Music*> m_musics;
        std::unordered_map<SoundType, sf::SoundBuffer> m_soundBuffers;
        std::unordered_map<SoundType, sf::Sound> m_sounds;

        static AudioManager* s_instance;
};
