#include "PlayList.hpp"
#include "Player.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Color.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/ListBox.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include <TGUI/Widgets/Slider.hpp>

namespace fs = std::filesystem;

namespace Colors {
inline const tgui::Color Background{24, 24, 27};
inline const tgui::Color Foreground{220, 220, 220};
inline const tgui::Color Accent{88, 101, 242};
inline const tgui::Color Danger{220, 38, 38};
} // namespace Colors

int main() {
  // Initialize the playlist
  PlayList playlist;
  playlist.load("/home/"); // Check out !!!!!!!!!!!!!!!!!

  sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML music player");
  window.setVerticalSyncEnabled(true);
  tgui::Gui gui(window);
  gui.setFont("JetBrainsMono-Medium.ttf");

  // Panel
  auto panel = tgui::Panel::create({"100%", "100%"});
  panel->getRenderer()->setBackgroundColor(Colors::Background);
  gui.add(panel);

  // Initialize the player
  Player player;
  player.load(playlist, 0);

  // --- UI elements ---

  // Files list
  auto songList = tgui::ListBox::create();
  songList->setPosition(20, 140);
  songList->setSize(360, 400);
  songList->setItemHeight(28);
  songList->setTextSize(14);
  songList->setFocusable(false);
  songList->getRenderer()->setBorderColor(Colors::Accent);
  songList->getRenderer()->setBackgroundColor(Colors::Background);
  songList->getRenderer()->setTextColor(Colors::Foreground);
  songList->getRenderer()->setSelectedBackgroundColor(Colors::Accent);
  songList->getRenderer()->setSelectedTextColor(Colors::Foreground);
  // Adding entries to the list
  for (const auto &title : playlist.titles) {
    songList->addItem(title);
  }

  // Song name label
  auto label = tgui::Label::create("No track is playing");
  label->setPosition(20, 20);
  label->setTextSize(16);
  label->getRenderer()->setTextColor(Colors::Foreground);
  label->setText("No Track playing");
  window.setTitle("SFML music player");

  // Track slider guide
  auto timeLabel = tgui::Label::create();
  timeLabel->setPosition(20, 60);
  timeLabel->setTextSize(12);
  timeLabel->getRenderer()->setTextColor(Colors::Foreground);
  std::string current, duration;

  // Play Button
  auto playButton = tgui::Button::create("Play");
  playButton->setFocusable(false);
  playButton->setPosition(80, 80);
  playButton->setSize(120, 40);
  playButton->getRenderer()->setBackgroundColor(Colors::Accent);
  playButton->getRenderer()->setTextColor(Colors::Foreground);
  playButton->getRenderer()->setRoundedBorderRadius(25);

  // Next song Button
  auto nextButton = tgui::Button::create("Next");
  nextButton->setFocusable(false);
  nextButton->setPosition(200, 80);
  nextButton->setSize(60, 40);
  nextButton->getRenderer()->setBackgroundColor(Colors::Accent);
  nextButton->getRenderer()->setTextColor(Colors::Foreground);
  nextButton->getRenderer()->setRoundedBorderRadius(25);

  // Previous song Button
  auto prevButton = tgui::Button::create("Prev");
  prevButton->setFocusable(false);
  prevButton->setPosition(20, 80);
  prevButton->setSize(60, 40);
  prevButton->getRenderer()->setBackgroundColor(Colors::Accent);
  prevButton->getRenderer()->setTextColor(Colors::Foreground);
  prevButton->getRenderer()->setRoundedBorderRadius(25);

  // Mute Button
  auto muteButton = tgui::Button::create("Mute");
  muteButton->setFocusable(false);
  muteButton->setPosition(340, 80);
  muteButton->setSize(60, 40);
  muteButton->getRenderer()->setBackgroundColor(Colors::Accent);
  muteButton->getRenderer()->setTextColor(Colors::Foreground);
  muteButton->getRenderer()->setRoundedBorderRadius(25);

  // Volume control
  auto volControl = tgui::Slider::create();
  volControl->setFocusable(false);
  volControl->setPosition(420, 95);
  volControl->setSize(60, 10);
  volControl->setMaximum(100.0);
  volControl->setValue(player.volume());
  volControl->getRenderer()->setTrackColor(Colors::Background);
  volControl->getRenderer()->setThumbColor(Colors::Accent);
  volControl->getRenderer()->setTrackColorHover(Colors::Background);

  // Track slider
  auto trackSlider = tgui::Slider::create();
  trackSlider->setFocusable(false);
  trackSlider->setPosition(100, 60);
  trackSlider->setSize(350, 10);
  trackSlider->setMinimum(0.f);
  trackSlider->setMaximum(player.getDuration());
  trackSlider->setValue(player.getPlayingOffset());
  trackSlider->getRenderer()->setTrackColor(Colors::Background);
  trackSlider->getRenderer()->setThumbColor(Colors::Accent);
  trackSlider->getRenderer()->setTrackColorHover(Colors::Background);

  // Repeat mode button
  auto repeatButton = tgui::Button::create("Off");
  repeatButton->setFocusable(false);
  repeatButton->setPosition(260, 80);
  repeatButton->setSize(60, 40);
  repeatButton->getRenderer()->setBackgroundColor(Colors::Accent);
  repeatButton->getRenderer()->setTextColor(Colors::Foreground);
  repeatButton->getRenderer()->setRoundedBorderRadius(25);

  // Add to playlist button

  // Adding elements panel
  panel->add(label);
  panel->add(playButton);
  panel->add(nextButton);
  panel->add(prevButton);
  panel->add(muteButton);
  panel->add(volControl);
  panel->add(trackSlider);
  panel->add(timeLabel);
  panel->add(repeatButton);
  panel->add(songList);

  // --- Buttons functionality ---

  // Play Button
  playButton->onPress([&] {
    player.pause();
    playButton->setText(player.status() == sf::Music::Status::Playing ? "Pause"
                                                                      : "Play");
  });

  // Next song
  nextButton->onPress([&] { player.playNext(playlist); });

  // Previous song
  prevButton->onPress([&] { player.playPrevious(playlist); });

  // Volume control and mute Button
  float vol;
  muteButton->onPress([&] {
    if (!player.isMute()) {
      vol = player.volume();
      player.setVolume(0.f);
      muteButton->setText("Unmute");
    } else {
      player.setVolume(vol > 0.f ? vol : 5.f);
      muteButton->setText("Mute");
    }
    volControl->setValue(player.volume());
  });

  volControl->onValueChange([&](float value) {
    player.setVolume(value);
    if (player.isMute()) {
      muteButton->setText("Unmute");
    } else {
      muteButton->setText("Mute");
    }
    volControl->setValue(player.volume());
  });

  // Track slider
  trackSlider->onValueChange([&](float t) {
    if (trackSlider->isMouseDown()) {
      player.setPlayingOffset(t);
    }
  });

  // Repeat mode button
  int repeat = 0;
  repeatButton->onClick([&] {
    repeat++;
    if (repeat > 2) {
      repeat = 0;
    }
  });

  // Song list
  songList->onItemSelect([&](const tgui::String &) {
    int selected = songList->getSelectedItemIndex();
    if (selected < 0 || selected == player.currentIndex()) {
      return;
    }
    player.setIndex(selected);
    player.load(playlist, selected);
    player.play();
  });

  //  ____________________________ Event handeling ____________________________
  while (window.isOpen()) {
    while (auto event = window.pollEvent()) {
      gui.handleEvent(*event);

      if (event->is<sf::Event::Closed>()) {
        window.close();
        continue;
      }

      if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
        switch (key->scancode) {

        // Pausing
        case sf::Keyboard::Scancode::Space:
          player.pause();
          playButton->setText(
              player.status() == sf::Music::Status::Playing ? "Pause" : "Play");
          break;

        // Next Track
        case sf::Keyboard::Scancode::Right:
          player.playNext(playlist);
          break;

        // Previous track
        case sf::Keyboard::Scancode::Left:
          player.playPrevious(playlist);
          break;

        // Replay
        case sf::Keyboard::Scancode::R:
          repeat++;
          if (repeat > 2) {
            repeat = 0;
          }
          break;

        // Volume up
        case sf::Keyboard::Scancode::Up:
          player.setVolume(player.volume() + 5.f);
          muteButton->setText("Mute");
          volControl->setValue(player.volume());
          break;

        // Volume down
        case sf::Keyboard::Scancode::Down:
          player.setVolume(player.volume() - 5.f);
          if (player.isMute()) {
            muteButton->setText("Unmute");
          }
          volControl->setValue(player.volume());
          break;

        // Mute / Unmute
        case sf::Keyboard::Scancode::M: {
          if (!player.isMute()) {
            vol = player.volume();
            player.setVolume(0.f);
            muteButton->setText("Unmute");
          } else {
            player.setVolume(vol > 0.f ? vol : 5.f);
            muteButton->setText("Mute");
          }
          volControl->setValue(player.volume());
          break;
        }

        default:
          break;
        }
      }
    }

    // Updating Track slider
    if (player.status() == sf::Music::Status::Playing) {
      trackSlider->setValue(player.getPlayingOffset());
    }
    trackSlider->setMaximum(player.getDuration());

    /* --- Time guide ---
    Recalculate current time*/
    int t_offset = static_cast<int>(player.getPlayingOffset());
    int t_minutes = t_offset / 60;
    int t_seconds = t_offset % 60;
    // Recalculate duration time
    int d_offset = static_cast<int>(player.getDuration());
    int d_minutes = d_offset / 60;
    int d_seconds = d_offset % 60;
    // Format time strings
    current = std::to_string(t_minutes) + ":" + (t_seconds < 10 ? "0" : "") +
              std::to_string(t_seconds);
    duration = std::to_string(d_minutes) + ":" + (d_seconds < 10 ? "0" : "") +
               std::to_string(d_seconds);
    // Updating the time guide
    timeLabel->setText(current + "/" + duration);

    // Reoeat mode
    switch (repeat) {
    case 0:
      repeatButton->setText("Off");
      if (player.status() == sf::Music::Status::Stopped) {
        playButton->setText("Play");
      }
      break;
    case 1:
      repeatButton->setText("Once");
      if (player.status() == sf::Music::Status::Stopped) {
        player.play();
      }
      break;
    case 2:
      repeatButton->setText("Playlist");
      if (player.status() == sf::Music::Status::Stopped) {
        player.playNext(playlist);
      }
      break;
    default:
      break;
    }

    // Update selected item in song list
    songList->setSelectedItemByIndex(player.currentIndex());

    // Title updating
    window.setTitle("SFML music player - " +
                    playlist.titles[player.currentIndex()]);
    label->setText(playlist.titles[player.currentIndex()]);

    // Updating frames
    window.clear();
    gui.draw();
    window.display();
  }
}