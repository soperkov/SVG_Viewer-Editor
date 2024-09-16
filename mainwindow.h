#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "includes.h"
#include "customgraphicsview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    enum CursorMode {
        DefaultPointer = 1,
        ZoomInMode = 2,
        ZoomOutMode = 3,
        RectangleDrawMode = 4,
        CircleDrawMode = 5,
        TriangleDrawMode = 6
    };

    CursorMode currentCursorMode = DefaultPointer;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionPrint_triggered();
    void on_actionExit_App_triggered();
    void on_arrowBtn_clicked();
    void on_zoomInBtn_clicked();
    void on_zoomOutBtn_clicked();
    void on_rectBtn_clicked();
    void on_circBtn_clicked();
    void on_triBtn_clicked();
    void on_brushBtn_clicked();
    void on_colorDlgBtn_clicked();
    void on_xValue_editingFinished();
    void on_yValue_editingFinished();
    void on_hValue_editingFinished();
    void on_wValue_editingFinished();
    void onTabChanged(int index);
    void updateActions();
    void zoomInAtPoint(const QPoint &point);
    void zoomOutAtPoint(const QPoint &point);
    void setCursorMode(CursorMode mode);
    void onGraphicsViewMousePressed(QMouseEvent *event);
    void onGraphicsViewMouseMoved(QMouseEvent *event);
    void onGraphicsViewMouseReleased(QMouseEvent *event);
    void setBrushColor(const QColor &color);
    void onSelectionChanged();
    void enableRubberBandSelection();
    void updateItemProperties(QGraphicsItem *item);
    void onButtonClicked();


private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QPen pen;
    QBrush brush;
    bool svgLoaded;
    QString dataAdded;
    QString currentSvgData;
    QString currentSvgPath;
    QSvgRenderer *svgRenderer;
    QGraphicsRectItem *currentRect = nullptr;
    QGraphicsEllipseItem *currentEllipse = nullptr;
    QGraphicsPolygonItem *currentPolygon = nullptr;
    QPointF initialPoint;
    bool isDrawing = false;
    CustomGraphicsView *editorGraphicsView;
    CustomGraphicsView *viewerGraphicsView;
    QPushButton *selectedButton = nullptr;
    void updateButtonStyles(QPushButton* clickedButton);
    void onBrushSizeSliderDialog();

};

#endif // MAINWINDOW_H
