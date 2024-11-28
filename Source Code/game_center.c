/*----------------------------------------------------------------------------
 * Name:    game_center.c
 * Purpose: manage the game center application
 * Note(s): 
 *----------------------------------------------------------------------------*/
                  
#include <LPC17xx.H>                    /* NXP LPC17xx definitions            */
#include "string.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"
#include "state.h"
#include "tictactoe.h"
#include "arrow.c"
#include "cmsis_os.h"

#define __FI        1                      /* Font index 16x24               */
#define USE_LCD		0


/*----------------------------------------------------------------------------
  Initialize the display for the game center main menu
 *----------------------------------------------------------------------------*/
void init_gc_display(){
    
    #ifdef USE_LCD
		GLCD_Clear(White);
        GLCD_SetBackColor(Blue);
    
        GLCD_SetTextColor(Red);
    
        GLCD_Bitmap(0, 0, 16, 16, ARROW_PIXEL_DATA);
        GLCD_DisplayString(0, 3, __FI, (unsigned char *)"Game Center");
    
    
        GLCD_SetBackColor(White);
        GLCD_SetTextColor(Black);
        GLCD_DisplayString(3, 0, __FI, (unsigned char *)"    Tic Tac Toe     ");    //Selection 1
    
        //Use these if you have more than one game
        //GLCD_DisplayString(4, 0, __FI, (unsigned char *)"Game 2");                //Selection 2
        //GLCD_DisplayString(5, 0, __FI, (unsigned char *)"Game 3");                //Selection 3
    
    #endif
}

/*----------------------------------------------------------------------------
  Update the LCD based on selected item
 *----------------------------------------------------------------------------*/
void update_gc_display(int selected){
    
    switch(selected){
        
        //Selected = 0; hovered on the back button
        case 0:
            GLCD_SetBackColor(White);
            
            GLCD_DisplayString(3, 0, __FI, (unsigned char *)"    Tic Tac Toe     ");
            //GLCD_DisplayString(4, 0, __FI, (unsigned char *)"Game 2");
            //GLCD_DisplayString(5, 0, __FI, (unsigned char *)"Game 3");
            break;
        
        //Selected = 1
        case 1:
            GLCD_SetBackColor(LightGrey);
            GLCD_DisplayString(3, 0, __FI, (unsigned char *)"    Tic Tac Toe     ");
            
            GLCD_SetBackColor(White);
            //GLCD_DisplayString(4, 0, __FI, (unsigned char *)"Game 2");
            //GLCD_DisplayString(5, 0, __FI, (unsigned char *)"Game 3");
            break;
        
        /*
        Left here in case more games are added
        
        //Selected = 2
        case 2:
            GLCD_SetBackColor(LightGrey);
            GLCD_DisplayString(4, 0, __FI, (unsigned char *)"Falcon 9");
            
            GLCD_SetBackColor(White);
            GLCD_DisplayString(3, 0, __FI, (unsigned char *)"Starship");
            GLCD_DisplayString(5, 0, __FI, (unsigned char *)"Saturn V");
            
        //Selected = 3
        case 3:
            GLCD_SetBackColor(LightGrey);
            GLCD_DisplayString(5, 0, __FI, (unsigned char *)"Saturn V");
            
            GLCD_SetBackColor(White);
            GLCD_DisplayString(3, 0, __FI, (unsigned char *)"Starship");
            GLCD_DisplayString(4, 0, __FI, (unsigned char *)"Falcon 9");
        */
    }
}

AppState game_center(void){
    
    int joystick_val;                   //The joystick value
    int selected = 1;                   //The menu item selected
    
    init_gc_display();
  
    for(;;){
        update_gc_display(selected);
        joystick_val = get_button();
        
        switch(joystick_val){
            case KBD_SELECT:
                if(selected == 0){
                    return MAIN_MENU_STATE;
                }else{
                    tictactoe_main();
                    init_gc_display();
                    osDelay(10000);
                }
                break;
            case KBD_DOWN:
                selected = 1;
                break;
            case KBD_LEFT:
                selected = 0;
                break;
        }
        osDelay(1000);
    }
}
