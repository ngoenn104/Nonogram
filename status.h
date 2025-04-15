#ifndef STATUS_H
#define STATUS_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>

void initStatusSounds();
void cleanupStatusSounds();
void renderStatus(SDL_Renderer* renderer, bool isWin, bool isLose, int lives);

#endif
