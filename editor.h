#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QShowEvent>
#include <QTableWidget>
#include <QPushButton>
#include <QVector2D>
#include <QDockWidget>
#include "geometry.h"
#include "mainwindow.h"

namespace Ui {
class Editor;
}

class Editor : public QWidget
{
    Q_OBJECT

public:
    explicit Editor(QWidget *parent = nullptr);
    // void setupNew();
    void setupExistingPolygon(Polygon* polygon);
    ~Editor();

protected:
    void showEvent(QShowEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onBufferConnect();
    void onPolygonSelectReceived(Polygon* polygon);

private:
    void updateTableSize();
    void addVertex();
    void addVRow(int row, QString vName, double x, double y);
    void saveVertex(int row);
    void resetVertex(int row);
    void finishEditVertex(int row);
    void editVertex(int row);
    void clearNew();
    void savePolygon();
    void resetEditor();

    Ui::Editor *ui;
    QTableWidget* vtable;
    QPushButton* cancelBtn = nullptr;; // костыль
    QTableWidget* etable;
    QVector<QVector2D> buffer;
    QDockWidget* dock;
    MainWindow* mainWindow;
    int polygonNumber;
    Polygon* editedPolygon = nullptr;

    const double minCoord = 0.0;
    const double maxCoord = 1000.0;
    const int precision = 3;
};

#endif // EDITOR_H
