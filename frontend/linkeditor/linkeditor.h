#ifndef LINKEDITOR_H
#define LINKEDITOR_H

#include <QDialog>
#include <QListWidget>
#include "polygon.h"

class LinkEditor : public QDialog {
	Q_OBJECT

public:
	LinkEditor(const QVector<QVector<QUuid>>& links, Polygon* p, QWidget* parent = nullptr);
	void setupLinkList();
	void save();
private slots:
	void onConfirm() {
		save();
		accept();
	}
private:
	QListWidget *listWidget;
	Polygon* currentPolygon;
	QVector<QVector<QUuid>> possibleLinks;
};

#endif // LINKEDITOR_H
