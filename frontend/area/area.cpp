#include "area.h"
#include "geometry.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPointF>
#include <QVector2D>
#include <QPolygonF>
#include <QScrollBar>

Area::Area(QWidget *parent) : QWidget{parent} {
	// connect(Mediator::instance(), &Mediator::bufferConnect, this,
	// &Area::onBufferConnectReceived); qWarning() << "constr";
	connect(Mediator::instance(), &Mediator::onBufferConnect, this,
			&Area::onBufferConnectReceived);
	connect(Mediator::instance(), &Mediator::onHighlightReset, this,
			&Area::resetHighlight);
	connect(Mediator::instance(), &Mediator::onPointHighlight, this,
			&Area::onPointHighlightReceived);
	connect(Mediator::instance(), &Mediator::onLineHighlight, this,
			&Area::onLineHighlightReceived);
	connect(Mediator::instance(), &Mediator::onPolygonHighlight, this,
			&Area::onPolygonHighlightReceived);

	// setMouseTracking(true); // For check mouse position
}

void Area::paintEvent([[maybe_unused]] QPaintEvent *event) {
	QPainter painter(this);
	painter.begin(this);
	painter.translate(coordOffset, coordOffset);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.scale(scaleFactor, scaleFactor);

	// рамка
	QPen axisPen;
	axisPen.setColor(Qt::gray);
	axisPen.setWidth(1);
	painter.setPen(axisPen);
	// painter.drawPolygon({QPointF(0, 0),
						// QPointF(width() - coordOffset, 0),
						// QPointF(width() - coordOffset, height() - coordOffset),
						// QPointF(0, height() - coordOffset)});
	painter.drawLine(QPointF(0, 0), QPointF(width() + coordOffset, 0));
	painter.drawLine(QPointF(0, 0), QPointF(0, height() + coordOffset));

	// точки на осях
	QFont axisFont;
	axisFont.setPointSize(8);
	painter.setFont(axisFont);
	int tickStep = 100;
	int tickHalfSize = 5;
	for (int curTick = 0; curTick < width(); curTick += tickStep) {
		painter.drawLine(curTick, -tickHalfSize, curTick, tickHalfSize);
		painter.drawText(curTick, -tickHalfSize, QString::number(curTick));
	}
	for (int curTick = tickStep; curTick < height(); curTick += tickStep) {
		painter.drawLine(-tickHalfSize, curTick, tickHalfSize, curTick);
		painter.drawText(-coordOffset, curTick + tickHalfSize, QString::number(curTick));
	}

	QPen normalPen;
	normalPen.setWidth(2.0);
	normalPen.setColor(Qt::black);
	painter.setPen(normalPen);

	// полигоны
	for (auto polygons = all_polygons.begin(); polygons != all_polygons.end();
		 ++polygons) {
		Polygon &polygon = polygons.value();
		if (edited == &polygon) {
			continue;
		}
		QVector<QUuid> &vertices = polygon.vertices;
		// qDebug() << vertices.size();
		for (int i = 0; i < vertices.size(); ++i) {
			Vertex &curV = all_vertices[vertices[i]];
			Vertex &nextV = all_vertices[vertices[(i + 1) % vertices.size()]];

			painter.drawEllipse(QPointF(curV.x(), curV.y()), 3.0, 3.0);
			painter.drawLine(QPointF(curV.x(), curV.y()),
							 QPointF(nextV.x(), nextV.y()));
		}
	}

	// редактируемый полигон
	if (!(bufferData == nullptr || bufferData->size() == 0)) {
		QPen editedPen;
		QPen dividerPen;
		editedPen.setWidth(4.0);
		editedPen.setColor(Qt::red);
		dividerPen.setWidth(4.0);
		dividerPen.setColor(Qt::black);
		painter.setPen(editedPen);

		for (int i = 0; i < bufferData->size(); ++i) {
			QVector3D point = bufferData[0][i];
			QVector3D nextPoint;
			if (i < bufferData->size() - 1) {
				nextPoint = bufferData[0][i + 1];
			} else {
				nextPoint = bufferData[0][0];
			}
			painter.drawEllipse(QPointF(point.x(), point.y()), 3.0, 3.0);
			// if (point[2]) {
				// painter.setPen(dividerPen);
				// painter.drawEllipse(QPointF(point.x(), point.y()), 6.0, 6.0);
				// painter.setPen(editedPen);
			// }
			painter.drawLine(QPointF(point.x(), point.y()),
							 QPointF(nextPoint.x(), nextPoint.y()))	;
		}
	}

	// выделенный в дереве
	QPen highlightPen;
	highlightPen.setColor(Qt::blue);
	highlightPen.setWidth(4.0);
	painter.setPen(highlightPen);

	if (highlighted != nullptr) {
		for (auto& edgeId : highlighted->edges) {
			Edge& edge = all_edges[edgeId];
			Vertex& v1 = all_vertices[edge.coords().first];
			Vertex& v2 = all_vertices[edge.coords().second];
			QLineF line({v1.x(), v1.y()}, {v2.x(), v2.y()});
			painter.drawLine(line);
		}
	}
	else if (pointH.x() != -1) {
		QBrush vertexBrush;
		vertexBrush.setStyle(Qt::SolidPattern);
		vertexBrush.setColor(Qt::blue);
		painter.setBrush(vertexBrush);
		painter.drawEllipse(pointH, 5.0, 5.0);
	} else if (lineH.length()) {
		painter.drawLine(lineH);
	}

	painter.end();
}

