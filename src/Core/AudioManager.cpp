#include <Core/AudioManager.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <unordered_map>

AudioManager* AudioManager::s_instance{ nullptr };

AudioManager* AudioManager::getInstance()
{
    if (s_instance == nullptr)
    {
        s_instance = new AudioManager();
    }
    return s_instance;
}

AudioManager::~AudioManager()
{
    clear();
}

void AudioManager::clear() {
    stopCurrentMusic();
    stopAllSounds();

    for (auto& [type, music] : m_musics)
    {
        delete music;
        music = nullptr;
    }
    m_musics.clear();

    m_sounds.clear();
    m_soundBuffers.clear();
    m_loadedSoundPaths.clear();
}

bool AudioManager::loadMusic(MusicType type, const std::string& filePath)
{
    sf::Music* music = new sf::Music();
    if (!music->openFromFile(filePath))
    {
        std::cerr << "Error: Could not load music from " << filePath << std::endl;
        return false;
    }
    m_currentMusicType = type;
    m_musics[m_currentMusicType] = music;
    return true;
}

bool AudioManager::loadSoundEffect(SoundType type, const std::string& filePath)
{
    if (m_loadedSoundPaths.find(type) != m_loadedSoundPaths.end() && m_loadedSoundPaths[type] == filePath)
    {
        return true;
    }

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filePath))
    {
        std::cerr << "Error: Could not load sound effect from " << filePath << std::endl;
        return false;
    }

    m_soundBuffers[type] = buffer;
    m_sounds[type].setBuffer(m_soundBuffers[type]);
    m_loadedSoundPaths[type] = filePath;

    return true;
}

void AudioManager::playMusic(MusicType type, bool loop, float volume)
{
    stopCurrentMusic();
    auto it = m_musics.find(type);
    if (it != m_musics.end()) {
        m_currentMusicType = type;
        it->second->setLoop(loop);
        it->second->setVolume(volume);
        it->second->play();
    }
    else {
        std::cerr << "Error: Music type not found!" << std::endl;
    }
}

void AudioManager::stopCurrentMusic()
{
    m_musics[m_currentMusicType]->stop();
}

void AudioManager::playSoundEffect(SoundType type)
{
    auto it = m_sounds.find(type);
    if (it != m_sounds.end())
    {
        it->second.setVolume(50.f);
        it->second.play();
    }
    else
    {
        std::cerr << "Error: Sound effect not found!" << std::endl;
    }
}

void AudioManager::stopAllSounds()
{
    for (auto& [type, sound] : m_sounds)
    {
        sound.stop();
    }
}

bool AudioManager::loadAudioConfig(const std::string& configFilePath) {
    std::ifstream file(configFilePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open config file " << configFilePath << std::endl;
        return false;
    }

    nlohmann::json config;
    file >> config;

    const auto& musicConfig = config["music"];
    for (auto& [key, value] : musicConfig.items()) {
        MusicType musicType = parseMusicType(key);
        loadMusic(musicType, value.get<std::string>());
    }

    const auto& soundConfig = config["sounds"];
    for (auto& [key, value] : soundConfig.items()) {
        SoundType soundType = parseSoundType(key);
        loadSoundEffect(soundType, value.get<std::string>());
    }

    return true;
}

