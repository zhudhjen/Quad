#ifndef __BOARD_H__
#define __BOARD_H__

#include "Entity.h"

class Board : public Entity {
public:
	explicit Board(void);

	~Board(void);

	/* Draw the board */
	void draw(void) const;

private:
	/* Coordinate */
	const long X, Y, Z;

	/* Width */
	const long W;

	/* Height */
	const long H;
};

#endif
