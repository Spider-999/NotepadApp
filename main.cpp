#include "notepad.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	Notepad notepad;
	notepad.show();

	return app.exec();
}
