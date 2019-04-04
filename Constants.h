#pragma once

#include <stdint.h>

constexpr wchar_t* WINDOW_TITLE = L"Pony Game";
constexpr wchar_t* WINDOW_CLASS_NAME = L"$PonyGame$WindowClass";
constexpr uint32_t SCREEN_WIDTH_PX = 1024;
constexpr uint32_t SCREEN_HEIGHT_PX = 576;
constexpr uint16_t SPRITE_SIZE_WIDTH = 64;
constexpr uint16_t SPRITE_SIZE_HEIGHT = 64;
constexpr uint32_t SCREEN_WIDTH_TILES = SCREEN_WIDTH_PX / SPRITE_SIZE_WIDTH;
constexpr uint32_t SCREEN_HEIGHT_TILES = SCREEN_HEIGHT_PX / SPRITE_SIZE_HEIGHT;
constexpr uint16_t FRAMES_PER_SEC = 60;
constexpr uint16_t PONY_IDLE_FRAMES = 7;
constexpr uint16_t PONY_IDLE_FRAMES_PER_SEC = 4;
constexpr uint16_t PONY_RUNNING_FRAMES = 5;
constexpr uint16_t PONY_RUNNING_FRAMES_PER_SEC = 10;
constexpr uint16_t PONY_SPEED = 2;
constexpr wchar_t* FILE_PATH_SPRITE_PONY_IDLE = L"assets/pony_idle.dds";
constexpr wchar_t* FILE_PATH_SPRITE_PONY_RUNNING = L"assets/pony_running.dds";
constexpr bool FULLSCREEN = false;