#include "contact.h"
#include "vertex.h"
#include "polygon.h"



QUuid Contact::gen_uuid() {
	QUuid uuid_str = QUuid::createUuid();
	while (all_contacts.find(uuid_str) != all_contacts.end()) {
		uuid_str = QUuid::createUuid();
	}
	return uuid_str;
}

Contact::Contact(QUuid first_vertex, QUuid second_vertex, QUuid first_polygon, QUuid second_polygon) : first_vertex_(first_vertex), second_vertex_(second_vertex), first_polygon_(first_polygon), second_polygon_(second_polygon){
	id_ = gen_uuid();
	all_contacts[id_] = *this;
	all_vertices[first_vertex].contacts.push_back(id_);
	all_vertices[second_vertex].contacts.push_back(id_);
	all_polygons[first_polygon].contacts.push_back(id_);
	all_polygons[second_polygon].contacts.push_back(id_);
}

void Contact::delete_contact() {

}
