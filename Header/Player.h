#ifndef __PLAYER_H__
#define __PLAYER_H__

class Player {
public:
	virtual ~Player(void) = 0;

	/* Get decision */
	virtual void get_decision(void) const = 0;
};

#endif
