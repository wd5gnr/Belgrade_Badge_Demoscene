// Life on Badge
// by Al Williams -- Hackaday
// Adapted from Life on RosettaCode

#include "HaD_Badge.h"

#include <stdio.h>
#include <stdlib.h>
 
// Helper macros
#define for_x for (int x = 0; x < w; x++)
#define for_y for (int y = 0; y < h; y++)
#define for_xy for_x for_y


// Randomize board
void init(void *u,int w,int h)
{
#if 0
  unsigned (*univ)[h][w]=u;
#else
  unsigned (*univ)[8][16]=u;  // hardcode for compilers w/o cstd99
#endif
  for_xy (*univ)[y][x]=rand()<RAND_MAX / 10 ? 1 : 0;
}


// Display board 
void show(void *u, int w, int h, unsigned *speed)
{

	int (*univ)[w] = u;
	int k;
	
	for_xy displayPixel(x, y, univ[y][x]?ON:OFF);
        displayLatch(); // update

	uint32_t tick;
	tick=getTime()+*speed;
	
	// Check for close or Pause
	while (1)
	  {
	    switch (getControl())
	      {
	      case (ESCAPE):
		displayClose();  // done
		break;
		
	      case (LEFT):
		*speed-=50;
		if (*speed<50) *speed=50;
		break;

	      case (RIGHT):
		*speed+=50;
		if (*speed>1000) *speed=1000;
		break;
		
	      case (UP):  // pause
		do 
		  {
		    k=getControl();
		  } while (k!=UP && k!=ESCAPE);  // unpause
		break;
	      case (DOWN): // new game
		init(u,w,h);
		break;
	      }
	    // clever way to do time tick withou overflow problems
	    if (getTime()-tick<0x80000000UL) break;

	  }
	    
}


 
// Compute next generation (with apologies to Picard)
void evolve(void *u, int w, int h)
{
	unsigned (*univ)[w] = u;
	unsigned new[h][w];
	int change=0;

 
	for_y for_x {
		int n = 0;
		for (int y1 = y - 1; y1 <= y + 1; y1++)
			for (int x1 = x - 1; x1 <= x + 1; x1++)
				if (univ[(y1 + h) % h][(x1 + w) % w])
					n++;
 
		if (univ[y][x]) n--;
		new[y][x] = (n == 3 || (n == 2 && univ[y][x]));
	}
	
	for_y for_x if (univ[y][x]!=new[y][x]) change=1;
	if (change) for_y for_x univ[y][x] = new[y][x];
	else init(u,w,h);
}
 

// Set up game
void game(int w, int h)
{
  unsigned univ[h][w];  // game board
  unsigned speed=250;  // delay value
  init(univ,w,h);       // Initialize
  while (1) {           // main loop
    show(univ, w, h, &speed);   // display
    evolve(univ, w, h); // next gen
  }
}
 

// Badge "main"
void animateBadge(void) {
     unsigned int seed;
     // assume the framework main called all the init* calls
     
     while(1) {
       seed++;   // get a quasi random seed
       switch (getControl()) {
       case (ESCAPE):
	 displayClose();  // exit (only for emulator)
	 return;
       case (DOWN):    // set up new game on down key
	 srand(seed);
	 game(8,16);
	 break;
       }
     }
}
