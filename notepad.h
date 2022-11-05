#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QTextEdit>
#include <QFileDialog>
#include <QFont>
#include <QFile>
#include <QFontDialog>
#include <QMessageBox>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class Notepad; }
QT_END_NAMESPACE

class Notepad : public QMainWindow
{
	Q_OBJECT

public:
	Notepad(QWidget *parent = nullptr);
	~Notepad();

private slots:
	void fileNew();
	void fileSave();
	void fileSaveAs();
	void fileOpen();
	void selectFont();
	void setFontBold(bool bold);
	void setFontUnderline(bool underline);
	void setFontItalic(bool italic);
	void readMore();

private:
	Ui::Notepad *ui;
	QString currentFile;
};
#endif // NOTEPAD_H
