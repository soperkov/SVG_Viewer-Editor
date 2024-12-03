#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QMainWindow>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , graphicsController(new GraphicsController(this))
    , svgRenderer(nullptr)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(0, 0, 800, 800, this);

    graphicsController->setScene(scene);

    ui->viewerGraphicsView->setScene(scene);
    ui->editorGraphicsView->setScene(scene);

    ui->viewerGraphicsView->setRenderHint(QPainter::Antialiasing);
    ui->editorGraphicsView->setRenderHint(QPainter::Antialiasing);
    ui->editorGraphicsView->setDragMode(QGraphicsView::RubberBandDrag);

    ui->editorGraphicsView->setFocus();
    ui->editorGraphicsView->setFocusPolicy(Qt::StrongFocus);

    ui->viewerGraphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    ui->editorGraphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);

    ui->viewerGraphicsView->installEventFilter(this);

    ui->svgTabs->setCurrentIndex(0);

    pen.setColor(Qt::black);
    brush.setColor(Qt::transparent);
    brush.setStyle(Qt::SolidPattern);

    connect(ui->svgTabs, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);

    connect(ui->editorGraphicsView, &CustomGraphicsView::mousePressed, this, &MainWindow::onGraphicsViewMousePressed);
    connect(ui->editorGraphicsView, &CustomGraphicsView::mouseMoved, this, &MainWindow::onGraphicsViewMouseMoved);
    connect(ui->editorGraphicsView, &CustomGraphicsView::mouseReleased, this, &MainWindow::onGraphicsViewMouseReleased);
    connect(scene, &QGraphicsScene::selectionChanged, this, &MainWindow::onSelectionChanged);
    connect(ui->xValue, &QSpinBox::editingFinished, [this]() { on_axis_editingFinished(XAxis); });
    connect(ui->yValue, &QSpinBox::editingFinished, [this]() { on_axis_editingFinished(YAxis); });
    connect(ui->wValue, &QSpinBox::editingFinished, [this]() { on_dimension_editingFinished(Width); });
    connect(ui->hValue, &QSpinBox::editingFinished, [this]() { on_dimension_editingFinished(Height); });
    connect(ui->arrowBtn, &QPushButton::clicked, this, [this]() { handleButtonClick(ui->arrowBtn, DefaultPointer, QGraphicsView::NoDrag); });
    connect(ui->zoomInBtn, &QPushButton::clicked, this, [this]() { handleButtonClick(ui->zoomInBtn, ZoomInMode); });
    connect(ui->zoomOutBtn, &QPushButton::clicked, this, [this]() { handleButtonClick(ui->zoomOutBtn, ZoomOutMode); });
    connect(ui->rectBtn, &QPushButton::clicked, this, [this]() { handleButtonClick(ui->rectBtn, RectangleDrawMode); });
    connect(ui->circBtn, &QPushButton::clicked, this, [this]() { handleButtonClick(ui->circBtn, CircleDrawMode); });
    connect(ui->triBtn, &QPushButton::clicked, this, [this]() { handleButtonClick(ui->triBtn, TriangleDrawMode); });

    selectedButton = ui->arrowBtn;
    updateButtonStyles(selectedButton);


    updateActions();
}

void MainWindow::onTabChanged(int index)
{
    if (currentCursorMode != DefaultPointer)
        setCursorMode(DefaultPointer);
    updateActions();
}

void MainWindow::onSelectionChanged()
{
    if (currentCursorMode == DefaultPointer) {
        QList<QGraphicsItem *> selectedItems = scene->selectedItems();
        if (!selectedItems.isEmpty()) {
            QGraphicsItem *selectedItem = selectedItems.first();
        }
    }
}

void MainWindow::updateActions()
{
    bool hasContent = isDrawing || svgLoaded || !scene->items().isEmpty();

    ui->actionNew->setEnabled(true);  // Always enable New action
    ui->actionSave->setEnabled(hasContent && ui->svgTabs->currentIndex() != 0);
    ui->actionSave_As->setEnabled(hasContent && ui->svgTabs->currentIndex() != 0);
    ui->actionPrint->setEnabled(hasContent);
    ui->objectControls->setVisible(ui->svgTabs->currentIndex() != 0);
    ui->fileControls->setVisible(ui->svgTabs->currentIndex() != 0);


}

void MainWindow::onButtonClicked()
{
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());

    if (selectedButton == clickedButton) {
        return;
    }

    updateButtonStyles(clickedButton);
}

