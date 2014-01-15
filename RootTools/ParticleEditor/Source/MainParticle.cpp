#include <RootTools/ParticleEditor/Include/MainParticle.h>
#include "ParticleEditor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ParticleEditor w;
	w.show();
	return a.exec();
}
namespace RootParticle
{
	MainParticle::MainParticle( std::string p_workingDirectory )
	{
	
	}
	
	MainParticle::~MainParticle()
	{
	
	}
	
	void MainParticle::Start()
	{
	
	}
	
	void MainParticle::HandleEvents()
	{
	
	}
}