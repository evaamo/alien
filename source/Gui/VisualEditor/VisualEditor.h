#pragma once

#include <QWidget>
#include <QVector2D>
#include <QMatrix>

#include "Model/Api/Definitions.h"
#include "gui/Definitions.h"

namespace Ui {
	class VisualEditor;
}

class VisualEditor : public QWidget
{
    Q_OBJECT
public:
    VisualEditor(QWidget *parent = 0);
    virtual ~VisualEditor();

	void init(Notifier* notifier, SimulationController* controller, DataManipulator* manipulator, SimulationAccess* access);
    void reset();

	void setActiveScene(ActiveScene activeScene);
    QVector2D getViewCenterWithIncrement ();
    QGraphicsView* getGraphicsView ();
    qreal getZoomFactor ();

public Q_SLOTS:
    void zoomIn ();
    void zoomOut ();

private:
    Ui::VisualEditor *ui;

	SimulationController* _controller = nullptr;
    PixelUniverseView* _pixelUniverse = nullptr;
    ItemUniverseView* _shapeUniverse = nullptr;
	ViewportController* _viewport = nullptr;

	ActiveScene _activeScene = ActiveScene::PixelScene;

    bool _pixelUniverseInit = false;
    bool _shapeUniverseInit = false;

    qreal _posIncrement = 0.0;

    bool _screenUpdatePossible = true;
};





