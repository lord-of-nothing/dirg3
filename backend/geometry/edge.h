#ifndef EDGE_H
#define EDGE_H

#include <algorithm>

#include <QHash>
#include <QPair>
#include <QString>
#include <QUuid>
#include <QVector>

inline QUuid shite;

class Edge {
public:
	Edge() = default;
	Edge(const QUuid &start, const QUuid &finish, const QString &name,
		 int property);

	static QUuid gen_uuid();

	inline auto id() const { return id_; }
	inline auto &name() const { return name_; }
	inline auto &coords() const { return coords_; }
	inline auto get_property() const { return property_; }
	inline void set_property(int property) {property_ = property;}
	inline void set_name(QString name) {name_ = name;}
	inline void set_coords(QPair<QUuid, QUuid> coords) {coords_ = coords;}

	inline void add_polygon(const QUuid &polygon_id) {
		polygons.append(polygon_id);
	}
	inline void remove_polygon(const QUuid &polygon_id) {
		polygons.erase(
			std::remove(polygons.begin(), polygons.end(), polygon_id),
			polygons.end());
	}

	QVector<QUuid> polygons;

private:
	int property_;
	QUuid id_;
	QString name_;
	QPair<QUuid, QUuid> coords_{shite, shite};
};

inline QHash<QUuid, Edge> all_edges;

#endif
