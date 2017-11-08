#ifndef Song___H
#define Song___H

#include "stdlib.h"
#include "string.h"
#include "types.h"
#include "ins_part_channel.h"
#include "txt.h"

#include "st_version.h"

#include "limits.h"

// välj vad som skall kompileras


class Song
{
friend class Player;
friend class InsPly;
public:

	Song( const Song &s );
	Song( txt &flat );
	virtual ~Song();

	status_t	Load( txt &t);
	status_t	InitCheck();
	
	// returnerar huruvida låten loopade senaste cykeln
	bool		Play( float *Buffer, uint32 FrameCount );

	float	Version();
	static float	LoaderVersion();
	
// hook function implemented by derived class to time breakpoints
virtual void	Break(uint32 command);
	
	uint32		GetPos();
	void		SetPos( uint32 PALs );

#ifdef TARGET_EDITOR
	void		PartPlay( float *Buffer, uint32 SampleCount );
	void		SetPartToPlay( uint8 part = 0 , uint8 step = 0 );
	void		SetPartPos( uint8 step = 0 );

	void		Save( char *filename, bool bin=false , bool no_names = false);
	uint32		SimpLen(); // len i pals
	uint32		Sps();

	// // // // // // // // // // CHANNEL stuff
	uint8		ChannelCount();
	Channel		*GetChannel( uint8 c );
	uint8		NewChannel(); // returnerar den som skapats
	status_t	RemoveChannel( uint8 channel );

	// // // // // // // // // // PART stuff
	uint8		PartCount();
	Part		*GetPart( uint8 part );
	uint8		NewPart( Part *copy_me = 0 );
	status_t	RemovePart( uint8 part );	// B_ERROR if part is used

	// // // // // // // // // // INS stuff
	uint8		InsCount();
	Ins		*GetIns( uint8 i );
	uint8		NewIns( uint8 copy_me );
	uint8		NewIns( txt &t );
	status_t	RemoveIns(uint8 l_ins);


	// // // // // // // // // // BREAKPOINT stuff
	uint8		BreakPointCount();
	uint8		GetBreakPointIndex( uint32 PAL );
	uint32		GetBreakPointByIndex( uint8 index );
	void		SetBreakPointByIndex( uint8 index, uint32 command );
	uint32		GetBreakPoint( uint32 PAL );
	void		NewBreakPoint( uint32 PAL, uint32 command );
	void		RemoveBreakPoint( uint32 PAL );

	// // // // // // // // // // OPTIONAL stuff
	status_t	Optimize();		//B_OK if something was removed.
#endif
	
	float	*N2F;
	float	*R2F;
	float cmul[CHN];

	char	*Name();
	char	*Author();

	void SetName(const char *);
	void SetAuthor(const char *);
private:
	char	*name;
	char	*author;

	void	Init();
	void	FreeSong();
	
	uint16 st_version;
	uint16 spspal;

	uint8 channelcount;
	uint8 partcount;
	uint8 instrumentcount;	// anger inte antal instrument, ett för mycket
	uint8 breakpcount;
	
	Channel	*chan;
	Part		*parts;
	Ins		*ins;

	BreakPoint *breakpoints;
	int		bpcount;		// counting the current bp
	status_t init;	

	bool looped;
	
	// player
	float *stereobuffer;
	void MemMulAdd( float *d, float *s, uint32 count, float l, float r);
	void MemMulMove( float *d, float *s, uint32 count, float l, float r);
	uint32 PALs;
	uint32 smp_left;
	float *bs;
	Player **p;
	
#ifdef TARGET_EDITOR
	// partplay
	Channel pchan;
	ChStep pchanstep;
	uint32 psmp_left;
	float *pbs;
	Player *pp;
#endif

};

#endif
