#pragma once

#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include <QtGui/QCloseEvent>
#include <QtWidgets/QMessageBox>
#include <map>
#include <string>
class ParticleTab : public QTabWidget
{
	Q_OBJECT

public:
	ParticleTab(QWidget *parent = 0);
	~ParticleTab();
	//SETTERS

	//METHODS
	void AddTab();
private:
	//METHODS
	void closeEvent(QCloseEvent *event);
	
	//MEMBERS
	std::map<std::string, QWidget*> m_tabs;
	unsigned int m_numTabs;
private slots:
	void RemoveTab(int);

};

