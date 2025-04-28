#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QHash>
#include <QSet>
#include <QPair>
#include <QString>
#include <QUuid>
#include <QVector>
#include <map>
#include <QPointF>

#include "edge.h"
#include "polygon.h"
#include "vertex.h"

// bool check_new_point(double first_x, double first_y, double second_x,
					 // double second_y, double third_x, double third_y);
// bool check_convex(double first_x, double first_y, double second_x,
				  // double second_y, double third_x, double third_y);
double area(QPair<double, double> first_point,
			QPair<double, double> second_point,
			QPair<double, double> third_point);

bool intersect(double a, double b, double c, double d);

bool check_intersect(QPair<double, double> first_point,
					 QPair<double, double> second_point,
					 QPair<double, double> third_point,
					 QPair<double, double> fourth_point);

bool point_in_polygon(QPair<double, double> point, QUuid polygon_id);

QVector<QUuid> find_polygons_by_point(QPair<double, double> point);

double cross(QPair<double, double> first_vector,
			 QPair<double, double> second_vector);

bool checkConvex(QVector<QPair<double, double>> vertices);

bool checkNotIntersecting(QVector<QPair<double, double>> vertices);

bool checkUniqueName(const QVector<QString>& names);

bool checkNestingInOnceLayer(QVector<QPair<double, double>> vertices, int layer);

auto possible_links(QUuid first_polygon, QUuid second_polygon);

QVector<QPair<QUuid, QUuid>> all_possible_links(QUuid polygon);

double lengthSides(QUuid first_vertex, QUuid second_vertex);

QVector<QPointF> splitCurve(QVector<QUuid> vertices, int fineness);

QPointF point_intersection(QPointF a1, QPointF a2, QPointF b1, QPointF b2);

QVector<QPointF> gridPolygon(const QVector<QUuid> &vertices, QVector<QUuid> sep, int fineness);

QVector<QPair<QPointF, QPointF>> gridLine(const QVector<QUuid> &vertices, QVector<QUuid> sep, int fineness);


// bool checkPolygon(QVector<QPair<double, double>> vertices);

inline QSet<QString> all_names;

#endif
