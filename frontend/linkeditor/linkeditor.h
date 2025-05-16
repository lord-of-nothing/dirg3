#ifndef LINKEDITOR_H
#define LINKEDITOR_H

#include <QDialog>
#include <QListWidget>

class LinkEditor : public QDialog {
	Q_OBJECT

public:
	LinkEditor(const QVector<QVector<QUuid>>& links, QWidget* parent = nullptr);
	void setupLinkList(const QVector<QVector<QUuid>>& links);
private:
	QListWidget *listWidget;
};

#endif // LINKEDITOR_H
