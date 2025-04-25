#include "polygon.h"
#include "edge.h"
#include "vertex.h"
#include "geometry.h"

QUuid Polygon::gen_uuid() {
	QUuid uuid_str = QUuid::createUuid();
	while (all_polygons.find(uuid_str) != all_polygons.end()) {
		uuid_str = QUuid::createUuid();
	}
	return uuid_str;
}

Polygon::Polygon(const QVector<QUuid> &vertices, const QVector<QUuid> &edges,
				 const QString &name, const int material, QVector<QUuid> separators,
				 const int existingNumber, const QUuid &existingId, const int fineness)
	: edges(edges), vertices(vertices), name_(name),
	  cur_polygon_number_(existingNumber), material_(material), fineness_(fineness), separators_(separators) {
	id_ = (existingId.isNull()) ? gen_uuid() : existingId;
	all_names.insert(name);

	for (auto &edge : edges) {
		all_edges[edge].add_polygon(id_);
	}
	for (auto &vertice : vertices) {
		all_vertices[vertice].add_polygon(id_);
	}

	total_polygon_number++;
	grid_ = gridPolygon(vertices, separators, fineness);
	grid_lines_ = gridLine(vertices, separators, fineness);

	all_polygons[id_] = *this;
}

auto &Polygon::next_vertex(const QUuid &current_vertex) const {
	auto begin = vertices.begin(), end = vertices.end();
	auto it = std::find(begin, end, current_vertex);
	if (it == end) {
		throw std::runtime_error("edge not found");
	}
	auto dist = std::distance(begin, it);
	return vertices[(dist + 1) % vertices.size()];
}

auto &Polygon::prev_vertex(const QUuid &current_vertex) const {
	auto begin = vertices.begin(), end = vertices.end();
	auto it = std::find(begin, end, current_vertex);
	if (it == end) {
		throw std::runtime_error("edge not found");
	}
	int dist = std::distance(begin, it);
	return vertices[(dist - 1) % vertices.size()];
}

auto &Polygon::next_edge(const QUuid &current_edge) const {
	auto begin = edges.begin(), end = edges.end();
	auto it = std::find(begin, end, current_edge);
	if (it == end) {
		throw std::runtime_error("edge not found");
	}
	auto dist = std::distance(begin, it);
	return edges[(dist + 1) % edges.size()];
}

auto &Polygon::prev_edge(const QUuid &current_edge) const {
	auto begin = edges.begin(), end = edges.end();
	auto it = std::find(begin, end, current_edge);
	if (it == end) {
		throw std::runtime_error("edge not found");
	}
	int dist = std::distance(begin, it);
	return edges[(dist - 1) % edges.size()];
}

void Polygon::delete_polygon() {
	QUuid polygon_id = id_;
	std::for_each(vertices.begin(), vertices.end(),
				  [](const auto &vertex) { all_vertices.remove(vertex); });

	std::for_each(edges.begin(), edges.end(),
				  [](const auto &edge) { all_edges.remove(edge); });

	all_polygons.remove(polygon_id);
}
