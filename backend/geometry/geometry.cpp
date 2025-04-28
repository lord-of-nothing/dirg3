#include "geometry.h"

#include <QRandomGenerator>
#include <QUuid>
#include <QPointF>

// bool check_convex(double first_x, double first_y, double second_x,
// 				  double second_y, double third_x, double third_y) {
// 	QPair<double, double> first_vector(second_x - first_x, second_y - first_y);
// 	QPair<double, double> second_vector(third_x - second_x, third_y - second_y);
// 	double mult = first_vector.first * second_vector.second -
// 				  second_vector.first * first_vector.second;
// 	return mult <= 0;
// }

double area(QPair<double, double> first_point, QPair<double, double> second_point,
			QPair<double, double> third_point) {
	double first_x = first_point.first;
	double first_y = first_point.second;

	double second_x = second_point.first;
	double second_y = second_point.second;

	double third_x = third_point.first;
	double third_y = third_point.second;

	return (second_x - first_x) * (third_y - first_y) -
		   (second_y - first_y) * (third_x - first_x);
}

bool intersect(double a, double b, double c, double d) {
	if (a > b) {
		std::swap(a, b);
	}
	if (c > d) {
		std::swap(c, d);
	}
	return std::max(a, c) <= std::min(b, d);
}

bool check_intersect(QPair<double, double> first_point,
					 QPair<double, double> second_point,
					 QPair<double, double> third_point,
					 QPair<double, double> fourth_point) {
	double first_x = first_point.first;
	double first_y = first_point.second;

	double second_x = second_point.first;
	double second_y = second_point.second;

	double third_x = third_point.first;
	double third_y = third_point.second;

	double fourth_x = fourth_point.first;
	double fourth_y = fourth_point.second;
	return intersect(first_x, second_x, third_x, fourth_x) &&
		   intersect(first_y, second_y, third_y, fourth_y) &&
		   area(first_point, second_point, third_point) *
				   area(first_point, second_point, fourth_point) <= 0 &&
		   area(third_point, fourth_point, first_point) *
				   area(third_point, fourth_point, second_point) <= 0;
}

// bool check_new_point(double first_x, double first_y, double second_x,
// 					 double second_y, double third_x, double third_y) {
// 	bool first_check =
// 		check_convex(first_x, first_y, second_x, second_y, third_x, third_y);
// 	bool second_check = true;

// 	for (auto pair = all_edges.begin(); pair != all_edges.end(); ++pair) {
// 		double old_first_x = all_vertices[pair.value().coords().first].x();
// 		double old_first_y = all_vertices[pair.value().coords().first].y();
// 		double old_second_x = all_vertices[pair.value().coords().second].x();
// 		double old_second_y = all_vertices[pair.value().coords().second].y();
// 		if (check_intersect(old_first_x, old_first_y, old_second_x,
// 							old_second_y, second_x, second_y, third_x,
// 							third_y) == false) {
// 			second_check = false;
// 		}
// 	}

// 	return first_check && second_check;
// }

bool point_in_polygon(QPair<double, double> point, QUuid polygon_id) {
	double vector_x = 40000 + QRandomGenerator::global()->bounded(0, 2000);
	double vector_y = 40000 + QRandomGenerator::global()->bounded(0, 2000);
	int count_intersection = 0;
	for (auto &edge : all_polygons[polygon_id].edges) {
		if (check_intersect(
				point, {vector_x, vector_y},
				{all_vertices[all_edges[edge].coords().first].x(),
				 all_vertices[all_edges[edge].coords().first].y()},
				{all_vertices[all_edges[edge].coords().second].x(),
				 all_vertices[all_edges[edge].coords().second].y()})) {
			count_intersection++;
		}
	}
	return count_intersection % 2 == 1;
}

QVector<QUuid> find_polygons_by_point(QPair<double, double> point) {
	QVector<QUuid> polygons;

	for (auto polygon = all_polygons.begin(); polygon != all_polygons.end();
		 ++polygon) {
		if (point_in_polygon(point, polygon.key())) {
			polygons.append(polygon.key());
		}
	}
	return polygons;
}

