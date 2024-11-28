/*----------------------------------------------------------------------------
 * Name:    tictactoe.c
 * Purpose: Houses the game logic for tic tac toe
 * Note(s): 
 *----------------------------------------------------------------------------*/
                  
#include <LPC17xx.H>                    /* NXP LPC17xx definitions            */
#include "string.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"
#include "state.h"
#include <stdbool.h>
#include "cmsis_os.h"

#include "tictactoe_o.h"
#include "tictactoe_x.h"

#define __FI        1                      /* Font index 16x24               */
#define USE_LCD			0

/*----------------------------------------------------------------------------
 *                          Game Variables
 *----------------------------------------------------------------------------*/
#define GRID_ROWS   3
#define GRID_COLS   3

char board[GRID_ROWS][GRID_COLS];           //Game board
char current_player;                  		//Current player X or O
int selected_row;                           //Selected row on the game board
int selected_col;                           //Selected column on the game board
bool game_over;                             //True = game over, False = game in progress


/*----------------------------------------------------------------------------
 *                          Game Setup
 *  Setup the board
 *----------------------------------------------------------------------------*/
 
//  Setup the game board
void init_board(){
    
    int i, j;
    
    for (i = 0; i < GRID_ROWS; i++) {
        for (j = 0; j < GRID_COLS; j++) {
            board[i][j] = ' ';
        }
    }
}

//  Draw the game grid
void draw_grid(){
    
    int x, y;
    
    GLCD_Clear(Black);
    GLCD_SetBackColor(Black);
    GLCD_SetTextColor(Red);
    
    //Draw vertical lines at x = 106 & x = 213 (Screen width 320/3 ~= every 106 pixels)
    for(y = 0; y < 240; y++){
        
        //Will draw pixels on the left and right of x=106&213 to give the lines some thickness
        GLCD_PutPixel(105, y);
        GLCD_PutPixel(106, y);
        GLCD_PutPixel(107, y);
        
        GLCD_PutPixel(212, y);
        GLCD_PutPixel(213, y);
        GLCD_PutPixel(214, y);
    }
    
    //Draw horizontal lines at y = 80 & y = 160 (Screen height 240/3 ~= every 80 pixels)
    for(x = 0; x < 320; x++){
        
        GLCD_PutPixel(x, 79);
        GLCD_PutPixel(x, 80);
        GLCD_PutPixel(x, 81);
        
        GLCD_PutPixel(x, 159);
        GLCD_PutPixel(x, 160);
        GLCD_PutPixel(x, 161);
    }
    
    /*
    //Alternative visual: make the gameboard square (length of horizontal & vertical lines the same)
    for(int y = 0; y < 240; y++){
        GLCD_PutPixel(106, y);
        GLCD_PutPixel(213, y);
    }
    
    //Draw horizontal lines at y = 80 & y = 160 (Screen height 240/3 ~= every 80 pixels)
    for(int x = 80; x < 240; x++){
        GLCD_PutPixel(x, 80);
        GLCD_PutPixel(x, 160);
    }
    */
}

/*----------------------------------------------------------------------------
 *                          Auxillary LCD functions
 *----------------------------------------------------------------------------*/

//Initialize tic tac toe main menu display
void init_ttt_lcd(){

    GLCD_Clear(Black);
    GLCD_SetBackColor(Black);
    GLCD_SetTextColor(White);
    
    GLCD_DisplayString(2, 0, __FI, (unsigned char *)"    Tic Tac Toe     ");
    GLCD_DisplayString(4, 0, __FI, (unsigned char *)"Push SELECT To Start");
    GLCD_DisplayString(6, 0, __FI, (unsigned char *)"Push LEFT To Go Back");
}

//  Highlights the cell that the cursor is on by drawing a box around the cell
void highlight_cell(int row, int col){
    
    int x, y;
    
    int start_x = col * 106;
    int end_x = start_x + 106;
    int start_y = row * 80;
    int end_y = start_y + 80;
    
    GLCD_SetTextColor(Green);
    for(x = start_x; x < end_x; x++){
        GLCD_PutPixel(x, start_y);              //Simultaneously highlight top and bottom edges
        GLCD_PutPixel(x, end_y);
    }
    
    for(y = start_y; y < end_y; y++){
        GLCD_PutPixel(start_x, y);              //Simultaneously highlight left and right edges
        GLCD_PutPixel(end_x, y);
    }
}

