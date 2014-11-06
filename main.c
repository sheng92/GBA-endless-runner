// Sheng Jiang
#include "mylib.h"
#include "text.h"
#include "objects.h"
#include "startscreen.h"
#include "bg.h"
#include<stdio.h>

// function prototype
void intro(void);

// main function
int main(void){
	// set GBA parameters
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	
	
	// intro sequence (start screen)
	intro();
	drawImage3(0,0,160,240,bg);

	// init variables
	Platform platforms[5];
	Player player;
	int num_platforms = 1;
	int collision[8] = {HEIGHT};
	init_player(&player);
	plot_player(&player);
	init_platform(&platforms[0],collision,105,50,300);
	int cycle = 0;
	int loop = 1;
	int notpressed = 1; // keeps track of button held down
	char buff[16];
	sprintf (buff, "%8d", player.distance);
	drawString(2, 80, buff, WHITE);
	int counter = 0;
	// main game loop
	while (loop){
		wait_for_vblank();
		if (cycle > 3){ // animating the player each frames is too fast
			player_run(&player);
			cycle = 0;
		}
		sprintf(buff, "Distance: %d", player.distance);
		drawImage3(0,20,10,160,bg);
		drawString(2, 2, buff, WHITE);
		plot_player(&player);
		num_platforms = iterate(&player,platforms,num_platforms,collision);
		if (player.hspeed > 0)
			cycle++;
		if (notpressed && (PRESSED(BUTTON_UP) || PRESSED(BUTTON_A)) ){
			jump(&player);
			notpressed = 0;
		}
		else{
			notpressed = 1;
		}
		if (player.distance - counter >= 1000){ // speeds up per 1000 pixels reached
			player.hspeed += 1;
			counter = player.distance;
		}
		if (player.collision){ // game over screen
			sprintf(buff, "Distance: %d", player.distance);
			drawImage3(0,20,10,160,bg);
			drawString(2, 2, buff, WHITE);
			int gameover = 1;
			drawImage3(120,0,40,240,bg);
			char score[20];
			sprintf(score, "Game over. Score: %d", player.distance);
			drawString(130,30,score, WHITE);
			drawString(140,30,"Press START to try again", WHITE);
			while (gameover){
				
				if (PRESSED(BUTTON_START)){
					while(PRESSED(BUTTON_START)){gameover = 0;}
				}
				else
					notpressed=1;
			}
			move_screen(platforms, num_platforms, collision, 200, 0);
			init_player(&player);
			player.vspeed = 300;
		}
	}// end main game loop
}// end main

// intro sequence
void intro(void){
	int loop = 1;
	while(loop){
		wait_for_vblank();
		drawImage3(0,0,160,240,startscreen);
		if (PRESSED(BUTTON_START) || PRESSED(BUTTON_A)){
			loop = 0;
			while(PRESSED(BUTTON_START) || PRESSED(BUTTON_A)) ;
		}
	}
}// end intro
