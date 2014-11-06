#include "objects.h"
#include "mylib.h"
#include "avatar.h"
#include "text.h"
#include "bg.h"
#include <stdlib.h>

// initializes a platform at (r,c) with length len
void init_platform(Platform *platform, int *collision, int r, int c, int len){
	platform->r = r;
	platform->c = c;
	platform->len = len;
	plot_platform(platform, collision);
}

// initializes a player
void init_player(Player *player){
	player->r = 80;
	player->c = 50;
	player->vspeed = 0;
	player->hspeed = 2;
	player->phase = 0;
	player->collision = 0;
	player->jump = 0;
	player->jcounter = 0;
	player->distance = 0;
	plot_player(player);
}

// sends a jump command to the player (only jumps when on ground)
void jump(Player *player){
	player->jump = 1;
}

// plots the player
void plot_player(Player *player){
	int r = player->r;
	int c = player->c;
	drawSprite(r, c, 0, 8*player->phase, 15, 8, avatar, 40);
}

// advances the sprite frame
void player_run(Player *player){
	player->phase = (player->phase + 1) % 5;
}

// plots the platform and records on a collision map if relevant
void plot_platform(Platform *platform, int *collision){
	int r = platform->r;
	int c = platform->c;
	int len = platform->len;
	drawSprite(r, c, 160, 0, HEIGHT-r, len, bg, WIDTH);
	for (int i = c; i < c+len; i++){
		if (i >= 50 && i < 58){
			if (r < collision[i-50])
				collision[i-50] = r;
		}
	}
}

// move platform according to player movement
void move_platform(Platform *platform, int *collision, int dr, int dc){
	platform->c += dc;
	platform->r += dr;
	if (platform->r < 0)
		platform->r = 0;
	plot_platform(platform, collision);
}

// plots background where platform was previously (erases)
void clear_platform(Platform *platform, int *collision, int dr, int dc){
	if (dc < 0){
		drawSprite(platform->r, platform->c+platform->len+dc, platform->r, platform->c+platform->len+dc, HEIGHT-platform->r, -dc, bg, WIDTH); 
	}
	if (dc > 0){
		drawSprite(platform->r, platform->c, platform->r, platform->c, HEIGHT-platform->r, dc, bg, WIDTH);
	}
	if (dr > 0){
		drawSprite(platform->r, platform->c, platform->r, platform->c, dr, platform->len, bg, WIDTH);
	}
}

// takes a platform array and overwrites expired platforms (as well as generating new ones when necessary)
int platform_gen(Platform *platforms, int num, int *collision){
	int newplats = 0;
	int last = 0;
	int todelete = -1;
	for (int i = 0; i<num; i++){
		if (platforms[i].c > WIDTH){
			newplats++;
		}
		if (platforms[i].c + platforms[i].len > last)
			last = platforms[i].c + platforms[i].len;
		if (platforms[i].c + platforms[i].len < 0){
			todelete = i;
		}
	}
	if (todelete > -1 && num > 1){
		int rn = rand();
		int r = 105 + (((rn>>4)<<4)-rn);
		int c = last + 20 + (rn-((rn>>5)<<5));
		int len = 100 + (rn-((rn>>7)<<7));
		init_platform(&platforms[todelete],collision,r,c,len);
	}
	else if (newplats<1 && todelete < 0){
		int rn = rand();
		int r = 105 + (((rn>>4)<<4)-rn);
		int c = last + (rn-((rn>>5)<<5));
		int len = 100 + (rn-((rn>>7)<<7));
		init_platform(&platforms[num+1],collision,r,c,len);
		num++;
	}
	return num;
}

// moves the screen in relation to player movement
void move_screen(Platform *platforms, int num, int *collision, int dr, int dc){
	for (int i=0; i<8; i++){
		collision[i] = HEIGHT;
	}
	for (int i=0; i<num; i++) {
		clear_platform(&platforms[i], collision, dr, dc);
	}
	for (int i=0; i<num; i++) {
		move_platform(&platforms[i], collision, dr, dc);
	}
}

// checks player collision
void check_collision(Player *player, int *collision){
	int collided = 0;
	int r = player->r;
	for (int i = 0; i < 8; i++){
		if (r +15 > collision[i]){
			collided = 1;
			break;
		}
	}
	player->collision = collided;
}

// iterates a cycle of the game
int iterate(Player *player, Platform *platforms, int num, int *collision){
	int r = player->r;
	int drop = 1;
	for (int i = 0; i < 8; i++){
		if (r+15 >= collision[i]){
			if (player->jump){
				player->vspeed = -JUMPSCALE;
				player->jcounter = 0;
			}
			else
				player->vspeed = 0;
			drop = 0;
			break;
		}
	}
	if (drop && player->jcounter >= JUMPTIME){
		player->vspeed += 1;
		player->jcounter = 0;
	}
	player->jcounter += 1;
	
	if (player->vspeed > 0){
		for (int i = 0; i < 8; i++){
			if (r+15+player->vspeed > collision[i]){
				player->vspeed = r+15+player->vspeed - collision[i];
			}
		}
	}
	int dr = -player->vspeed;
	int dc = -player->hspeed;
	player->distance += player->hspeed;
	player->jump = 0;
	move_screen(platforms,num,collision,dr,dc);
	check_collision(player, collision);
	return platform_gen(platforms, num, collision);
}
