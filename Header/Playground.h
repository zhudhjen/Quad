#ifndef __PLAYGROUND_H__
#define __PLAYGROUND_H__

#include "Entity.h"

class Playground : public Entity {
public:
	explicit Playground(void);

	~Playground(void);

	/* Draw the playground */
	void draw(void) const;

private:
	/* Coordinate */
	const long x, y, z;

	/* Board */
	const Entity* board;

	/* Columns */
	const Entity* columns[4][4];
};

#endif
