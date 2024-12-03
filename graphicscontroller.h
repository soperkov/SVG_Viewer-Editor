#ifndef GRAPHICSCONTROLLER_H
#define GRAPHICSCONTROLLER_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsView>
#include <QPen>
#include <QBrush>
#include <QObject>

class GraphicsController : public QObject
{
    Q_OBJECT

public:
    explicit GraphicsController(QObject *parent = nullptr);

    void setScene(QGraphicsScene *scene);

    // Rectangle drawing
    void startDrawingRectangle(const QPointF &initialPoint, const QPen &pen, const QBrush &brush);
    void updateDrawingRectangle(const QPointF &currentPoint);
    void finishDrawingRectangle();

    // Ellipse drawing
    void startDrawingEllipse(const QPointF &initialPoint, const QPen &pen, const QBrush &brush);
    void updateDrawingEllipse(const QPointF &currentPoint);
    void finishDrawingEllipse();

    // Triangle drawing
    void startDrawingTriangle(const QPointF &initialPoint, const QPen &pen, const QBrush &brush);
    void updateDrawingTriangle(const QPointF &currentPoint);
    void finishDrawingTriangle();

    // Zooming
    void zoomInAtPoint(const QPointF &point, QGraphicsView *view);
    void zoomOutAtPoint(const QPointF &point, QGraphicsView *view);

private:
    QGraphicsScene *scene = nullptr;

    // Initial point for drawing
    QPointF initialPoint;

    // Temporary items for drawing
    QGraphicsRectItem *currentRect = nullptr;
    QGraphicsEllipseItem *currentEllipse = nullptr;
    QGraphicsPolygonItem *currentTriangle = nullptr;
};

#endif // GRAPHICSCONTROLLER_H
