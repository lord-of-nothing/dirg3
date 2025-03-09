#ifndef POLYGON_H
#define POLYGON_H

#include <QHash>
#include <QPair>
#include <QString>
#include <QUuid>
#include <QVector>

class Polygon {
public:
	Polygon() = default;
	Polygon(const QVector<QUuid> &v, const QVector<QUuid> &e,
			const QString &name, const int material,
			const int existingNumber = -1, const QUuid &existingId = QUuid(), const int layer = 0);

	static QUuid gen_uuid();
	inline auto id() const { return id_; }
	inline auto &name() const { return name_; }
	inline auto number() const { return cur_polygon_number_; }
	inline auto layer() const { return layer_; }
	inline auto material() const { return material_; }

	auto &next_vertex(const QUuid &current_vertex) const;
	auto &prev_vertex(const QUuid &current_vertex) const;

	auto &next_edge(const QUuid &current_edge) const;
	auto &prev_edge(const QUuid &current_edge) const;

	void delete_polygon();

	static inline int get_polygons_total() { return total_polygon_number; }
	static inline void reset_polygons_total() { total_polygon_number = 0; }

	QVector<QUuid> edges;
	QVector<QUuid> vertices;

private:
	QString name_;
	static inline int total_polygon_number = 0;
	int cur_polygon_number_;
	[[maybe_unused]] int material_;
	QUuid id_;
	int layer_;
};

inline QHash<QUuid, Polygon> all_polygons;

#endif
