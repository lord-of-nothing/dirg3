#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "area.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	void addPolygon(Polygon *polygon);
	void removePolygon(QUuid id);
	~MainWindow();

private slots:
	void onPolygonSelectReceived();
	void onEditingExitReceived();
	void onZoomReceived(QPointF oldPos, QPointF newPos);
	void loadLinks();

private:
	Ui::MainWindow *ui;
	void newPolygon();
	void saveToJson();
	void selectPolygon(QUuid id);
	void resetEverything();
	// void selectEdge(QUuid id);
	// void selectVertex(QUuid id);
	// Editor editor;
};
#endif // MAINWINDOW_H
