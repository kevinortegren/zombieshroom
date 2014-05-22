#include <RootTools/Treenity/Include/Shaders/AbstractShaderView.h>
#include <RootTools/Treenity/Include/MaterialExporter.h>

AbstractShaderView::AbstractShaderView(QWidget* p_parent)
	: QWidget(p_parent)
{

}

void AbstractShaderView::SetMaterialName(const QString& p_name)
{
	m_name = p_name;
}

void AbstractShaderView::SetMaterial(Render::Material* p_material)
{
	m_material = p_material;
}

void AbstractShaderView::SetEngineInterface(EngineInterface* p_engineInterface)
{
	m_engineInterface = p_engineInterface;
}

void AbstractShaderView::Save()
{
	const std::string savePath = m_engineInterface->GetContentPath() + "Assets\\Materials\\" + m_name.toStdString() + ".material";

	MaterialExporter e;
	e.Export(m_material, savePath);
}