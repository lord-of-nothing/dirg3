#include "linkeditor.h"
#include "vertex.h"
#include <QDialogButtonBox>
#include <QVBoxLayout>

LinkEditor::LinkEditor(const QVector<QPair<QUuid, QUuid>>& links, QWidget* parent) : QDialog(parent) {
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

void LinkEditor::setupLinkList(const QVector<QPair<QUuid, QUuid>>& links) {
	listWidget->clear();
	for (const auto& elem : links) {
		Vertex& v1 = all_vertices[elem.first];
		Vertex& v2 = all_vertices[elem.second];
		QString text = v1.name() + " and " + v2.name();
		QListWidgetItem* item = new QListWidgetItem(text);
		item->setData(Qt::UserRole, QVariant::fromValue(elem));
	}
}