double cross(QPair<double, double> first_vector,
			 QPair<double, double> second_vector) {
	return first_vector.first * second_vector.second -
		   second_vector.first * first_vector.second;
}

bool checkConvex(QVector<QPair<double, double>> vertices) {
	double init = cross({vertices[1].first - vertices[0].first,
						 vertices[1].second - vertices[0].second},
						{vertices[2].first - vertices[1].first,
						 vertices[2].second - vertices[1].second});
	for (int i = 1; i <= vertices.size(); i++) {
		double current_cross = cross(
			{vertices[i % vertices.size()].first - vertices[i - 1].first,
			 vertices[i % vertices.size()].second - vertices[i - 1].second},
			{vertices[(i + 1) % vertices.size()].first - vertices[i % vertices.size()].first,
			 vertices[(i + 1) % vertices.size()].second - vertices[i % vertices.size() ].second});
		if (init * current_cross <= 0) {
			return false;
		}
	}
	return true;
}

bool checkNotIntersecting(QVector<QPair<double, double>> vertices) {
	for (int i = 0; i < vertices.size(); i++) {
		for (auto pair = all_edges.begin(); pair != all_edges.end(); ++pair) {
			double old_first_x = all_vertices[pair.value().coords().first].x();
			double old_first_y = all_vertices[pair.value().coords().first].y();
			double old_second_x =
				all_vertices[pair.value().coords().second].x();
			double old_second_y =
				all_vertices[pair.value().coords().second].y();
			if (check_intersect({old_first_x, old_first_y}, {old_second_x, old_second_y},
								{vertices[i].first, vertices[i].second},
								{vertices[(i + 1) % vertices.size()].first, vertices[(i + 1) % vertices.size()].second})) {
				return false;
			}
		}
	}
	return true;
}

bool checkUniqueName(const QVector<QString>& names) {
	QSet<QString> temp;
	for (int i = 0; i < names.size(); ++i){
		temp.insert(names[i]);
	}
	if (temp.size() != names.size()) {
		return false;
	}
	for (int i = 0; i < names.size(); ++i) {
		if (all_names.contains(names[i])) {
			return false;
		}
	}
	return true;
}

bool checkNestingInOnceLayer(QVector<QPair<double, double>> vertices, int layer) {
	for (auto pair = all_polygons.begin(); pair != all_polygons.end(); ++pair) {
		if (pair.value().layer() == layer) {
			for (auto vertex : vertices) {
				if (point_in_polygon(vertex, pair.key())) {
					return false;
				}
			}
		}
	}
	return true;
}

// bool checkPolygon(QVector<QPair<double, double>> vertices) {
// 	return checkConvex(vertices) && checkNotIntersecting(vertices);
// }

auto possible_links(QUuid first_polygon, QUuid second_polygon) {
	for (QUuid edge : all_polygons[first_polygon].edges){
		QPair<QUuid, QUuid> edge_coords = all_edges[edge].coords();
		double length_edge = std::sqrt((all_vertices[edge_coords.second].x() - all_vertices[edge_coords.first].x()) * (all_vertices[edge_coords.second].x() - all_vertices[edge_coords.first].x()) + (all_vertices[edge_coords.second].y() - all_vertices[edge_coords.first].y()) * (all_vertices[edge_coords.second].y() - all_vertices[edge_coords.first].y()));
		QUuid vertex_on_edge;
		QUuid vertex_on_line;
		for (QUuid vertex : all_polygons[second_polygon].vertices) {
			double first_dist = std::sqrt((all_vertices[vertex].x() - all_vertices[edge_coords.first].x()) * (all_vertices[vertex].x() - all_vertices[edge_coords.first].x()) + (all_vertices[vertex].y() - all_vertices[edge_coords.first].y()) * (all_vertices[vertex].y() - all_vertices[edge_coords.first].y()));
			double second_dist = std::sqrt((all_vertices[vertex].x() - all_vertices[edge_coords.second].x()) * (all_vertices[vertex].x() - all_vertices[edge_coords.second].x()) + (all_vertices[vertex].y() - all_vertices[edge_coords.second].y()) * (all_vertices[vertex].y() - all_vertices[edge_coords.second].y()));
			if (std::abs(length_edge - first_dist - second_dist) < std::numeric_limits<double>::epsilon()){
				vertex_on_edge = vertex;
			} else if (std::abs(2 * std::max(length_edge, std::max(first_dist, second_dist)) - length_edge - first_dist - second_dist) < std::numeric_limits<double>::epsilon()) {
				vertex_on_edge = vertex;
			}
		}
		if (!vertex_on_edge.isNull() && !vertex_on_line.isNull()) {
			QPair<QUuid, QUuid> result = {vertex_on_edge, vertex_on_line};
			return result;
		}
	}
	QPair<QUuid, QUuid> result;
	return result;
}

