#include "vertex.h"
#include "geometry.h"

QUuid Vertex::gen_uuid() {
	QUuid uuid_str = QUuid::createUuid();
	while (all_vertices.find(uuid_str) != all_vertices.end()) {
		uuid_str = QUuid::createUuid();
	}
	return uuid_str;
}

Vertex::Vertex(const double x, const double y, const QString &name)
	: x_(x), y_(y), name_(name) {
	id_ = gen_uuid();
	all_vertices[id_] = *this;
	all_names.insert(name);
}

#if DELETE_VERTEX
void Vertex::delete_vertex() {
	for (int i = 0; i < polygons.size(); ++i) {
		int index = 0;
		int count_vertices = all_polygons[polygons[i]].get_vertices().size();
		for (int j = 0; j < count_vertices; j++) {
			if (id == all_polygons[polygons[i]].get_vertices()[j]) {
				index = j;
				all_polygons[polygons[i]].get_vertices().erase(
					all_polygons[polygons[i]].get_vertices().begin() + j);
				break;
			}
		}
		if (index > 0 && index <= count_vertices - 1) {
			Edge new_edge =
				Edge(all_edges[all_polygons[polygons[i]].get_edges()[index - 1]]
						 .get_coords()
						 .first,
					 all_edges[all_polygons[polygons[i]].get_edges()[index]]
						 .get_coords()
						 .second,
					 0);
			all_edges[new_edge.get_id()] = new_edge;
			all_polygons[polygons[i]].get_edges()[index - 1] =
				new_edge.get_id();
			all_polygons[polygons[i]].get_edges().erase(
				all_polygons[polygons[i]].get_edges().begin() + index);
		} else if (index == 0) {
			Edge new_edge =
				Edge(all_edges[all_polygons[polygons[i]]
								   .get_edges()[count_vertices - 1]]
						 .get_coords()
						 .first,
					 all_edges[all_polygons[polygons[i]].get_edges()[0]]
						 .get_coords()
						 .second,
					 0);
			all_edges[new_edge.get_id()] = new_edge;
			all_polygons[polygons[i]].get_edges()[0] = new_edge.get_id();
			all_polygons[polygons[i]].get_edges().pop_back();
		}
	}
	all_vertices.erase(id);
}
#endif
