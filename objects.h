#ifndef OBJECTS_H
#define OBJECTS_H

// platform structure
typedef struct platform{
	int r;
	int c;
	int len;
} Platform;

// player structure
typedef struct player{
	int r;
	int c;
	int vspeed;
	int hspeed;
	int phase;
	int collision;
	int jump;
	int jcounter;
	int distance;
} Player;

#define JUMPSCALE 3
#define JUMPTIME 7

// function prototypes
void init_platform(Platform *platform, int *collision, int r, int c, int len);
void init_player(Player *player);
void jump(Player *player);
void plot_player(Player *player);
void player_run(Player *player);
void plot_platform(Platform *platform, int *collision);
void move_platform(Platform *platform, int *collision, int dr, int dc);
void clear_platform(Platform *platform, int *collision, int dr, int dc);
int platform_gen(Platform *platforms, int num, int *collision);
void move_screen(Platform *platforms, int num, int *collision, int dr, int dc);
void check_collision(Player *player, int *collision);
int iterate(Player *player,Platform *platforms,int num, int *collision);
#endif
