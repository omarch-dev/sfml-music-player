#pragma once
#include "PlayList.hpp"
#include <SFML/Audio.hpp>

class Player {
public:
  bool load(const PlayList &playlist, int index);
  void play();
  void pause();
  void stop();

  void playPrevious(const PlayList &platlist);

  sf::Music::Status status() const;
  float volume() const;
  void setVolume(float v);
  float getDuration();
  void setPlayingOffset(float t);
  float getPlayingOffset();
  bool isMute();
  int currentIndex() const;
  void setIndex(int selectedIndex);
  void playNext(const PlayList &playlist);

private:
  sf::Music music;
  int index = 0;
};