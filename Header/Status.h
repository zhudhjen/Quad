#ifndef __STATUS_H__
#define __STATUS_H__

#include "Player.h"

struct Status {
	unsigned short level;

	Player &p1, &p2;

	bool black[4][4][4], white[4][4][4];

	unsigned long step[64];

	unsigned long undo_count;
};

#endif
