#include "linkeditor.h"
#include "polygon.h"
#include "vertex.h"
#include "contact.h"
#include <QDialogButtonBox>
#include <QVBoxLayout>

LinkEditor::LinkEditor(const QVector<QVector<QUuid>>& possible_links, Polygon* p, QWidget* parent) : QDialog(parent) {
	listWidget = new QListWidget(this);
	listWidget->setSelectionMode(QAbstractItemView::MultiSelection);

	currentPolygon = p;
	possibleLinks = possible_links;

	auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(buttons, &QDialogButtonBox::accepted, this, &LinkEditor::onConfirm);
	connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

	auto layout = new QVBoxLayout;
	layout->addWidget(listWidget);
	layout->addWidget(buttons);
	setLayout(layout);

	setupLinkList();
}

void LinkEditor::setupLinkList() {
	listWidget->clear();

	for (const auto& elem : possibleLinks) {
		// Vertex& v1 = all_vertices[elem[0]];
		// Vertex& v2 = all_vertices[elem[1]];
		Polygon& p = all_polygons[elem[2]];
		QString text = p.name();
		QListWidgetItem* item = new QListWidgetItem(text);
		item->setData(Qt::UserRole, QVariant::fromValue(elem));

		if (currentPolygon->contacts.contains(elem[2])) {
			item->setSelected(true);
		}

		listWidget->addItem(item);
	}
}

void LinkEditor::save() {
	QVector<QUuid> newContacts;

	for (auto& elem : currentPolygon->contacts) {
		all_contacts[elem].delete_contact();
	}

	for (int i = 0; i < listWidget->count(); ++i) {
		QListWidgetItem* item = listWidget->item(i);

		if (!item->isSelected()) {
			continue;
		}

		QVector<QUuid> value = item->data(Qt::UserRole).value<QVector<QUuid>>();
		Contact c(value[0], value[1], currentPolygon->id(), value[2]);
		newContacts.append(c.id());
	}
}
