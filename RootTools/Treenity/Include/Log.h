#pragma once

#include <QTextBrowser>
#include <QString>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QLineEdit>

class Log : public QDockWidget
{
	Q_OBJECT
public:
	static Log* GetInstance();
	static void Write(const QString& p_text);

	~Log();

	void WriteToConsole(const QString& p_text);
	

private:
	static bool m_instanceFlag;
	static Log* m_console;

	Log(QWidget* p_parent = 0);

	QTextBrowser*	m_textBrowser;
	QWidget*		m_contentWidget;
	QVBoxLayout*	m_layout;
	QLineEdit*		m_lineEdit;

public slots:
	void Show();
};