void MainWindow::updateButtonStyles(QPushButton* clickedButton)
{
    if (selectedButton != nullptr) {
        selectedButton->setStyleSheet("");
    }

    selectedButton = clickedButton;
    selectedButton->setStyleSheet("background-color: lightblue;");
}

void MainWindow::setCursorMode(CursorMode mode)
{
    currentCursorMode = mode;

    switch (mode) {
    case DefaultPointer:
        ui->editorGraphicsView->unsetCursor();
        ui->editorGraphicsView->setDragMode(QGraphicsView::NoDrag);
        break;

    case ZoomInMode: {
        QPixmap zoomInCursorPixmap(":/icons/Icons/zoom-in-cursor.png");
        QPixmap scaledCursorPixmap = zoomInCursorPixmap.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QCursor magnifier(scaledCursorPixmap);
        ui->editorGraphicsView->setCursor(magnifier);
        break;
    }
    case ZoomOutMode: {
        QPixmap zoomOutCursorPixmap(":/icons/Icons/zoom-out-cursor.png");
        QPixmap scaledCursorPixmap = zoomOutCursorPixmap.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QCursor zoomOut(scaledCursorPixmap);
        ui->editorGraphicsView->setCursor(zoomOut);
        break;
    }
    case RectangleDrawMode:
    case CircleDrawMode:
    case TriangleDrawMode:
    {
        QPixmap zoomOutCursorPixmap(":/icons/Icons/draw-cursor.png");
        QPixmap scaledCursorPixmap = zoomOutCursorPixmap.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QCursor zoomOut(scaledCursorPixmap);
        ui->editorGraphicsView->setCursor(zoomOut);
        break;
    }
    }
}


void MainWindow::onGraphicsViewMousePressed(QMouseEvent *event)
{
    QPointF clickedPoint = ui->editorGraphicsView->mapToScene(event->pos());

    switch (currentCursorMode) {
    case RectangleDrawMode:
        graphicsController->startDrawingRectangle(clickedPoint, pen, brush);
        break;

    case CircleDrawMode:
        graphicsController->startDrawingEllipse(clickedPoint, pen, brush);
        break;

    case TriangleDrawMode:
        graphicsController->startDrawingTriangle(clickedPoint, pen, brush);
        break;

    case ZoomInMode:
        graphicsController->zoomInAtPoint(clickedPoint, ui->editorGraphicsView);
        break;

    case ZoomOutMode:
        graphicsController->zoomOutAtPoint(clickedPoint, ui->editorGraphicsView);
        break;

    default:
        break;
    }
}


void MainWindow::onGraphicsViewMouseMoved(QMouseEvent *event)
{
    QPointF currentPoint = ui->editorGraphicsView->mapToScene(event->pos());

    switch (currentCursorMode) {
    case RectangleDrawMode:
        graphicsController->updateDrawingRectangle(currentPoint);
        break;

    case CircleDrawMode:
        graphicsController->updateDrawingEllipse(currentPoint);
        break;

    case TriangleDrawMode:
        graphicsController->updateDrawingTriangle(currentPoint);
        break;

    default:
        break;
    }
}


void MainWindow::onGraphicsViewMouseReleased(QMouseEvent *event)
{
    switch (currentCursorMode) {
    case RectangleDrawMode:
        graphicsController->finishDrawingRectangle();
        break;

    case CircleDrawMode:
        graphicsController->finishDrawingEllipse();
        break;

    case TriangleDrawMode:
        graphicsController->finishDrawingTriangle();
        break;

    default:
        break;
    }
}


