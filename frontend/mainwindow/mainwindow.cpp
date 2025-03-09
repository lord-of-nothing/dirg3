#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "editor.h"

#include <QPalette>
#include <QAction>
#include <QMenu>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);

	ui->editorDock->setWidget(new Editor(ui->editorDock)); // ?? -- а что не так?

	connect(Mediator::instance(), &Mediator::onPolygonSelect, this, &MainWindow::onPolygonSelectReceived);
	connect(Mediator::instance(), &Mediator::onEditorReset, this, &MainWindow::onEditingExitReceived);
	connect(Mediator::instance(), &Mediator::onResetEverything, this, &MainWindow::resetEverything);

	connect(ui->editorDock, &QDockWidget::visibilityChanged, this, [this]() {
		if (!ui->editorDock->isVisible()) {
			emit Mediator::instance() -> onEditorReset();
		}
	});
	ui->editorDock->close();

	ui->tree->setHeaderHidden(true);
	connect(ui->newPolygonBtn, &QPushButton::released, this,
			&MainWindow::newPolygon);

	// new file
	connect(ui->actionNew_file, &QAction::triggered, this,
			[this]() { emit Mediator::instance()->onResetEverything(); });
	// save to json
	connect(ui->actionSave_As, &QAction::triggered, this,
			[this]() { emit Mediator::instance()->onSaveToJson(); });
	// load from json
	connect(ui->actionOpen_File, &QAction::triggered, this,
			[this]() { emit Mediator::instance()->onLoadFromJson(); });

	// double click (open editor)
	connect(ui->tree, &QTreeWidget::itemDoubleClicked, this,
			[this](QTreeWidgetItem *item, [[maybe_unused]] int column) {
				QUuid itemId = item->data(0, Qt::UserRole).value<QUuid>();
				if (all_polygons.contains(itemId)) {
					selectPolygon(itemId);
					return;
				}
				QUuid polygonId = item->parent()->parent()->data(0, Qt::UserRole).value<QUuid>();
				selectPolygon(polygonId);
				if (all_vertices.contains(itemId)) {
					// selectVertex(id);
					emit Mediator::instance()->onVertexSelect(&all_vertices[itemId]);
				} else {
					// selectEdge(id);
					emit Mediator::instance()->onEdgeSelect(&all_edges[itemId]);
				}
			});

	// selection change (vertex/edge highlighting)
	connect(ui->tree, &QTreeWidget::itemSelectionChanged,
			this, [this]() {
				if (ui->editorDock->isVisible()) {
					return;
				}
				emit Mediator::instance()->onHighlightReset();
				auto* item = ui->tree->selectedItems()[0];
				if (item->parent()) {
					if (item->parent()->parent()) {
						QUuid id = item->data(0, Qt::UserRole).value<QUuid>();
						if (all_vertices.contains(id)) {
							Vertex& v = all_vertices[id];
							emit Mediator::instance()->onPointHighlight(QPointF(v.x(), v.y()));
						} else if (all_edges.contains(id)){
							Edge& e = all_edges[id];
							Vertex& v1 = all_vertices[e.coords().first];
							Vertex& v2 = all_vertices[e.coords().second];
							emit Mediator::instance()->onLineHighlight(QLineF(QPointF(v1.x(), v1.y()),
																				QPointF(v2.x(), v2.y())));
						}
					}
				} else {
					QUuid id = item->data(0, Qt::UserRole).value<QUuid>();
					emit Mediator::instance()->onPolygonHighlight(&all_polygons[id]);
				}
			});

	// context menu
	connect(ui->tree, &QTreeWidget::customContextMenuRequested, this,
			[this] (const QPoint& pos) {
				auto* item = ui->tree->itemAt(pos);
				if (item) {
					if (!item->parent()) {
						QMenu contextMenu;
						QUuid id = item->data(0, Qt::UserRole).value<QUuid>();

						QAction *deletePolygonAction = contextMenu.addAction("Delete");
						connect(deletePolygonAction, &QAction::triggered, this,
								[this, id] () { all_polygons[id].delete_polygon();
									removePolygon(id);
					emit Mediator::instance()->onEditorReset();
								// emit Mediator::instance()->onAreaRepaint();
								});

						contextMenu.exec(ui->tree->viewport()->mapToGlobal(pos));
					}
				}
			});

	connect(Mediator::instance(), &Mediator::onPolygonSave, this,
			[this](Polygon *polygon, bool isNew) {
				if (!isNew) {
					removePolygon(polygon->id());
				}
				addPolygon(polygon);
			});
	ui->tree->setSortingEnabled(true);
	ui->tree->sortByColumn(0, Qt::AscendingOrder);

	setWindowTitle("Grid Editor");
	setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);

	QColor gray(229, 228, 226);
	QPalette pal = QPalette();
	pal.setColor(QPalette::Window, gray);

	ui->editorDock->setAutoFillBackground(true);
	ui->editorDock->setPalette(pal);
	ui->treeDock->setAutoFillBackground(true);
	ui->treeDock->setPalette(pal);

	setStyleSheet("QMainWindow::separator{ width: 0px; height: 0px; }");
	// setDockOptions(QMainWindow::GroupedDockWidgets);

	// Exit by Ctrl+Q
	auto actionClose = new QAction();
	actionClose->setShortcut(QKeySequence::Quit);
	addAction(actionClose);
	QObject::connect(actionClose, &QAction::triggered, this,
					 &QCoreApplication::quit);
}


