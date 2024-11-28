/*----------------------------------------------------------------------------
 * Name:    menu.c
 * Purpose: manage the application state
 * Note(s): 
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <LPC17xx.H>                    /* NXP LPC17xx definitions            */
#include "string.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"
#include "state.h"
#include "cmsis_os.h"

#define __FI        1                      /* Font index 16x24               */
#define USE_LCD			0

//ITM Stimulus Port definitions for printf //////////////////
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

struct __FILE { int handle;  };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}


/*----------------------------------------------------------------------------
  Main Menu Display Function
  Updates the LCD to indicate the highlighted selection by the user
 *----------------------------------------------------------------------------*/
void updateMainMenuDisplay(int selected){

    switch(selected){
        
        //Selected = 1; Cursor hovered on Photo gallery
        case 1:
            GLCD_SetBackColor(LightGrey);
            GLCD_DisplayString(4, 0, __FI, (unsigned char *)"   Photo Gallery   ");
            
            GLCD_SetBackColor(White);
            GLCD_DisplayString(5, 0, __FI, (unsigned char *)"   Audio Player    ");
            GLCD_DisplayString(6, 0, __FI, (unsigned char *)"    Game Center    ");
			break;
        
        //Selected = 2; Cursor hovered on audio player
        case 2:
            GLCD_SetBackColor(LightGrey);
            GLCD_DisplayString(5, 0, __FI, (unsigned char *)"   Audio Player    ");
            
            GLCD_SetBackColor(White);
            GLCD_DisplayString(4, 0, __FI, (unsigned char *)"   Photo Gallery   ");
            GLCD_DisplayString(6, 0, __FI, (unsigned char *)"    Game Center    ");
			break;
            
        //Selected = 3; Cursor hovered on Photo gallery
        case 3:
            GLCD_SetBackColor(LightGrey);
            GLCD_DisplayString(6, 0, __FI, "    Game Center    ");
            
            GLCD_SetBackColor(White);
            GLCD_DisplayString(4, 0, __FI, "   Photo Gallery   ");
            GLCD_DisplayString(5, 0, __FI, "   Audio Player    ");
			break;
    }

}

/*----------------------------------------------------------------------------
  Main Menu Function
 *----------------------------------------------------------------------------*/
AppState main_menu(){
    
    int joystick_val;
    int selected_option = 1;                   //The menu option currently selected
    
    
    //Setup main menu display
    #ifdef USE_LCD

		KBD_Init();
		GLCD_Clear(White);
        GLCD_SetBackColor(Blue);
    
        GLCD_SetTextColor(Red);
        GLCD_DisplayString(0, 0, __FI, (unsigned char *)"   Media Center   ");
    
        GLCD_SetTextColor(Black);
        GLCD_DisplayString(1, 0, __FI, (unsigned char *)"     Main Menu    ");
    
        GLCD_SetBackColor(White);
        GLCD_SetTextColor(Black);
        GLCD_DisplayString(4, 0, __FI, (unsigned char *)"   Photo Gallery  ");

        GLCD_DisplayString(5, 0, __FI, (unsigned char *)"   Audio Player   ");
    
        GLCD_DisplayString(6, 0, __FI, (unsigned char *)"   Game Center    ");

    #endif
    
    for(;;){
		updateMainMenuDisplay(selected_option);
		
		joystick_val = get_button();
		
		switch(joystick_val){
			case KBD_SELECT:
				switch(selected_option){
					case 1:
						//photo gallery
						return PHOTO_GALLERY_STATE;
					case 2:
						//audio player
						return AUDIO_PLAYER_STATE;
					case 3:
						//game center
						return GAME_CENTER_STATE;
				}
				break;
			case KBD_UP:
				if(selected_option > 1){
					selected_option--;
				}else{
					selected_option = 3;
				}
				break;
			case KBD_DOWN:
				if(selected_option < 3){
					selected_option++;
				}else{
					selected_option = 1;
				}
				break;
		}
		osDelay(1000);
	}
}