QVector<QPair<QUuid, QUuid>> all_possible_links(QUuid polygon) {
	QVector<QPair<QUuid, QUuid>> links;
	for (QUuid poly : all_polygons.keys()) {
		if (poly != polygon) {
			QPair<QUuid, QUuid> link = possible_links(polygon, poly);
			if (!link.first.isNull() && !link.second.isNull()) {
				links.push_back(link);
			}
		}
	}
	return links;
}

double lengthSides(QUuid first_vertex, QUuid second_vertex) {
	double dx = all_vertices[first_vertex].x() - all_vertices[second_vertex].x();
	double dy = all_vertices[first_vertex].y() - all_vertices[second_vertex].y();
	return std::sqrt(dx * dx + dy * dy);
}

QVector<QPointF> splitCurve(QVector<QUuid> vertices, int fineness) {
	QVector<QPointF> result_split;

	QVector<double> current_lengths;
	current_lengths.push_back(0.0);
	for (int i = 1; i < vertices.size(); i++) {
		double len = lengthSides(vertices[i - 1], vertices[i]);
		current_lengths.push_back(current_lengths[current_lengths.size() - 1] + len);
	}

	double all_length = current_lengths[current_lengths.size() - 1];
	double segment_length = all_length / fineness;

	result_split.push_back({all_vertices[vertices[0]].x(), all_vertices[vertices[0]].y()});

	for (int m = 1; m < fineness; m++) {
		double cur_d = m * segment_length;

		int i = 0;
		while (i < current_lengths.size() - 1 && current_lengths[i + 1] < cur_d) {
			i++;
		}

		if (i >= current_lengths.size() - 1) {
			break;
		}

		double remainder = cur_d - current_lengths[i];
		double cur_segment_len = current_lengths[i + 1] - current_lengths[i];

		double t = remainder / cur_segment_len;
		QPointF point;
		point.setX(all_vertices[vertices[i]].x() + t * (all_vertices[vertices[i + 1]].x() - all_vertices[vertices[i]].x()));
		point.setY(all_vertices[vertices[i]].y() + t * (all_vertices[vertices[i + 1]].y() - all_vertices[vertices[i]].y()));
		result_split.push_back(point);
	}

	result_split.push_back({all_vertices[vertices[vertices.size() - 1]].x(), all_vertices[vertices[vertices.size() - 1]].y()});
	return result_split;
}

QPointF point_intersection(QPointF a1, QPointF a2, QPointF b1, QPointF b2) {
	double dx1 = a2.x() - a1.x();
	double dy1 = a2.y() - a1.y();
	double dx2 = b2.x() - b1.x();
	double dy2 = b2.y() - b1.y();

	double det = dx1 * dy2 - dx2 * dy1;
	double t_numerator = (b1.x() - a1.x()) * dy2 - (b1.y() - a1.y()) * dx2;
	double t = t_numerator / det;

	return {a1.x() + t * dx1, a1.y() + t * dy1};

}