void MainWindow::onZoomReceived(QPointF oldPos, QPointF newPos) {
	auto* scrollArea = ui->scrollArea;

	int oldScrollX = scrollArea->horizontalScrollBar()->value();
	int oldScrollY = scrollArea->verticalScrollBar()->value();

	int newScrollX = oldScrollX + newPos.x() - oldPos.x();
	int newScrollY = oldScrollY + newPos.y() - oldPos.y();

	scrollArea->horizontalScrollBar()->setValue(newScrollX);
	scrollArea->verticalScrollBar()->setValue(newScrollY);
}

void MainWindow::newPolygon() {
	emit Mediator::instance() -> onEditorReset();
	ui->tree->setEnabled(false);
	ui->tree->clearSelection();
	ui->editorDock->show();
}

void MainWindow::addPolygon(Polygon *polygon) {
	QTreeWidgetItem *polyItem = new QTreeWidgetItem(ui->tree);
	polyItem->setText(0, polygon->name());
	polyItem->setData(0, Qt::UserRole, QVariant::fromValue(polygon->id()));

	QTreeWidgetItem *vertexFolder = new QTreeWidgetItem(polyItem);
	vertexFolder->setText(0, "Vertices");
	vertexFolder->setData(0, Qt::UserRole, QVariant::fromValue(polygon->id()));
	QTreeWidgetItem *edgeFolder = new QTreeWidgetItem(polyItem);
	edgeFolder->setText(0, "Edges");
	edgeFolder->setData(0, Qt::UserRole, QVariant::fromValue(polygon->id()));

	for (auto &vertex : polygon->vertices) {
		QTreeWidgetItem *v = new QTreeWidgetItem(vertexFolder);
		v->setText(0, all_vertices[vertex].name());
		v->setData(0, Qt::UserRole, QVariant::fromValue(vertex));
	}
	for (auto &edge : polygon->edges) {
		QTreeWidgetItem *e = new QTreeWidgetItem(edgeFolder);
		e->setText(0, all_edges[edge].name());
		e->setData(0, Qt::UserRole, QVariant::fromValue(edge));
	}
}

void MainWindow::removePolygon(QUuid id) {
	QVariant target = QVariant::fromValue(id);
	int count = ui->tree->topLevelItemCount();
	// Проходим по всем дочерним элементам первого уровня
	for (int i = 0; i < count; ++i) {
		QTreeWidgetItem *item =
			ui->tree->topLevelItem(i); // Убираем первый элемент
		if (item->data(0, Qt::UserRole) == target) {
			delete item; // Удаляем поддерево
			return;		 // После удаления выходим
		}
	}
}

void MainWindow::onPolygonSelectReceived() {
	ui->tree->setEnabled(false);
	ui->tree->clearSelection();
}

void MainWindow::onEditingExitReceived() {
	ui->tree->setEnabled(true);
}

void MainWindow::selectPolygon(QUuid id) {
	if (id.isNull()) {
		return;
	}
	Polygon *polygon = &all_polygons[id];
	ui->editorDock->show();
	emit Mediator::instance() -> onPolygonSelect(polygon);
}

void MainWindow::resetEverything() {
	ui->tree->clear();
	all_edges.clear();
	all_vertices.clear();
	all_polygons.clear();
	Polygon::reset_polygons_total();
	ui->paintArea->repaint();
}

MainWindow::~MainWindow() { delete ui; }