void Area::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		auto pos = mapFromGlobal(event->globalPosition().toPoint()) / scaleFactor;
		auto candidates = find_polygons_by_point({pos.x() - coordOffset, pos.y() - coordOffset});
		if (candidates.size() == 0) {
			// emit Mediator::instance()
				// -> addNewVertex(&pos); // If we have not selected any polygon,
									   // we create a new point.
			return;
		}

		auto &last_candidate = all_polygons[candidates.last()];
		auto vertices = last_candidate.vertices;

		// emit Mediator::instance()->onPolygonSelect(&all_polygons[candidates.back()]);
		emit Mediator::instance()
			-> onPolygonSelect(&last_candidate); // FIXME: Если заменить на
												 // last_coniddate, то ломается

		#if 0
		// Verification of click on the vertex
		for (int i = 0; i < vertices.size(); ++i) {
			QPoint cur_point_vertex{int(all_vertices[vertices.at(i)].x()),
									int(all_vertices[vertices.at(i)].y())};
			if (QRect(cur_point_vertex - QPoint(3, 3), QSize(10, 10))
					.contains(event->pos())) {
				qDebug() << "Click on point with coord: " << cur_point_vertex;
				draggingVertex = i;
				dragOffset = event->pos() - cur_point_vertex;
				emit Mediator::instance() -> editVertexMouse(draggingVertex);
				break;
			}
		}
		#endif
	}
}

void Area::wheelEvent(QWheelEvent *event) {
	auto mousePos = mapFromGlobal(event->globalPosition().toPoint());
	auto oldPos = event->position();

	double currentFactor = 1;
	if (event->angleDelta().y() > 0) {
		currentFactor = scaleStep;
	} else {
		currentFactor = 1.0 / scaleStep;
	}
	scaleFactor *= currentFactor;
	if (scaleFactor < 1) {
		scaleFactor = 1;
	} else if (scaleFactor > 3) {
		scaleFactor = 3;
	}
	updateSize(1000 * scaleFactor, 1000 * scaleFactor);

	// emit Mediator::instance()->onZoom(oldPos, mousePos * currentFactor);
	// repaint();
}

void Area::updateSize(double width, double height) {
	setMinimumSize(width, height);
	repaint();
}

#if 0
// В теории тут будем ещё отслеживать мышку, для изменения курсора при наведении
// на точку
void Area::mouseMoveEvent(QMouseEvent *event) {
	if (draggingVertex != -1) {
		auto new_coord = event->pos() - dragOffset;
		emit Mediator::instance() -> editVertexCoordMouse(draggingVertex,
														  &new_coord);
	}
}

void Area::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		if (draggingVertex != -1) {
			emit Mediator::instance() -> saveVertexMouse(draggingVertex);
		}
		draggingVertex = -1;
	}
}
#endif

void Area::onBufferConnectReceived(QVector<QVector3D> *data, Polygon *editedP) {
	if (bufferData == nullptr) {
		bufferData = data;
	}
	edited = editedP;
	repaint();
	// qWarning() << "signal";
}

void Area::resetHighlight() {
	highlighted = nullptr;
	pointH.setX(-1);
	pointH.setY(-1);
	lineH.setP1(QPointF(0, 0));
	lineH.setP2(QPointF(0, 0));
	repaint();
}

void Area::onPolygonHighlightReceived(Polygon* poly) {
	resetHighlight();
	highlighted = poly;
	repaint();
}

void Area::onPointHighlightReceived(QPointF point) {
	resetHighlight();
	pointH = point;
	repaint();
}

void Area::onLineHighlightReceived(QLineF line) {
	resetHighlight();
	lineH = line;
	repaint();
}

void Area::onAreaRepaintReceived() {
	repaint();
}