QVector<QPointF> gridPolygon(const QVector<QUuid> &vertices, QVector<QUuid> sep, int fineness) {
	QVector<QUuid> first_side;
	QVector<QUuid> second_side;
	QVector<QUuid> third_side;
	QVector<QUuid> fourth_side;

	int index_first = 0;
	int index_second = 0;
	int index_third = 0;
	int index_fourth = 0;

	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i] == sep[0]) {
			index_first = i;
		}
		if (vertices[i] == sep[1]) {
			index_second = i;
		}
		if (vertices[i] == sep[2]) {
			index_third = i;
		}
		if (vertices[i] == sep[3]) {
			index_fourth = i;
		}

	}

	for (int i = index_first; i <= index_second; i++) {
		first_side.push_back(vertices[i]);
	}

	for (int i = index_second; i <= index_third; i++) {
		second_side.push_back(vertices[i]);
	}

	for (int i = index_third; i <= index_fourth; i++) {
		third_side.push_back(vertices[i]);
	}

	for (int i = index_fourth; i < vertices.size(); i++) {
		fourth_side.push_back(vertices[i]);
	}

	for (int i = 0; i <= index_first; i++) {
		fourth_side.push_back(vertices[i]);
	}

	QVector<QPointF> first_side_grid = splitCurve(first_side, fineness);
	QVector<QPointF> second_side_grid = splitCurve(second_side, fineness);
	QVector<QPointF> third_side_grid = splitCurve(third_side, fineness);
	QVector<QPointF> fourth_side_grid = splitCurve(fourth_side, fineness);

	QVector<QPointF> grid;

	for (int i = 0; i < first_side_grid.size(); i++) {
		for (int j = 0; j < second_side_grid.size(); j++) {
			grid.push_back(point_intersection(first_side_grid[i], third_side_grid[third_side_grid.size() - i - 1], second_side_grid[j], fourth_side_grid[fourth_side_grid.size() - j - 1]));
		}
	}

	return grid;
}

QVector<QPair<QPointF, QPointF>> gridLine(const QVector<QUuid> &vertices, QVector<QUuid> sep, int fineness) {
	QVector<QUuid> first_side;
	QVector<QUuid> second_side;
	QVector<QUuid> third_side;
	QVector<QUuid> fourth_side;

	int index_first = 0;
	int index_second = 0;
	int index_third = 0;
	int index_fourth = 0;

	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i] == sep[0]) {
			index_first = i;
		}
		if (vertices[i] == sep[1]) {
			index_second = i;
		}
		if (vertices[i] == sep[2]) {
			index_third = i;
		}
		if (vertices[i] == sep[3]) {
			index_fourth = i;
		}

	}

	for (int i = index_first; i <= index_second; i++) {
		first_side.push_back(vertices[i]);
	}

	for (int i = index_second; i <= index_third; i++) {
		second_side.push_back(vertices[i]);
	}

	for (int i = index_third; i <= index_fourth; i++) {
		third_side.push_back(vertices[i]);
	}

	for (int i = index_fourth; i < vertices.size(); i++) {
		fourth_side.push_back(vertices[i]);
	}

	for (int i = 0; i <= index_first; i++) {
		fourth_side.push_back(vertices[i]);
	}

	QVector<QPointF> first_side_grid = splitCurve(first_side, fineness);
	QVector<QPointF> second_side_grid = splitCurve(second_side, fineness);
	QVector<QPointF> third_side_grid = splitCurve(third_side, fineness);
	QVector<QPointF> fourth_side_grid = splitCurve(fourth_side, fineness);

	QVector<QPair<QPointF, QPointF>> gridLines;

	for (int i = 0; i < first_side_grid.size(); i++) {
		gridLines.push_back({first_side_grid[i], third_side_grid[third_side_grid.size() - i - 1]});
	}
	for (int i = 0; i < second_side_grid.size(); i++) {
		gridLines.push_back({second_side_grid[i], fourth_side_grid[fourth_side_grid.size() - i - 1]});
	}

	return gridLines;
}



