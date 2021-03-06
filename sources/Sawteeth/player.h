#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"
#include "insply.h"
#include "song.h"

class Player
{
public:
	Player(Song *s, Channel *chn);
	~Player();

	// if buffer was updated
	bool	NextBuffer();
	float	*Buffer();

	float Mul();

	bool Looped();
	
	void JumpPos(uint8 seqpos, uint8 steppos, uint8 pal);
private:

	bool looped;
	bool tmploop;
	
	InsPly *ip;
	Song *song;
	ChStep *step;
	Channel *ch;
	
	Step *currstep;	
	Part *currpart;
	uint32 seqcount;		// step in sequencer
	uint8 stepc;		// step in part
	
	uint32 nexts;		// PAL-screen counter	

	float amp;
	float ampstep;	

	float freq;
	float freqstep;	
	float targetfreq;
	
	float cutoff;
	float cutoffstep;
	
	//------------------
	// buffers
	float *buffer;
};
#endif