#include "notepad.h"
#include "./ui_notepad.h"

// Notepad class constructor.
Notepad::Notepad(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::Notepad)
{
	ui->setupUi(this);

	// Connect all of the file actions to
	// their respective custom slot functions.
	connect(ui->actionNew, &QAction::triggered, this, &Notepad::fileNew);
	connect(ui->actionSave, &QAction::triggered, this, &Notepad::fileSave);
	connect(ui->actionSaveAs, &QAction::triggered, this, &Notepad::fileSaveAs);
	connect(ui->actionOpen, &QAction::triggered, this, &Notepad::fileOpen);
	connect(ui->actionExit, &QAction::triggered, this, &Notepad::close);

	// Text editing actions.
	connect(ui->actionFonts, &QAction::triggered, this, &Notepad::selectFont);
	connect(ui->actionBold, &QAction::triggered, this, &Notepad::setFontBold);
	connect(ui->actionUnderline, &QAction::triggered, this, &Notepad::setFontUnderline);
	connect(ui->actionItalic, &QAction::triggered, this, &Notepad::setFontItalic);

	// Copy, cut, paste, undo, redo actions.
#if QT_CONFIG(clipboard)
	connect(ui->textEdit, &QTextEdit::copyAvailable, ui->actionCopy, &QAction::setEnabled);
	connect(ui->actionCopy, &QAction::triggered, ui->textEdit, &QTextEdit::copy);
	connect(ui->actionCut, &QAction::triggered, ui->textEdit, &QTextEdit::cut);
	connect(ui->actionPaste, &QAction::triggered, ui->textEdit, &QTextEdit::paste);
#endif
	connect(ui->textEdit, &QTextEdit::redoAvailable, ui->actionRedo, &QAction::setEnabled);
	connect(ui->actionRedo, &QAction::triggered, ui->textEdit, &QTextEdit::redo);
	connect(ui->textEdit, &QTextEdit::undoAvailable, ui->actionUndo, &QAction::setEnabled);
	connect(ui->actionUndo, &QAction::triggered, ui->textEdit, &QTextEdit::undo);
	connect(ui->actionReadMore, &QAction::triggered, this, &Notepad::readMore);
}

// Notepad class deconstructor.
Notepad::~Notepad()
{
	delete ui;
}

// Notepad private slot functions.
void Notepad::fileNew()
{
	currentFile.clear();
	ui->textEdit->setText(QString() );
}

void Notepad::fileSave()
{
	QString fileName;

	// If we dont find a previous filename
	// we allow the user to select one.
	if(currentFile.isEmpty() )
	{
		fileName = QFileDialog::getSaveFileName(this, "Save");
		if(fileName.isEmpty() )
		{
			return ;
		}
		currentFile = fileName;
	}
	else
	{
		fileName = currentFile;
	}

	// Display an error if the file
	// cannot be saved.
	QFile file(fileName);
	if(!file.open(QIODevice::WriteOnly | QFile::Text) )
	{
		QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString() );
		return ;
	}

	// Change the window title to
	// the saved file name.
	QFileInfo fileInfo(file);
	setWindowTitle(fileInfo.fileName() );

	// Create a text stream and
	// write them to the file.
	QTextStream out(&file);
	QString text = ui->textEdit->toPlainText();
	out << text;

	file.close();
}

void Notepad::fileSaveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save as");

	if(fileName.isEmpty() )
	{
		return ;
	}

	QFile file(fileName);

	if(!file.open(QFile::WriteOnly | QFile::Text) )
	{
		QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString() );
		return ;
	}

	currentFile = fileName;

	QFileInfo fileInfo(file.fileName() );
	setWindowTitle(fileInfo.fileName() );

	QTextStream out(&file);
	QString text = ui->textEdit->toPlainText();
	out << text;
	file.close();
}

void Notepad::fileOpen()
{
	// This allows the user to acces
	// the file explorer and decide
	// which file they want to open.
	QString fileName = QFileDialog::getOpenFileName(this, "Open the file.");

	// If the file name is unused
	// then this function does nothing.
	if(fileName.isEmpty() )
	{
		return ;
	}

	// Store the selected file
	// in a QFile variable named
	// file and assign the private
	// class variable member to fileName;
	QFile file(fileName);
	currentFile = fileName;

	// If the selected file cannot be opened
	// we show a warning and the type of error.
	if(!file.open(QIODevice::ReadOnly | QFile::Text) )
	{
		QMessageBox::warning(this, "Warning", "Cannot open the file: " + file.errorString() );
		return ;
	}

	// Set the window title to the
	// file's name. Strip the path of
	// the file and show only the file name.
	QFileInfo fileInfo(file.fileName() );
	setWindowTitle(fileInfo.fileName() );

	// Create a stream for user input.
	QTextStream in(&file);
	// Store the text that the user enters in a QString.
	QString text = in.readAll();
	// Modify the textEdit to show the user's input.
	ui->textEdit->setText(text);
	// Close the input stream.
	file.close();
}

void Notepad::selectFont()
{
	bool selectFont;
	QFont font = QFontDialog::getFont(&selectFont, this);
	if(selectFont)
	{
		ui->textEdit->setFont(font);
	}
}

void Notepad::setFontBold(bool bold)
{
	bold ? ui->textEdit->setFontWeight(QFont::Bold) :
		   ui->textEdit->setFontWeight(QFont::Normal);
}

void Notepad::setFontUnderline(bool underline)
{
	ui->textEdit->setFontUnderline(underline);
}

void Notepad::setFontItalic(bool italic)
{
	ui->textEdit->setFontItalic(italic);
}

void Notepad::readMore()
{
	QMessageBox::about(this, tr("About Notepad"), tr("Notepad app project made by Hary Patrascu.") );
}
