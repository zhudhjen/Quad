#ifndef __COLUMN_H__
#define __COLUMN_H__

#include "Entity.h"

class Column : public Entity {
public:
	explicit Column(void);

	~Column(void);

	/* Draw the column */
	void draw(void) const;
	
private:
	/* Coordinate */
	const long x, y, z;

	/* Radium */
	const long r;

	/* Length */
	const long l;
};

#endif
