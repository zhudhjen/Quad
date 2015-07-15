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
	const long X, Y, Z;

	/* Radium */
	static const long R;

	/* Length */
	static const long L;
};

#endif
