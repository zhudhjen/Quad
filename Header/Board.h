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
	const long x, y, z;

	/* Width */
	const long w;

	/* Height */
	const long h;
};

#endif
