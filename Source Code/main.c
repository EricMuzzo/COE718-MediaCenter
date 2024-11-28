/*----------------------------------------------------------------------------
 * Name:    main.c
 * Purpose: manage the application state
 * Note(s): 
 *----------------------------------------------------------------------------*/
                  
#include <LPC17xx.H>                    /* NXP LPC17xx definitions            */
#include "string.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"
#include "state.h"
#include "menu.h"
#include "photo_gallery.h"
#include "audio_player.h"
#include "game_center.h"

#define __FI        1                      /* Font index 16x24               */
#define USE_LCD			0

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
    
    AppState state = MAIN_MENU_STATE;       //initialize the state to main menu
    
    GLCD_Init();
    KBD_Init();
    
    #ifdef USE_LCD
        GLCD_Clear(White);
    #endif

    while(state != EXIT_STATE){
        
        switch(state){
            
            case MAIN_MENU_STATE:
                state = main_menu();
                break;
            
            case PHOTO_GALLERY_STATE:
                state = photo_gallery();
                break;
            
            case AUDIO_PLAYER_STATE:
                state = audio_player();
                break;
            
            case GAME_CENTER_STATE:
                state = game_center();
                break;
            
            default:
                state = EXIT_STATE;
                break;
        }
    }
    return 0;
}
