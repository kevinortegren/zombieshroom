#pragma once

#include <QWidget>
#include <QFrame>
#include <QPropertyAnimation>
#include <RootTools/Treenity/Include/EngineInterface.h>
#include <RootTools/Treenity/Include/EditorInterface.h>

class AbstractComponentView : public QFrame
{
	Q_OBJECT
public:
	AbstractComponentView(QWidget* p_parent = 0);
	virtual ~AbstractComponentView();

	virtual void SetEngineInterface(EngineInterface* p_engineInterface);
	virtual void SetEditorInterface(EditorInterface* p_editorInterface);
	virtual void SetReceiveUpdates(bool p_value);
	virtual void Delete(ECS::Entity* p_entity);
	virtual void Show(bool p_value);

	virtual EditorInterface* GetEditorInterface();
	virtual EngineInterface* GetEngineInterface();

	virtual bool IsReceivingUpdates() const;

	virtual const QString& GetComponentName() const = 0;
	virtual void DisplayEntity(ECS::Entity* p_entity) = 0;

	bool m_shown;

protected:
	EngineInterface* m_engineInterface;
	EditorInterface* m_editorInterface;
	bool m_receiveUpdates;
	

signals:
	void deleted(ECS::Entity* p_entity);
};