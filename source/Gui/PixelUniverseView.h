#pragma once

#include <QGraphicsScene>
#include <QVector2D>
#include <QTimer>

#include "Gui/Definitions.h"
#include "ModelBasic/Definitions.h"
#include "ModelBasic/Descriptions.h"

class PixelUniverseView : public QGraphicsScene
{
    Q_OBJECT
public:
    PixelUniverseView(QObject* parent = nullptr);
    virtual ~PixelUniverseView();

    virtual void init(
        Notifier* notifier,
        SimulationController* controller,
        SimulationAccess* access,
        DataRepository* manipulator,
        ViewportInterface* viewport);
    virtual void activate();
	virtual void deactivate();

	virtual void refresh();

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent* e);

private:
	Q_SLOT void receivedNotifications(set<Receiver> const& targets);
	Q_SLOT void requestImage();
	Q_SLOT void imageReady();
	Q_SLOT void scrolled();

	list<QMetaObject::Connection> _connections;

    SimulationAccess* _access = nullptr;
	DataRepository* _repository = nullptr;
	SimulationController* _controller = nullptr;
	ViewportInterface* _viewport = nullptr;

    ImageSectionItem* _imageSectionItem = nullptr;

	Notifier* _notifier = nullptr;
};

