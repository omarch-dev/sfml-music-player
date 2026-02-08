#include "Player.hpp"
#include <algorithm>

bool Player::load(const PlayList &playlist, int i) {
  if (i < 0 || i > static_cast<int>(playlist.size())) {
    return false;
  }

  index = i;
  return music.openFromFile(playlist.files[index]);
}

void Player::play() { music.play(); }

void Player::pause() {
  if (music.getStatus() == sf::Music::Status::Paused ||
      music.getStatus() == sf::Music::Status::Stopped) {
    music.play();
  } else {
    music.pause();
  }
}

void Player::stop() { music.stop(); }

void Player::playNext(const PlayList &playlist) {
  index++;
  if (index >= static_cast<int>(playlist.size())) {
    index = 0;
  }
  Player::load(playlist, index);
  Player::stop();
  Player::play();
}

void Player::playPrevious(const PlayList &playlist) {
  index--;
  if (index < 0) {
    index = playlist.size() - 1;
  }
  Player::load(playlist, index);
  Player::stop();
  Player::play();
}

float Player::getPlayingOffset() {
  return music.getPlayingOffset().asSeconds();
}

bool Player::isMute() { return music.getVolume() == 0.f; }

void Player::setPlayingOffset(float t) {
  music.setPlayingOffset(sf::seconds(t));
}

void Player::setIndex(int selectedIndex) { index = selectedIndex; }

float Player::getDuration() { return music.getDuration().asSeconds(); }

sf::Music::Status Player::status() const { return music.getStatus(); }

float Player::volume() const { return music.getVolume(); }

void Player::setVolume(float v) { music.setVolume(std::clamp(v, 0.f, 100.f)); }

int Player::currentIndex() const { return index; }