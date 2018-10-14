﻿#pragma once
#include <QObject>

#include "Definitions.h"

class ActionHolder
	: public QObject
{
	Q_OBJECT

public:
	ActionHolder(QObject* parent = nullptr);
	virtual ~ActionHolder() = default;

	QAction* actionNewSimulation = nullptr;
	QAction* actionLoadSimulation = nullptr;
	QAction* actionSaveSimulation = nullptr;
	QAction* actionRunSimulation = nullptr;
	QAction* actionRunStepForward = nullptr;
	QAction* actionRunStepBackward = nullptr;
	QAction* actionSnapshot = nullptr;
	QAction* actionRestore = nullptr;
	QAction* actionExit = nullptr;

	QAction* actionComputationSettings = nullptr;
	QAction* actionEditSimParameters = nullptr;
	QAction* actionLoadSimParameters = nullptr;
	QAction* actionSaveSimParameters = nullptr;

	QAction* actionEditSymbols = nullptr;
	QAction* actionLoadSymbols = nullptr;
	QAction* actionSaveSymbols = nullptr;
	QAction* actionMergeWithSymbols = nullptr;

	QAction* actionEditor = nullptr;
	QAction* actionMonitor = nullptr;
	QAction* actionZoomIn = nullptr;
	QAction* actionZoomOut = nullptr;
	QAction* actionFullscreen = nullptr;
	QAction* actionShowCellInfo = nullptr;
	QAction* actionCenterSelection = nullptr;

	QAction* actionNewCell = nullptr;
	QAction* actionNewParticle = nullptr;
	QAction* actionCopyEntity = nullptr;
	QAction* actionPasteEntity = nullptr;
	QAction* actionDeleteEntity = nullptr;
	QAction* actionNewToken = nullptr;
	QAction* actionCopyToken = nullptr;
	QAction* actionPasteToken = nullptr;
	QAction* actionDeleteToken = nullptr;

	QAction* actionNewRectangle = nullptr;
	QAction* actionNewHexagon = nullptr;
	QAction* actionNewParticles = nullptr;
	QAction* actionLoadCol = nullptr;
	QAction* actionSaveCol = nullptr;
	QAction* actionCopyCol = nullptr;
	QAction* actionPasteCol = nullptr;
	QAction* actionDeleteCol = nullptr;
	QAction* actionDeleteSel = nullptr;
	QAction* actionRandomMultiplier = nullptr;
	QAction* actionGridMultiplier = nullptr;

	QAction* actionAbout = nullptr;
	QAction* actionDocumentation = nullptr;

	QAction* actionRestrictTPS = nullptr;
};