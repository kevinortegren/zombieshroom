#include <RootTools/Treenity/Include/Components/ParticleView.h>

ParticleView::ParticleView(QWidget* p_parent)
	: AbstractComponentView(p_parent), m_name("Particle")
{
	ui.setupUi(this);
	ui.lineEdit_Particle->AddDropFilter("particle");

	connect(ui.lineEdit_Particle, SIGNAL(editingFinished()), this, SLOT(ParticleDropped()));
}

const QString& ParticleView::GetComponentName() const
{
	return m_name;
}

void ParticleView::DisplayEntity(ECS::Entity* p_entity)
{
	ui.lineEdit_Particle->setText(QString::fromStdString(m_engineInterface->GetParticleEmitter(p_entity)));
}

void ParticleView::ParticleDropped()
{
	QString name = ui.lineEdit_Particle->text();

	m_engineInterface->SetParticleEmitter(*m_editorInterface->GetSelection().begin(), name.toStdString());
}