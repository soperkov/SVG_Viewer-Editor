#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QPointF>

class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit CustomGraphicsView(QWidget *parent = nullptr);

signals:
    void mousePressed(QMouseEvent *event);
    void mouseMoved(QMouseEvent *event);
    void mouseReleased(QMouseEvent *event);

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        emit mousePressed(event);
    }
    void mouseMoveEvent(QMouseEvent *event) override
    {
         emit mouseMoved(event);
    }
    void mouseReleaseEvent(QMouseEvent *event) override
    {
         emit mouseReleased(event);
    }

private:
    QGraphicsRectItem *currentRect = nullptr;
    QGraphicsEllipseItem *currentEllipse = nullptr;
    QPointF initialPoint;
    bool isDrawing = false;
};

#endif // CUSTOMGRAPHICSVIEW_H
