#pragma once
#include <Qt/QtWidgets/QListWidget>
#include <Qt/QtGui/QDropEvent>
#include "Entity.h"
class CustomCompListWidget : public QListWidget
	{
		Q_OBJECT
	public:
		explicit CustomCompListWidget(QWidget* parent = 0);
		void SetEntity(AbilityEditorNameSpace::Entity* p_entity);
		void RemoveSelected(QListWidgetItem* p_item);
		void ViewSelectedData(QListWidgetItem* p_item, QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_propFact);
		void SaveSelectedData(QListWidgetItem* p_item, QtVariantPropertyManager* p_propMan, QtTreePropertyBrowser* p_propBrows, QtVariantEditorFactory* p_propFact);
		void LoadData();
		void Clear();
	protected:
		void dropEvent(QDropEvent* event);
		void dragEnterEvent(QDragEnterEvent* event);
		void dragMoveEvent(QDragMoveEvent *event);
	private:
		AbilityEditorNameSpace::Entity* m_entity;
};
