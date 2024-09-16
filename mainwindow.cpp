#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QMainWindow>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , svgRenderer(nullptr)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(0, 0, 800, 800, this);

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
    connect(ui->xValue, SIGNAL(valueChanged(int)), this, SLOT(on_xValue_editingFinished()));
    connect(ui->yValue, SIGNAL(valueChanged(int)), this, SLOT(on_yValue_editingFinished()));
    connect(ui->wValue, SIGNAL(valueChanged(int)), this, SLOT(on_wValue_editingFinished()));
    connect(ui->hValue, SIGNAL(valueChanged(int)), this, SLOT(on_hValue_editingFinished()));
    connect(ui->arrowBtn, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    connect(ui->zoomInBtn, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    connect(ui->zoomOutBtn, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    connect(ui->rectBtn, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    connect(ui->circBtn, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    connect(ui->triBtn, &QPushButton::clicked, this, &MainWindow::onButtonClicked);

    // You can optionally select a default button to be highlighted at start
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

    // Enable/disable file actions based on whether there's content to work with
    ui->actionNew->setEnabled(true);  // Always enable New action
    ui->actionSave->setEnabled(hasContent && ui->svgTabs->currentIndex() != 0);
    ui->actionSave_As->setEnabled(hasContent && ui->svgTabs->currentIndex() != 0);
    ui->actionPrint->setEnabled(hasContent);
    ui->objectControls->setVisible(ui->svgTabs->currentIndex() != 0);
    ui->fileControls->setVisible(ui->svgTabs->currentIndex() != 0);


}

void MainWindow::onButtonClicked()
{
    // Get the clicked button
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
    if (currentCursorMode == mode) {
        currentCursorMode = DefaultPointer;
        ui->editorGraphicsView->unsetCursor();
        ui->editorGraphicsView->setDragMode(QGraphicsView::NoDrag);
    } else {
        currentCursorMode = mode;
        switch (currentCursorMode) {
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

        case DefaultPointer:
        default:
            ui->editorGraphicsView->unsetCursor();
            ui->editorGraphicsView->setDragMode(QGraphicsView::NoDrag);
        }
    }
}

void MainWindow::onGraphicsViewMousePressed(QMouseEvent *event)
{
    QPointF clickedPoint = ui->editorGraphicsView->mapToScene(event->pos());

    if (currentCursorMode == DefaultPointer) {
        QRectF selectionArea(clickedPoint.x() - 5, clickedPoint.y() - 5, 10, 10);

        QList<QGraphicsItem *> itemsInArea = scene->items(selectionArea);
        if (!itemsInArea.isEmpty()) {
            QGraphicsItem *item = itemsInArea.first();

            scene->clearSelection();
            item->setSelected(true);
            updateItemProperties(item);
        } else {
            scene->clearSelection();
        }
        return;
    }

    initialPoint = clickedPoint;

    qDebug() << "Mouse Pressed at: " << event->pos() << "initial point at:" << initialPoint;


    if (currentCursorMode == DefaultPointer)
        return;

    initialPoint = ui->editorGraphicsView->mapToScene(event->pos());

    switch (currentCursorMode) {
    case ZoomInMode:
        zoomInAtPoint(event->pos());
        break;

    case ZoomOutMode:
        zoomOutAtPoint(event->pos());
        break;

    case RectangleDrawMode:
        currentRect = new QGraphicsRectItem(QRectF(initialPoint, initialPoint));
        currentRect->setPen(QPen(pen.color(), pen.width()));
        currentRect->setBrush(brush);

        currentRect->setFlag(QGraphicsItem::ItemIsSelectable, true);
        currentRect->setFlag(QGraphicsItem::ItemIsMovable, true);

        ui->editorGraphicsView->scene()->addItem(currentRect);
        isDrawing = true;

        // Ensure file-related actions like "Save" and "Print" are enabled after the user starts drawing
        updateActions();
        break;

    case CircleDrawMode:
        currentEllipse = new QGraphicsEllipseItem(QRectF(initialPoint, initialPoint));
        currentEllipse->setPen(QPen(pen.color(), pen.width()));
        currentEllipse->setBrush(brush);

        currentEllipse->setFlag(QGraphicsItem::ItemIsSelectable, true);
        currentEllipse->setFlag(QGraphicsItem::ItemIsMovable, true);

        ui->editorGraphicsView->scene()->addItem(currentEllipse);
        isDrawing = true;

        // Ensure file-related actions like "Save" and "Print" are enabled after the user starts drawing
        updateActions();
        break;

    case TriangleDrawMode:
        currentPolygon = new QGraphicsPolygonItem();
        currentPolygon->setPen(QPen(pen.color(), pen.width()));
        currentPolygon->setBrush(brush);

        currentPolygon->setFlag(QGraphicsItem::ItemIsSelectable, true);
        currentPolygon->setFlag(QGraphicsItem::ItemIsMovable, true);

        ui->editorGraphicsView->scene()->addItem(currentPolygon);
        isDrawing = true;

        // Ensure file-related actions like "Save" and "Print" are enabled after the user starts drawing
        updateActions();
        break;

    default:
        break;

    }
}

void MainWindow::onGraphicsViewMouseMoved(QMouseEvent *event)
{
    if (!isDrawing)
        return;

    QPointF currentPos = ui->editorGraphicsView->mapToScene(event->pos());

    switch (currentCursorMode) {
    case RectangleDrawMode:
        if (currentRect) {
            QRectF newRect(initialPoint, currentPos);
            currentRect->setRect(newRect.normalized());
        }
        break;

    case CircleDrawMode:
        if (currentEllipse) {
            QRectF newEllipse(initialPoint, currentPos);
            currentEllipse->setRect(newEllipse.normalized());
        }
        break;

    case TriangleDrawMode:
        if (currentPolygon) {
            QPointF point1 = initialPoint;
            QPointF point2(currentPos.x(), initialPoint.y());
            QPointF point3((initialPoint.x() + currentPos.x()) / 2, currentPos.y());

            QPolygonF triangle;
            triangle << point1 << point2 << point3;

            currentPolygon->setPolygon(triangle);
        }
        break;

    default:
        break;
    }
}

void MainWindow::onGraphicsViewMouseReleased(QMouseEvent *event)
{
    QPointF scenePoint = ui->editorGraphicsView->mapToScene(event->pos());

    if (!isDrawing)
        return;

    isDrawing = false;

    switch (currentCursorMode) {
    case RectangleDrawMode:
    {
        if (currentRect) {
            // Map the final scene point correctly
            QPointF finalPoint = ui->editorGraphicsView->mapToScene(event->pos());

            // Normalize the rectangle to avoid negative dimensions
            QRectF finalRect = QRectF(initialPoint, finalPoint).normalized();

            // Debug to check if coordinates are correct
            qDebug() << "Final Rect Dimensions:"
                     << "x:" << finalRect.x()
                     << "y:" << finalRect.y()
                     << "width:" << finalRect.width()
                     << "height:" << finalRect.height();

            // Ensure colors are applied correctly
            qDebug() << "Pen Color:" << pen.color().name() << "Brush Color:" << brush.color().name();

            // Construct the SVG rectangle string
            QString rectSvg = QString("<rect x='%1' y='%2' width='%3' height='%4' style='stroke:%5; fill:%6; stroke-width:2;'/>")
                                  .arg(finalRect.x())
                                  .arg(finalRect.y())
                                  .arg(finalRect.width())
                                  .arg(finalRect.height())
                                  .arg(pen.color().name())
                                  .arg(brush.color().alpha() == 0 ? "none" : brush.color().name());

            // Append the SVG string to the current SVG data
            dataAdded += rectSvg + "\n";

            // Clear the current rectangle
            currentRect = nullptr;
        }
        break;
    }

    case CircleDrawMode:
    {
        if (currentEllipse) {
            QRectF finalEllipse(initialPoint, scenePoint);
            currentEllipse->setRect(finalEllipse.normalized());
            currentEllipse->setBrush(brush);

            QString ellipseSvg = QString("<ellipse cx='%1' cy='%2' rx='%3' ry='%4' style='stroke:%5; fill:%6; stroke-width:2;'/>")
                                     .arg(finalEllipse.center().x())
                                     .arg(finalEllipse.center().y())
                                     .arg(finalEllipse.width() / 2.0)
                                     .arg(finalEllipse.height() / 2.0)
                                     .arg(pen.color().name())
                                     .arg(brush.color().alpha() == 0 ? "none" : brush.color().name());
            dataAdded += ellipseSvg + "\n";

            currentEllipse = nullptr;
        }
        break;
    }

    case TriangleDrawMode:
    {
        if (currentPolygon) {
            QPointF point1 = initialPoint;
            QPointF point2(scenePoint.x(), initialPoint.y());
            QPointF point3((initialPoint.x() + scenePoint.x()) / 2, scenePoint.y());

            // Create the final triangle polygon
            QPolygonF finalTriangle;
            finalTriangle << point1 << point2 << point3;

            currentPolygon->setPolygon(finalTriangle);
            currentPolygon->setBrush(brush);

            // Add triangle to SVG data
            QString triangleSvg = QString("<polygon points='%1,%2 %3,%4 %5,%6' style='stroke:%7; fill:%8; stroke-width:2;'/>")
                                      .arg(point1.x()).arg(point1.y())
                                      .arg(point2.x()).arg(point2.y())
                                      .arg(point3.x()).arg(point3.y())
                                      .arg(pen.color().name())
                                       .arg(brush.color().alpha() == 0 ? "none" : brush.color().name());
            dataAdded += triangleSvg + "\n";

            currentPolygon = nullptr;
        }
        break;
    }

    default:
        QMainWindow::mouseReleaseEvent(event);
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
    case Key_Right:
        ui->xValue->setValue(ui->xValue->value() + 5);
        break;

    case Key_A:
    case Key_Left:
        ui->xValue->setValue(ui->xValue->value() - 5);
        break;

    case Key_W:
    case Key_Up:
        ui->yValue->setValue(ui->yValue->value() - 5);
        break;

    case Key_S:
    case Key_Down:
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

void MainWindow::zoomInAtPoint(const QPoint &point)
{
    QPointF scenePoint = ui->editorGraphicsView->mapToScene(point);

    const double scaleFactor = 1.2;
    ui->editorGraphicsView->scale(scaleFactor, scaleFactor);
    ui->editorGraphicsView->centerOn(scenePoint);
}

void MainWindow::zoomOutAtPoint(const QPoint &point)
{
    QPointF scenePoint = ui->editorGraphicsView->mapToScene(point);

    const double scaleFactor = 1.2;
    ui->editorGraphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    ui->editorGraphicsView->centerOn(scenePoint);
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
        // Add a new SVG header if not present
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
        out << svgContent;  // Write the SVG content
        file.close();
    }

    qDebug() << "Saved SVG:\n" << svgContent;  // Log the output
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


void MainWindow::on_arrowBtn_clicked()
{
    if (currentCursorMode != DefaultPointer)
        setCursorMode(DefaultPointer);

    ui->editorGraphicsView->setDragMode(QGraphicsView::NoDrag);
}

void MainWindow::on_zoomInBtn_clicked()
{
    setCursorMode(ZoomInMode);
}

void MainWindow::on_zoomOutBtn_clicked()
{
    setCursorMode(ZoomOutMode);
}

void MainWindow::on_rectBtn_clicked()
{
    setCursorMode(RectangleDrawMode);
}

void MainWindow::on_circBtn_clicked()
{
    setCursorMode(CircleDrawMode);
}

void MainWindow::on_triBtn_clicked()
{
    setCursorMode(TriangleDrawMode);
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
        qDebug() << "Pen color set to:" << selectedColor.name();
    } else if (msgBox.clickedButton() == brushButton) {
        brush.setColor(selectedColor);
        qDebug() << "Brush color set to:" << selectedColor.name();
    }

    ui->colorDlgBtn->setStyleSheet(QString("background-color: %1").arg(selectedColor.name()));
}



void MainWindow::on_xValue_editingFinished()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    if (!selectedItem)
        return;

    // Get the new x position from the UI and update the item
    qreal newX = ui->xValue->text().toDouble();
    selectedItem->setPos(newX, selectedItem->pos().y());
}

void MainWindow::on_yValue_editingFinished()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    if (!selectedItem)
        return;

    // Get the new y position from the UI and update the item
    qreal newY = ui->yValue->text().toDouble();
    selectedItem->setPos(selectedItem->pos().x(), newY);
}

void MainWindow::on_wValue_editingFinished()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QGraphicsRectItem *rectItem = dynamic_cast<QGraphicsRectItem *>(selectedItem);
    QGraphicsEllipseItem *ellipseItem = dynamic_cast<QGraphicsEllipseItem *>(selectedItem);
    QGraphicsPolygonItem *polygonItem = dynamic_cast<QGraphicsPolygonItem *>(selectedItem);

    // Handle rectangle scaling
    if (rectItem) {
        QRectF rect = rectItem->rect();
        qreal newWidth = ui->wValue->value();
        rect.setWidth(newWidth);
        rectItem->setRect(rect);
        return;
    }

    // Handle ellipse scaling
    if (ellipseItem) {
        QRectF rect = ellipseItem->rect();
        qreal newWidth = ui->wValue->value();
        rect.setWidth(newWidth);
        ellipseItem->setRect(rect);
        return;
    }

    // Handle triangle (polygon) scaling
    if (polygonItem) {
        QPolygonF polygon = polygonItem->polygon();
        if (polygon.size() >= 3) {
            QPointF p1 = polygon[0]; // Fixed point
            QPointF p2(p1.x() + ui->wValue->value(), p1.y()); // Adjust width
            QPointF p3((p1.x() + p2.x()) / 2, polygon[2].y()); // Keep the triangle symmetric
            polygon[1] = p2;
            polygon[2] = p3;
            polygonItem->setPolygon(polygon);
        }
    }
}

void MainWindow::on_hValue_editingFinished()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QGraphicsRectItem *rectItem = dynamic_cast<QGraphicsRectItem *>(selectedItem);
    QGraphicsEllipseItem *ellipseItem = dynamic_cast<QGraphicsEllipseItem *>(selectedItem);
    QGraphicsPolygonItem *polygonItem = dynamic_cast<QGraphicsPolygonItem *>(selectedItem);

    // Handle rectangle scaling
    if (rectItem) {
        QRectF rect = rectItem->rect();
        qreal newHeight = ui->hValue->value();
        rect.setHeight(newHeight);
        rectItem->setRect(rect);
        return;
    }

    // Handle ellipse scaling
    if (ellipseItem) {
        QRectF rect = ellipseItem->rect();
        qreal newHeight = ui->hValue->value();
        rect.setHeight(newHeight);
        ellipseItem->setRect(rect);
        return;
    }

    // Handle triangle (polygon) scaling
    if (polygonItem) {
        QPolygonF polygon = polygonItem->polygon();
        if (polygon.size() >= 3) {
            QPointF p1 = polygon[0]; // Fixed point
            QPointF p2 = polygon[1]; // Keep width
            QPointF p3((p1.x() + p2.x()) / 2, p1.y() + ui->hValue->value()); // Adjust height
            polygon[2] = p3;
            polygonItem->setPolygon(polygon);
        }
    }
}


