#pragma once
#include <Qt/QtWidgets/QTreeWidget>
#include <Qt/QtGui/QDropEvent>
#include "OnCreate.h"
#include "OnCollide.h"
#include "OnDestroy.h"
class CustomTreeWidget : public QTreeWidget
	{
		Q_OBJECT
	public:
		explicit CustomTreeWidget(QWidget* parent = 0);
		void SetOnEventClass(AbilityEditorNameSpace::MainOnEvent* p_onEvent);
	protected:
		void dropEvent(QDropEvent* event);
		void dragEnterEvent(QDragEnterEvent* event);
		void dragMoveEvent(QDragMoveEvent *event);
	private:
		AbilityEditorNameSpace::MainOnEvent*  m_onEvent;
};
