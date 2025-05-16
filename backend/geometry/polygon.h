#ifndef POLYGON_H
#define POLYGON_H

#include <QHash>
#include <QPair>
#include <QString>
#include <QUuid>
#include <QVector>
#include <QPointF>

class Polygon {
public:
	Polygon() = default;
	Polygon(const QVector<QUuid> &v, const QVector<QUuid> &e,
			const QString &name, const int material, QVector<QUuid> separators,
			const int existingNumber = -1, const QUuid &existingId = QUuid(), const int fineness = 1);

	static QUuid gen_uuid();
	inline auto id() const { return id_; }
	inline auto &name() const { return name_; }
	inline auto number() const { return cur_polygon_number_; }
	inline auto layer() const { return layer_; }
	inline auto material() const { return material_; }
	inline auto fineness() const { return fineness_; }
	inline auto separators() const { return separators_; }
	inline void set_name(QString name) {name_ = name;}
	inline void set_material(int material) {material_ = material;}
	void set_polygon(QVector<QPair<double, double>> new_vertices, QVector<QString> new_vertices_name, QVector<QString> new_edges_name, QVector<int> new_edges_properties);

	// inline auto grid() const { return grid_; }
	const QVector<QPointF> grid() { return grid_; }
	const QVector<QPair<QPointF, QPointF>> grid_lines() { return grid_lines_; }


	auto &next_vertex(const QUuid &current_vertex) const;
	auto &prev_vertex(const QUuid &current_vertex) const;

	auto &next_edge(const QUuid &current_edge) const;
	auto &prev_edge(const QUuid &current_edge) const;

	void set_vertices(const QUuid &old_vertex, const QUuid &new_vertex);

	void delete_polygon();

	static inline int get_polygons_total() { return total_polygon_number; }
	static inline void reset_polygons_total() { total_polygon_number = 0; }
	

	
	QVector<QUuid> edges;
	QVector<QUuid> vertices;
	QVector<QUuid> contacts;
	QVector<QUuid> unused_vertices;

private:
	QString name_;
	static inline int total_polygon_number = 0;
	int cur_polygon_number_;
	[[maybe_unused]] int material_;
	QUuid id_;
	int layer_;
	int fineness_;
	QVector<QUuid> separators_;
	QVector<QPointF> grid_;
	QVector<QPair<QPointF, QPointF>> grid_lines_;
};

inline QHash<QUuid, Polygon> all_polygons;

#endif
