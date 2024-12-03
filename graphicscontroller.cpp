#include "graphicscontroller.h"

GraphicsController::GraphicsController(QObject *parent)
    : QObject(parent), scene(nullptr), currentRect(nullptr), currentEllipse(nullptr), currentTriangle(nullptr) {}

void GraphicsController::setScene(QGraphicsScene *scene)
{
    this->scene = scene;
}

// Rectangle Methods
void GraphicsController::startDrawingRectangle(const QPointF &initialPoint, const QPen &pen, const QBrush &brush)
{
    this->initialPoint = initialPoint;
    currentRect = new QGraphicsRectItem(QRectF(initialPoint, initialPoint));
    currentRect->setPen(pen);
    currentRect->setBrush(brush);
    currentRect->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    scene->addItem(currentRect);
}

void GraphicsController::updateDrawingRectangle(const QPointF &currentPoint)
{
    if (currentRect) {
        QRectF newRect(initialPoint, currentPoint);
        currentRect->setRect(newRect.normalized());
    }
}

void GraphicsController::finishDrawingRectangle()
{
    currentRect = nullptr;
}

// Ellipse Methods
void GraphicsController::startDrawingEllipse(const QPointF &initialPoint, const QPen &pen, const QBrush &brush)
{
    this->initialPoint = initialPoint;
    currentEllipse = new QGraphicsEllipseItem(QRectF(initialPoint, initialPoint));
    currentEllipse->setPen(pen);
    currentEllipse->setBrush(brush);
    currentEllipse->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    scene->addItem(currentEllipse);
}

void GraphicsController::updateDrawingEllipse(const QPointF &currentPoint)
{
    if (currentEllipse) {
        QRectF newEllipse(initialPoint, currentPoint);
        currentEllipse->setRect(newEllipse.normalized());
    }
}

void GraphicsController::finishDrawingEllipse()
{
    currentEllipse = nullptr;
}

// Triangle Methods
void GraphicsController::startDrawingTriangle(const QPointF &initialPoint, const QPen &pen, const QBrush &brush)
{
    this->initialPoint = initialPoint;
    currentTriangle = new QGraphicsPolygonItem();
    currentTriangle->setPen(pen);
    currentTriangle->setBrush(brush);
    currentTriangle->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    scene->addItem(currentTriangle);
}

void GraphicsController::updateDrawingTriangle(const QPointF &currentPoint)
{
    if (currentTriangle) {
        QPointF point1 = initialPoint;
        QPointF point2(currentPoint.x(), initialPoint.y());
        QPointF point3((initialPoint.x() + currentPoint.x()) / 2, currentPoint.y());

        QPolygonF triangle;
        triangle << point1 << point2 << point3;
        currentTriangle->setPolygon(triangle);
    }
}

void GraphicsController::finishDrawingTriangle()
{
    currentTriangle = nullptr;
}

// Zoom Methods
void GraphicsController::zoomInAtPoint(const QPointF &point, QGraphicsView *view)
{
    const double scaleFactor = 1.2;
    view->scale(scaleFactor, scaleFactor);
    view->centerOn(point);
}

void GraphicsController::zoomOutAtPoint(const QPointF &point, QGraphicsView *view)
{
    const double scaleFactor = 1.2;
    view->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    view->centerOn(point);
}
