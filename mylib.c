// Sheng Jiang
#include "mylib.h"

// video buffer
u16 *videoBuffer = (u16 *) 0x6000000;

// A function that waits for scanline blank to prevent tearing
void wait_for_vblank() {
  u16 *X = SCANLINECOUNTER;

  while (*X >= 160) {;} // in vblank, wait to exit
  while (*X < 160) {;} // wait for beginning of next vblank
}

// A function to set pixel (r, c) to the color passed in.
void setPixel(int r, int c, u16 color){
	videoBuffer[240*r+c] = color;
}

// A function to draw a FILLED rectangle starting at (r, c)
void drawRect(int r, int c, int height, int width, u16 color){
	for (int i = 0; i < height; i++) { // loop through each row
		for (int j = 0; j < width; j++) { // each column
			setPixel(r+i, c+j, color);
		}
	}
}

// A function to draw a HOLLOW rectangle starting at (r, c)
void drawHollowRect(int r, int c, int height, int width, u16 color){
	for (int i = 0; i < height; i++) {
		setPixel(r+i, c, color);
		setPixel(r+i, c+width-1, color);
	}
	for (int i = 0; i < width; i++) {
		setPixel(r, c+i, color);
		setPixel(r+height-1, c+i, color);
	}
}

// A function that draws an image of specified height and width onto screen starting at (c,r)
void drawImage3(int r, int c, int height, int width, const u16* image){
	drawSprite(r,c,0,0,height,width,image,width);
}

// A function that draws a portion of an image with coordinates (sc, sr, sc+swidth, sr+sheight) onto the screen starting at (dc, dr)
void drawSprite(int dr, int dc, int sr, int sc, int sheight, int swidth, const u16* image, int iwidth){
	int plot = 1;
	if (sheight <= 0 || swidth <= 0){
		plot = 0;
	}
	if (dc < 0){
		if (swidth+dc > 0){
			sc += 0-dc;
			swidth += dc;
			dc = 0;
		}
		else{
			plot = 0;
		}
	}
	if (dc+swidth > WIDTH){
		if (dc >= WIDTH){
			plot = 0;
		}
		else{
			swidth += WIDTH-(dc+swidth);
		}
	}
	if (dr < 0){
		if (sheight+dr > 0){
			sr += 0-dr;
			sheight += dr;
			dr = 0;
		}
		else{
			plot = 0;
		}
	}
	if (dr+sheight > HEIGHT){
		if (dr >= HEIGHT){
			plot = 0;
		}
		else{
			sheight += HEIGHT-(dr+sheight);
		}
	}
	if (plot){
		for (int i = dr; i < sheight+dr; i++){
			REG_DMA3CNT = 0;
			REG_DMA3SAD = (vu32) (image+(i-dr+sr)*iwidth+sc);
			REG_DMA3DAD = (vu32) (videoBuffer+dc+i*240);
			REG_DMA3CNT = swidth|DMA_ON|DMA_NOW;
		}
	}
}
