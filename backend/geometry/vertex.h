#ifndef VERTEX_H
#define VERTEX_H

#include <algorithm>

#include <QHash>
#include <QString>
#include <QUuid>
#include <QVector>

class Vertex {
public:
	Vertex() = default;
	Vertex(const double x, const double y, const QString &name);

	static QUuid gen_uuid();

	inline double x() const { return x_; }
	inline double y() const { return y_; }
	inline auto &id() const { return id_; }
	inline auto &name() const { return name_; }
	inline void set_x(double x) {x_ = x;};
	inline void set_y(double y) {y_ = y;};
	inline void set_name(QString name) {name_ = name;};




	inline void add_edge(const QUuid &edge_id) { edges.append(edge_id); }
	inline void remove_edge(const QUuid &edge_id) {
		edges.erase(std::remove(edges.begin(), edges.end(), edge_id),
					edges.end());
	}

	inline void add_polygon(const QUuid &polygon_id) {
		polygons.append(polygon_id);
	}
	inline void remove_polygon(const QUuid &polygon_id) {
		polygons.erase(
			std::remove(polygons.begin(), polygons.end(), polygon_id),
			polygons.end());
	}

#if DELETE_VERTEX
	void delete_vertex();
#endif

	QVector<QUuid> polygons;
	QVector<QUuid> edges;
	QVector<QUuid> contacts;

private:
	double x_;
	double y_;
	QUuid id_;
	QString name_;
};

inline QHash<QUuid, Vertex> all_vertices;

#endif
