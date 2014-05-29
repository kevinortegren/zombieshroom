#pragma once

#include <RootTools/Treenity/Include/Components/AbstractComponentView.h>
#include <RootTools/Treenity/GeneratedFiles/ui_ParticleComponent.h>

class ParticleView : public AbstractComponentView
{
	Q_OBJECT
public:
	ParticleView(QWidget* p_parent = 0);

	const QString& GetComponentName() const;
	void DisplayEntity(ECS::Entity* p_entity);
private:
	QString m_name;
	Ui::Form_ParticleView ui;

private slots:
	void ParticleDropped();
};