#ifndef __BEAD_H__
#define __BEAD_H__

#include "Entity.h"

enum BeadColor {
	black = 0,
	white
};

class Bead : public Entity {
public:
	explicit Bead(void);

	~Bead(void);

	/* Draw the bead */
	void draw(void) const;

	/* Get type */
	inline const bool get_type(void) const {
		return type;
	}

private:
	/* Coordinate */
	const long x, y, z;

	/* Radium */
	const long r;

	/* Type */
	const bool type;
};

#endif
