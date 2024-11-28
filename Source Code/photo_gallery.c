/*----------------------------------------------------------------------------
 * Name:    photo_gallery.c
 * Purpose: manage the photo gallery application
 * Note(s): 
 *----------------------------------------------------------------------------*/
                  
#include <LPC17xx.H>                    /* NXP LPC17xx definitions            */
#include "string.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"
#include "state.h"
#include "falcon9.h"
#include "starship.h"
#include "saturnv.h"
#include "arrow.c"
#include "cmsis_os.h"

#define __FI        1                      /* Font index 16x24               */
#define USE_LCD		0

unsigned char* images[] = {STARSHIP_PIXEL_DATA, FALCON9_PIXEL_DATA, SATURNV_PIXEL_DATA};

/*----------------------------------------------------------------------------
  Initialize the display for the photo viewer application
 *----------------------------------------------------------------------------*/
void init_display(){
    
    #ifdef USE_LCD
		GLCD_Clear(White);
        GLCD_SetBackColor(Blue);
    
        GLCD_SetTextColor(Red);
    
        GLCD_Bitmap(0, 0, 16, 16, ARROW_PIXEL_DATA);
        GLCD_DisplayString(0, 3, __FI, (unsigned char *)"Photo Gallery");
    
    
        GLCD_SetBackColor(White);
        GLCD_SetTextColor(Black);
        GLCD_DisplayString(3, 0, __FI, (unsigned char *)"Starship");        //Selection 1
        GLCD_DisplayString(4, 0, __FI, (unsigned char *)"Falcon 9");        //Selection 2
        GLCD_DisplayString(5, 0, __FI, (unsigned char *)"Saturn V");        //Selection 3
    
    #endif
}

/*----------------------------------------------------------------------------
  Update the LCD based on selected item
 *----------------------------------------------------------------------------*/
void update_display(int selected){
    
    switch(selected){
        
        //Selected = 0; hovered on the back button
        case 0:
            GLCD_SetBackColor(White);
            
            GLCD_DisplayString(3, 0, __FI, (unsigned char *)"Starship");
            GLCD_DisplayString(4, 0, __FI, (unsigned char *)"Falcon 9");
            GLCD_DisplayString(5, 0, __FI, (unsigned char *)"Saturn V");
			break;
        
        //Selected = 1
        case 1:
            GLCD_SetBackColor(LightGrey);
            GLCD_DisplayString(3, 0, __FI, (unsigned char *)"Starship");
            
            GLCD_SetBackColor(White);
            GLCD_DisplayString(4, 0, __FI, (unsigned char *)"Falcon 9");
            GLCD_DisplayString(5, 0, __FI, (unsigned char *)"Saturn V");
			break;
        
        //Selected = 2
        case 2:
            GLCD_SetBackColor(LightGrey);
            GLCD_DisplayString(4, 0, __FI, (unsigned char *)"Falcon 9");
            
            GLCD_SetBackColor(White);
            GLCD_DisplayString(3, 0, __FI, (unsigned char *)"Starship");
            GLCD_DisplayString(5, 0, __FI, (unsigned char *)"Saturn V");
			break;
            
        //Selected = 3
        case 3:
            GLCD_SetBackColor(LightGrey);
            GLCD_DisplayString(5, 0, __FI, (unsigned char *)"Saturn V");
            
            GLCD_SetBackColor(White);
            GLCD_DisplayString(3, 0, __FI, (unsigned char *)"Starship");
            GLCD_DisplayString(4, 0, __FI, (unsigned char *)"Falcon 9");
			break;
    }
}

/*----------------------------------------------------------------------------
  Preview's the selected image file
 *----------------------------------------------------------------------------*/
void image_preview(int selected_image){
    
	int joystick_val;
    int index = selected_image - 1;
    
    GLCD_Clear(White);
    GLCD_Bitmap(0, 0, 320, 240, images[index]);
    
    //Wait for left joystick to be pressed to return to photo menu
    for(;;){
		joystick_val = get_button();
		if(joystick_val == KBD_LEFT){
			init_display();					//re-initialize display
			break;
		}
	}
}

AppState photo_gallery(){

    int joystick_val;
    int selected = 1;
    
    init_display();
	
	for(;;){
		update_display(selected);
		joystick_val = get_button();
		
		switch(joystick_val){
			case KBD_SELECT:
				if(selected == 0){
					return MAIN_MENU_STATE;
				}else{
					image_preview(selected);
				}
				break;
			case KBD_UP:
				if(selected > 1){
					selected--;
				}else{
					selected = 3;
				}
				break;
			case KBD_DOWN:
				if(selected < 3){
					selected++;
				}else{
					selected = 1;
				}
				break;
			case KBD_LEFT:
				selected = 0;
				break;
		}
		osDelay(1000);
	}
}