void MainWindow::updateItemProperties(QGraphicsItem *item)
{
    if (!item) return;

    ui->xValue->setValue(item->pos().x());
    ui->yValue->setValue(item->pos().y());

    QGraphicsRectItem *rectItem = dynamic_cast<QGraphicsRectItem *>(item);
    if (rectItem) {
        QRectF rect = rectItem->rect();
        ui->wValue->setValue(rect.width());
        ui->hValue->setValue(rect.height());
        return;
    }

    QGraphicsEllipseItem *ellipseItem = dynamic_cast<QGraphicsEllipseItem *>(item);
    if (ellipseItem) {
        QRectF rect = ellipseItem->rect();
        ui->wValue->setValue(rect.width());
        ui->hValue->setValue(rect.height());
        return;
    }

    QGraphicsPolygonItem *polygonItem = dynamic_cast<QGraphicsPolygonItem *>(item);
    if (polygonItem) {
        QPolygonF polygon = polygonItem->polygon();
        if (polygon.size() >= 3) {
            QPointF p1 = polygon[0];
            QPointF p2 = polygon[1];
            QPointF p3 = polygon[2];
            ui->wValue->setValue(p2.x() - p1.x());
            ui->hValue->setValue(p3.y() - p1.y());
        }
        return;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    using namespace Qt;

    QList<QGraphicsItem *> selectedItems = scene->selectedItems();
    if (selectedItems.isEmpty()) {
        QMainWindow::keyPressEvent(event);
        return;
    }

    QGraphicsItem *selectedItem = selectedItems.first();

    switch(event->key()) {
    case Key_Delete:
    case Key_Backspace:
        for (QGraphicsItem *item : selectedItems) {
            scene->removeItem(item);
            delete item;
        }
        break;

    case Key_D:
        ui->xValue->setValue(ui->xValue->value() + 5);
        break;

    case Key_A:
        ui->xValue->setValue(ui->xValue->value() - 5);
        break;

    case Key_W:
        ui->yValue->setValue(ui->yValue->value() - 5);
        break;

    case Key_S:
        ui->yValue->setValue(ui->yValue->value() + 5);
        break;


    default:
        QMainWindow::keyPressEvent(event);
        break;
    }
    ui->editorGraphicsView->viewport()->update();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    ui->viewerGraphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::enableRubberBandSelection()
{
    ui->editorGraphicsView->setDragMode(QGraphicsView::RubberBandDrag);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::setBrushColor(const QColor &color)
{
    brush.setColor(color);
}


void MainWindow::on_actionNew_triggered()
{
    scene->clear();
    currentSvgPath.clear();
    svgLoaded = false;
    isDrawing = false;
    currentSvgData.clear();
    dataAdded.clear();
    updateActions();
}

void MainWindow::on_actionOpen_triggered()
{
    QString defaultDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QString fileName = QFileDialog::getOpenFileName(this, "Open SVG", defaultDir, "SVG Files (*.svg)");
    if (!fileName.isEmpty()) {
        svgLoaded = true;

        currentSvgPath = fileName;
        if (svgRenderer) {
            delete svgRenderer;
        }
        svgRenderer = new QSvgRenderer(fileName, this);

        QGraphicsSvgItem *svgItem = new QGraphicsSvgItem();
        svgItem->setSharedRenderer(svgRenderer);
        scene->clear();
        scene->addItem(svgItem);

        ui->viewerGraphicsView->fitInView(svgItem, Qt::KeepAspectRatio);

        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            currentSvgData = file.readAll();
            dataAdded.clear();
            file.close();
        }

        updateActions();
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (currentSvgPath.isEmpty() || !svgLoaded) {
        on_actionSave_As_triggered();
        return;
    }

    QFile file(currentSvgPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        QString svgContent;
        svgContent = currentSvgData;
        int closingSvgIndex = svgContent.indexOf("</svg>");
        svgContent.insert(closingSvgIndex, dataAdded);
        out << svgContent;
        file.close();
    }
}

void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save As SVG File", "", "SVG Files (*.svg);;All Files (*)");

    if (fileName.isEmpty()) {
        return;
    }

    if (!fileName.endsWith(".svg", Qt::CaseInsensitive)) {
        fileName += ".svg";
    }


    QString svgContent;
    if (!currentSvgData.contains("<svg")) {
        QString svgHeader = "<svg xmlns='http://www.w3.org/2000/svg' version='1.1' width='800' height='800' viewBox='0 0 800 800'>\n";
        svgContent = svgHeader + dataAdded + "\n</svg>";
    } else {
        svgContent = currentSvgData;
        int closingSvgIndex = svgContent.indexOf("</svg>");
        svgContent.insert(closingSvgIndex, dataAdded);
    }

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << svgContent;
        file.close();
    }

    currentSvgPath = fileName;
    svgLoaded = true;
}

void MainWindow::on_actionPrint_triggered()
{
    if (isDrawing || svgLoaded || !scene->items().isEmpty()) {
        QPrinter printer;
        QPrintPreviewDialog previewDialog(&printer, this);

        connect(&previewDialog, &QPrintPreviewDialog::paintRequested, this, [=](QPrinter *printer) {
            QPainter painter(printer);

            if (svgRenderer && svgLoaded) {
                svgRenderer->render(&painter);
            }

            else if (!scene->items().isEmpty()) {
                ui->editorGraphicsView->render(&painter);
            }
        });

        previewDialog.exec();
    }
}


void MainWindow::on_actionExit_App_triggered()
{
    QApplication::quit();
}

void MainWindow::handleButtonClick(QPushButton* clickedButton, CursorMode mode, QGraphicsView::DragMode dragMode)
{
    if (selectedButton == clickedButton) {
        return; // Prevent redundant action if the same button is clicked
    }

    // Update button styles
    updateButtonStyles(clickedButton);
    selectedButton = clickedButton;

    // Change cursor mode and optionally drag mode
    if (currentCursorMode != mode) {
        setCursorMode(mode);
    }
    if (mode == DefaultPointer) {
        ui->editorGraphicsView->setDragMode(dragMode);
    }
}


void MainWindow::on_brushBtn_clicked()
{
    bool ok;
    int brushSize = QInputDialog::getInt(this, "Select Brush Size",
                                         "Brush Size:", 5, 1, 100, 1, &ok);
    if (ok) {
        pen.setWidth(brushSize);
    }
}

void MainWindow::onBrushSizeSliderDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Brush Size");

    QVBoxLayout layout(&dialog);

    QLabel label("Brush Size:");
    layout.addWidget(&label);

    QSlider slider(Qt::Horizontal);
    slider.setMinimum(1);
    slider.setMaximum(100);
    slider.setValue(pen.width());
    layout.addWidget(&slider);

    QPushButton button("OK");
    layout.addWidget(&button);

    connect(&button, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&slider, &QSlider::valueChanged, this, [this](int value){
        pen.setWidth(value);
    });

    dialog.exec();
}

