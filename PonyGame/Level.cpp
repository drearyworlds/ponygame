#include "Level.h"

using namespace ParticleHomeEntertainment;

Level::Level() {
}

void Level::Draw(uint32_t screen) {
    // Draw the level's current screen
    _Screens[screen]->Draw();
}