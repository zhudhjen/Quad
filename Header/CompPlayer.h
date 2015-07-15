#ifndef __COMPPLAYER_H__
#define __COMPPLAYER_H__

#include "Player.h"

class CompPlayer : public Player {
public:
	explicit CompPlayer(void);

	~CompPlayer(void);

	/* Get decision */
	void get_decision(void) const;
};

#endif
