#ifndef EDITOR_H
#define EDITOR_H

#include "geometry.h"
#include "mainwindow.h"
#include <QDockWidget>
#include <QPushButton>
#include <QShowEvent>
#include <QTableWidget>
#include <QVector2D>
#include <QWidget>

namespace Ui {
class Editor;
}

class Editor : public QWidget {
	Q_OBJECT

public:
	explicit Editor(QWidget *parent = nullptr);
	// void setupNew();
	void setupExistingPolygon(Polygon *polygon);
	// void resetEditor();
	~Editor();

protected:
	void showEvent(QShowEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;

private slots:
	void onBufferConnect();
	void onPolygonSelectReceived(Polygon *polygon);
	void onVertexSelectReceived(Vertex* vertex);
	void onEdgeSelectReceived(Edge* edge);

	void addVertex();
	void loadFromJson();
	void saveToJson();

	// void addVertexByMouse(QPoint *point); // add new point
	// void editVertexCoord(
		// int row,
		// QPoint *new_coord); // change coord for vertex when move by mouse

private:
	void updateTableSize();
	void loadError();
	// void addVertexRow(int row, QString vName, double x, double y);
	void addVRow(int row, QString vName, double x, double y, int isDivider);
	void addERow(int row, QString name, QString property);

	void setupLinks();

	void saveVertex(int row);
	void resetVertex(int row);
	void finishEditVertex(int row);
	void editVertex(int row);

	void saveEdge(int row);
	void resetEdge(int row);
	void finishEditEdge(int row);
	void editEdge(int row);

	void clearNew();
	void savePolygon();
	void resetEditor();

	Ui::Editor *ui;
	QTableWidget *vtable;
	// QPushButton *cancelBtn = nullptr;
	QTableWidget *etable;
	QDockWidget *dock;
	int polygonNumber;
	Polygon *editedPolygon = nullptr;
	QIcon addIcon = style()->standardIcon(QStyle::SP_DialogOkButton);
	QIcon editIcon = style()->standardIcon(QStyle::SP_ArrowBack);
	QIcon saveIcon = style()->standardIcon(QStyle::SP_DialogApplyButton);
	QIcon resetIcon = style()->standardIcon(QStyle::SP_DialogCancelButton);

	QVector<QVector3D> buffer;
	QString editedEdgeName;
	QString editedEdgeProperty;

	const double minCoord = 0.0;
	const double maxCoord = 1000.0;
	const int precision = 3;
	QVector<QString> polygonMaterials;
	QVector<QString> edgeProperties;
};

#endif // EDITOR_H
