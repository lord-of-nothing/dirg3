#ifndef AREA_H
#define AREA_H

#include "geometry.h"
#include <QMouseEvent>
#include <QWidget>
#include <QScrollArea>

class Mediator : public QObject {
	Q_OBJECT
public:
	static Mediator *instance() {
		static Mediator *mediatorInstance = new Mediator();
		return mediatorInstance;
	}

signals:
#if 0
    void bufferConnect(QVector<QVector2D>* data, Polygon* editedP);
    void polygonSelect(Polygon* polygon);
    void polygonAdd(Polygon* polygon);
#endif
	void onBufferConnect(QVector<QVector2D> *data, Polygon *editedP);
	void onAreaRepaint();
	void onPolygonSelect(Polygon *polygon);
	void onEditorReset();
	void onPolygonSave(Polygon *polygon, bool isNew);
	void onEdgeSelect(Edge* edge);
	void onVertexSelect(Vertex* vertex);
	void onPointHighlight(QPointF point);
	void onLineHighlight(QLineF line);
	void onPolygonHighlight(Polygon* poly);
	void onHighlightReset();
	void onEditingExit();
	void onZoom(QPointF oldPos, QPointF newPos);

	void onSaveToJson();
	void onLoadFromJson();

	// void addNewVertex(QPoint *point);
	// void editVertexMouse(int row);
	// void editVertexCoordMouse(int row, QPoint *new_coord);
	// void saveVertexMouse(int row);

private:
	Mediator() {}
};

class Area : public QWidget {
	Q_OBJECT
public:
	explicit Area(QWidget *parent = nullptr);

protected:
	void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	// void mouseMoveEvent(QMouseEvent *event) override;
	// void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
	void onBufferConnectReceived(QVector<QVector2D> *data, Polygon *editedP);
	void resetHighlight();
	void onPointHighlightReceived(QPointF point);
	void onLineHighlightReceived(QLineF line);
	void onAreaRepaintReceived();
	void onPolygonHighlightReceived(Polygon* poly);

private:
	void updateSize(double width, double height);

	// QScrollArea* scrollArea = qobject_cast<QScrollArea*>(parentWidget());
	QScrollArea* scrollArea;
	QVector<QVector2D> *bufferData = nullptr;
	Polygon *edited;
	Polygon *highlighted;
	QPointF pointH;
	QLineF lineH;
	const int coordOffset = 25;
	double scaleFactor = 1;
	const double scaleStep = 1.15;

	// int draggingVertex =
		// -1;			   // Index of dragged vertex (-1 if no vertex is dragged)
	// QPoint dragOffset; // Cursor offset relative to the vertex at the start of
					   // dragging
};

#endif // AREA_H
