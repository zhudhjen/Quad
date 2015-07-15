#ifndef __REALPLAYER_H__
#define __REALPLAYER_H__

#include "Player.h"

class RealPlayer : public Player {
public:
	explicit RealPlayer(void);

	~RealPlayer(void);

	/* Get decision */
	void get_decision(void) const;
};

#endif
