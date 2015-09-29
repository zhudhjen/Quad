#ifndef __VIEW_H__
#define __VIEW_H__

#include <assert.h>
#include <vector>
#include <irrlicht.h>

class View {
public:
	explicit View(irr::IrrlichtDevice *device);
	virtual ~View(void);
	virtual void initialize(void);
	virtual void drop(void);
	virtual void update(void);
protected:
	irr::IrrlichtDevice *getIrrlichtDevice(void) const;
	const bool getEnable(void) const;
private:
	irr::IrrlichtDevice *m_device;
	bool m_enable;
};

View::View(irr::IrrlichtDevice *device) :
	m_device(device),
	m_enable(false)
{

}
View::~View(void) {

}
void View::initialize(void) {
	m_enable = true;
}
void View::drop(void) {
	m_device->getVideoDriver()->removeAllOcclusionQueries();
	m_device->getSceneManager()->clear();
	m_device->getGUIEnvironment()->clear();
	m_enable = false;
}
void View::update(void) {
	m_device->getVideoDriver()->beginScene(true, true, irr::video::SColor(0, 255, 255, 255));

	m_device->getSceneManager()->drawAll();
	m_device->getGUIEnvironment()->drawAll();

	m_device->getVideoDriver()->endScene();
}

irr::IrrlichtDevice *View::getIrrlichtDevice(void) const {
	return m_device;
}
const bool View::getEnable(void) const {
	return m_enable;
}

#endif
