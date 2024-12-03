#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "includes.h"
#include "customgraphicsview.h"
#include "graphicscontroller.h"

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

    enum Axis {
        XAxis,
        YAxis
    };

    enum Dimension {
        Width,
        Height
    };

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
    void handleButtonClick(QPushButton* clickedButton, CursorMode mode, QGraphicsView::DragMode dragMode = QGraphicsView::NoDrag);
    void on_brushBtn_clicked();
    void on_colorDlgBtn_clicked();
    void on_axis_editingFinished(Axis axis);
    void on_dimension_editingFinished(Dimension dimension);
    void onTabChanged(int index);
    void updateActions();
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
    QPointF initialPoint;
    bool isDrawing = false;
    CustomGraphicsView *editorGraphicsView;
    CustomGraphicsView *viewerGraphicsView;
    QPushButton *selectedButton = nullptr;
    void updateButtonStyles(QPushButton* clickedButton);
    void onBrushSizeSliderDialog();
    GraphicsController *graphicsController;
    QSvgRenderer *svgRenderer;

};

#endif // MAINWINDOW_H
