#ifndef CONTACT_H
#define CONTACT_H

#include <QHash>
#include <QString>
#include <QUuid>
#include <QVector>

class Contact {
public:
	Contact() = default;
	Contact(QUuid first_vertex, QUuid second_vertex, QUuid first_polygon, QUuid second_polygon);

	static QUuid gen_uuid();

	inline auto &id() const { return id_; }
	inline auto &first_vertex() const { return first_vertex_; }
	inline auto &second_vertex() const { return second_vertex_; }
	inline auto &first_polygon() const { return first_polygon_; }
	inline auto &second_polygon() const { return second_polygon_; }
	void delete_contact();


private:
	QUuid id_;
	QUuid first_vertex_;
	QUuid second_vertex_;
	QUuid first_polygon_;
	QUuid second_polygon_;
};

inline QHash<QUuid, Contact> all_contacts;
#endif // CONTACT_H
