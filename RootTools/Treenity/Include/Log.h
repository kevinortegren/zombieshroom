#pragma once

#include <QTextBrowser>
#include <QString>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QLineEdit>

#include <QProgressDialog>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>

class Log : public QDockWidget
{
	Q_OBJECT
public:
	static Log* GetInstance();
	static void Write(const QString& p_text);
	static void RunWithProgressBar( QFuture<void> p_future);

	~Log();

	void WriteToConsole(const QString& p_text);
	void Run( QFuture<void> p_future);
	

private:
	static bool m_instanceFlag;
	static Log* m_console;

	Log(QWidget* p_parent = 0);

	QTextBrowser*	m_textBrowser;
	QWidget*		m_contentWidget;
	QVBoxLayout*	m_layout;
	QLineEdit*		m_lineEdit;

	QFutureWatcher<void> m_futureWatcher;
	QProgressDialog* m_progressDialog;

public slots:
	void Show();
};


