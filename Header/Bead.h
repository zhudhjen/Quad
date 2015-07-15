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

	/* Fall */
	void fall(void);

private:
	/* Coordinate */
	long x, y, z;

	/* Radium */
	static const long R;

	/* Type */
	const bool TYPE;
};

#endif