void MainWindow::on_colorDlgBtn_clicked()
{
    QColor selectedColor = QColorDialog::getColor(Qt::black, this, "Select Color");

    if (!selectedColor.isValid()) {
        return; // Do nothing if the user cancels
    }

    QMessageBox msgBox;
    msgBox.setWindowTitle("Apply Color To");
    msgBox.setText("Where do you want to apply the selected color?");
    QAbstractButton* penButton = msgBox.addButton("Pen (Stroke)", QMessageBox::YesRole);
    QAbstractButton* brushButton = msgBox.addButton("Brush (Fill)", QMessageBox::NoRole);
    msgBox.exec();

    if (msgBox.clickedButton() == penButton) {
        pen.setColor(selectedColor);
    } else if (msgBox.clickedButton() == brushButton) {
        brush.setColor(selectedColor);
    }

    ui->colorDlgBtn->setStyleSheet(QString("background-color: %1").arg(selectedColor.name()));
}



void MainWindow::on_axis_editingFinished(Axis axis)
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem* selectedItem = scene->selectedItems().first();
    if (!selectedItem)
        return;

    qreal newValue = (axis == XAxis) ? ui->xValue->text().toDouble() : ui->yValue->text().toDouble();
    if (axis == XAxis) {
        selectedItem->setPos(newValue, selectedItem->pos().y());
    } else if (axis == YAxis) {
        selectedItem->setPos(selectedItem->pos().x(), newValue);
    }
}


void MainWindow::on_dimension_editingFinished(Dimension dimension)
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem* selectedItem = scene->selectedItems().first();
    QGraphicsRectItem* rectItem = dynamic_cast<QGraphicsRectItem*>(selectedItem);
    QGraphicsEllipseItem* ellipseItem = dynamic_cast<QGraphicsEllipseItem*>(selectedItem);
    QGraphicsPolygonItem* polygonItem = dynamic_cast<QGraphicsPolygonItem*>(selectedItem);

    qreal newValue = (dimension == Width) ? ui->wValue->value() : ui->hValue->value();

    if (rectItem) {
        QRectF rect = rectItem->rect();
        if (dimension == Width) {
            rect.setWidth(newValue);
        } else {
            rect.setHeight(newValue);
        }
        rectItem->setRect(rect);
        return;
    }

    if (ellipseItem) {
        QRectF rect = ellipseItem->rect();
        if (dimension == Width) {
            rect.setWidth(newValue);
        } else {
            rect.setHeight(newValue);
        }
        ellipseItem->setRect(rect);
        return;
    }

    if (polygonItem) {
        QPolygonF polygon = polygonItem->polygon();
        if (polygon.size() >= 3) {
            QPointF p1 = polygon[0]; // Fixed point
            QPointF p2 = polygon[1];
            QPointF p3 = polygon[2];
            if (dimension == Width) {
                p2.setX(p1.x() + newValue); // Adjust width
                p3.setX((p1.x() + p2.x()) / 2); // Keep triangle symmetric
            } else if (dimension == Height) {
                p3.setY(p1.y() + newValue); // Adjust height
            }
            polygon[1] = p2;
            polygon[2] = p3;
            polygonItem->setPolygon(polygon);
        }
    }
}