//  Draw the current board state to the LCD
void draw_board(){

    int i, j, x, y;
    
    for(i = 0; i < GRID_ROWS; i++){
        for(j = 0; j < GRID_COLS; j++){
            
            x = (j * 106) + 18;
            y = (i * 80) + 5;
            
            if(board[i][j] != ' '){
                switch(board[i][j]){
                    case 'X':
                        GLCD_Bitmap(x, y, TICTACTOE_X_WIDTH, TICTACTOE_X_HEIGHT, TICTACTOE_X_PIXEL_DATA);
                        break;
                    case 'O':						
                        GLCD_Bitmap(x, y, TICTACTOE_O_WIDTH, TICTACTOE_O_HEIGHT, TICTACTOE_O_PIXEL_DATA);
						break;
                }
            }
        }
    }
}


/*----------------------------------------------------------------------------
 *                          Game Logic Functions
 *----------------------------------------------------------------------------*/

//  Check for a winner
bool check_for_winner(){

    int i;
    
    //Checking rows and columns
    for(i = 0; i < GRID_ROWS; i++) {
        if (board[i][0] == current_player && board[i][1] == current_player && board[i][2] == current_player){
			return true;
		}
        if (board[0][i] == current_player && board[1][i] == current_player && board[2][i] == current_player){
			return true;
		}
    }
    
    //Checking diagonals
    if (board[0][0] == current_player && board[1][1] == current_player && board[2][2] == current_player){
		return true;
	}
    if (board[0][2] == current_player && board[1][1] == current_player && board[2][0] == current_player){
        return true;
    }
    
    return false;
}

//  Check if board is full
bool is_board_full(){

    int i, j;
    
    for (i = 0; i < GRID_ROWS; i++) {
        for (j = 0; j < GRID_COLS; j++) {
            if (board[i][j] == ' '){
                return false;
            }
        }
    }
    return true;
}


/*----------------------------------------------------------------------------
 *                          Main Game Loop
 * Notes: sets up an instance of the tic tac toe game
 *----------------------------------------------------------------------------*/

void run_tictactoe(){

    int joystick_val;
    selected_row = 1;
    selected_col = 1;
    game_over = false;
    current_player = 'X';
    
    init_board();
    draw_board();
    draw_grid();
    
    while(!game_over){

        highlight_cell(selected_row, selected_col);
        
        //-----------Input Handler---------------------------
        joystick_val = get_button();
    
        switch(joystick_val){
        
            case KBD_UP:
                if(selected_row > 0){
                    selected_row--;
                }else{
                    selected_row = 2;
                }
                draw_grid();
                draw_board();
                highlight_cell(selected_row, selected_col);
                break;
            
            case KBD_DOWN:
                if(selected_row < (GRID_ROWS - 1)){
                    selected_row++;
                }else{
                    selected_row = 0;
                }
                draw_grid();
                draw_board();
                highlight_cell(selected_row, selected_col);
                break;
            
            case KBD_LEFT:
                if(selected_col > 0){
                    selected_col--;
                }else{
                    selected_col = 2;
                }
                draw_grid();
                draw_board();
                highlight_cell(selected_row, selected_col);
                break;
            
            case KBD_RIGHT:
                if(selected_col < (GRID_COLS - 1)){
                    selected_col++;
                }else{
                    selected_col = 0;
                }
                draw_grid();
                draw_board();
                highlight_cell(selected_row, selected_col);
                break;
            
            case KBD_SELECT:
                if(board[selected_row][selected_col] == ' '){                   //if cell is empty, allow user to select
                    
                    board[selected_row][selected_col] = current_player;         //Save current player char to game board
                    draw_grid();
					draw_board();
										
                    if(check_for_winner()){
                        GLCD_SetBackColor(Red);
                        GLCD_SetTextColor(Black);
                        GLCD_DisplayString(6, 0, __FI, (unsigned char *)"      Winner!       ");
                        
                        //Small delay
                        osDelay(5000);
                        game_over = true;
                        return;
                    }
                    
                    else if(is_board_full()){
        
                        GLCD_SetBackColor(Red);
                        GLCD_SetTextColor(Black);
                        GLCD_DisplayString(6, 0, __FI, (unsigned char *)"         Tie        ");
                        
                        //Small delay
                        osDelay(5000);
                        game_over = true;
                        return;
                    }
                    else{
                        if(current_player == 'X'){
                            current_player = 'O';
                        }
                        else{
                            current_player = 'X';
                        }
                    }
                }
                break;
        }
    }
    
}

/*----------------------------------------------------------------------------
 *                          Tic Tac Toe Main Loop
 * Notes: the loop that starts a game, prompts users to play again, or gives the
 *        option to exit back to the game center
 *----------------------------------------------------------------------------*/

void tictactoe_main(){
    
    int joystick_val;
    bool running = true;
    
    init_ttt_lcd();
    
    while(running){
        
        joystick_val = get_button();
        
        switch(joystick_val){
            case KBD_SELECT:
                run_tictactoe();
                init_ttt_lcd();
                break;
            
            case KBD_LEFT:
                running = false;
                return;
        }
    }
    
}
