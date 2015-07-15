#ifndef __ENTITY_H__
#define __ENTITY_H__

class Entity {
public:
	virtual ~Entity(void) = 0;
	
	/* Draw the entity (pure virtual) */
	virtual void draw(void) const = 0;
};

#endif
