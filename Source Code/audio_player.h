// audio_player.h
#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include "state.h"
#include <stdint.h>

AppState audio_player(void);
extern volatile uint8_t clock_1s;
extern uint16_t ADC_last;

#endif
