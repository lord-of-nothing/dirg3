#include "linkeditor.h"
#include "polygon.h"
#include "vertex.h"
#include <QDialogButtonBox>
#include <QVBoxLayout>

LinkEditor::LinkEditor(const QVector<QVector<QUuid>>& links, QWidget* parent) : QDialog(parent) {
	listWidget = new QListWidget(this);
	listWidget->setSelectionMode(QAbstractItemView::MultiSelection);

	auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

	auto layout = new QVBoxLayout;
	layout->addWidget(listWidget);
	layout->addWidget(buttons);
	setLayout(layout);

	setupLinkList(links);
}

void LinkEditor::setupLinkList(const QVector<QVector<QUuid>>& links) {
	listWidget->clear();
	for (const auto& elem : links) {
		// Vertex& v1 = all_vertices[elem[0]];
		// Vertex& v2 = all_vertices[elem[1]];
		Polygon& p = all_polygons[elem[2]];
		QString text = p.name();
		QListWidgetItem* item = new QListWidgetItem(text);
		item->setData(Qt::UserRole, QVariant::fromValue(elem));

		listWidget->addItem(item);
	}
}
