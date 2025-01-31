#include <Core/AudioManager.h>
#include <iostream>

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
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filePath))
    {
        std::cerr << "Error: Could not load sound effect from " << filePath << std::endl;
        return false;
    }

    m_soundBuffers[type] = buffer;
    m_sounds[type].setBuffer(m_soundBuffers[type]);
    return true;
}

void AudioManager::playMusic(MusicType type, bool loop)
{
    stopCurrentMusic();
    auto it = m_musics.find(type);
    if (it != m_musics.end()) {
        m_currentMusicType = type;
        it->second->setLoop(loop);
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

void AudioManager::loadAllSounds() {

    s_instance->loadMusic(MusicType::MainMenu, "../data/Sound/MainMenu.wav");
    s_instance->loadMusic(MusicType::Game, "../data/Sound/Game.wav");
    s_instance->loadSoundEffect(SoundType::ButtonClick, "../data/Sound/ButtonClick.wav");
    s_instance->loadSoundEffect(SoundType::Jump, "../data/Sound/Jump.wav");
}
