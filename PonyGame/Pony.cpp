#include "Pony.h"
#include "Constants.h"
#include "PonyGame.h"

using namespace ParticleHomeEntertainment;

Pony::Pony(const float startingLocationX, const float startingLocationY) : Entity(startingLocationX, startingLocationY) {
}

Pony::~Pony() { }

void Pony::UpdateStates() {
    Move(PONY_X_SPEED_PX, 0);
}