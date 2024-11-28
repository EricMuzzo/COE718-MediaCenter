/*----------------------------------------------------------------------------
 * Name:    state.h
 * Purpose: define application states for use in state machine
 * Note(s): 
 *----------------------------------------------------------------------------*/
 
 #ifndef STATE_H
 #define STATE_H
 
 typedef enum {
    MAIN_MENU_STATE,
    PHOTO_GALLERY_STATE,
    AUDIO_PLAYER_STATE,
    GAME_CENTER_STATE,
    EXIT_STATE
} AppState;
 
 #endif
