#pragma once

#include <nowarn/stdint.h>

constexpr wchar_t* WINDOW_TITLE = L"Pony Game";
constexpr wchar_t* WINDOW_CLASS_NAME = L"$PonyGame$WindowClass";
constexpr uint32_t SCREEN_WIDTH_PX = 1024;
constexpr uint32_t SCREEN_HEIGHT_PX = 576;
constexpr uint16_t SPRITE_SIZE_WIDTH_PX = 64;
constexpr uint16_t SPRITE_SIZE_HEIGHT_PX = 64;
constexpr uint32_t SCREEN_WIDTH_TILES = SCREEN_WIDTH_PX / SPRITE_SIZE_WIDTH_PX;
constexpr uint32_t SCREEN_HEIGHT_TILES = SCREEN_HEIGHT_PX / SPRITE_SIZE_HEIGHT_PX;
constexpr uint16_t FRAMES_PER_SEC = 60;
constexpr uint16_t PONY_IDLE_FRAMES = 7;
constexpr uint16_t PONY_IDLE_FRAMES_PER_SEC = 4;
constexpr float PONY_IDLE_TIME_PER_FRAME_SEC = 1.f / static_cast<float>(PONY_IDLE_FRAMES_PER_SEC);
constexpr uint16_t PONY_RUNNING_FRAMES = 5;
constexpr uint16_t PONY_RUNNING_FRAMES_PER_SEC = 10;
constexpr float PONY_RUNNING_TIME_PER_FRAME_SEC = 1.f / static_cast<float>(PONY_RUNNING_FRAMES_PER_SEC);
constexpr uint16_t PONY_JUMPING_FRAMES = 5;
constexpr uint16_t PONY_JUMPING_FRAMES_PER_SEC = 10;
constexpr float PONY_JUMPING_TIME_PER_FRAME_SEC = 1.f / static_cast<float>(PONY_JUMPING_FRAMES_PER_SEC);
constexpr float PONY_JUMP_X_VELOCITY = 3.f;
constexpr float PONY_JUMP_Y_VELOCITY = -12.f;
constexpr uint16_t PONY_X_SPEED_PX = 3;
constexpr float GRAVITY = 1.f;
constexpr wchar_t* FILE_PATH_SPRITE_GRASS = L"assets/grass_tile.png";
constexpr wchar_t* FILE_PATH_SPRITE_PONY_IDLE = L"assets/pony_idle.dds";
constexpr wchar_t* FILE_PATH_SPRITE_PONY_RUNNING = L"assets/pony_running.dds";
constexpr wchar_t* FILE_PATH_SPRITE_PONY_JUMPING = L"assets/pony_jumping.dds";
constexpr bool FULLSCREEN = false;
// Integer format represents time using 10,000,000 ticks per second.
constexpr uint64_t TICKS_PER_SECOND = 10000000;
constexpr uint8_t SECONDS_PER_MINUTE = 60;