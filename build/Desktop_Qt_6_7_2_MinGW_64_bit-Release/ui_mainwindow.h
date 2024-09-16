/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <customgraphicsview.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionPrint;
    QAction *actionExit_App;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QGroupBox *objectControls;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_5;
    QPushButton *rectBtn;
    QSpacerItem *verticalSpacer;
    QPushButton *circBtn;
    QSpacerItem *verticalSpacer_2;
    QPushButton *triBtn;
    QSpacerItem *verticalSpacer_3;
    QPushButton *brushBtn;
    QSpacerItem *verticalSpacer_4;
    QPushButton *colorDlgBtn;
    QSpacerItem *verticalSpacer_6;
    QTabWidget *svgTabs;
    QWidget *Viewer;
    QHBoxLayout *horizontalLayout_3;
    CustomGraphicsView *viewerGraphicsView;
    QWidget *Editor;
    QHBoxLayout *horizontalLayout_2;
    CustomGraphicsView *editorGraphicsView;
    QGroupBox *fileControls;
    QHBoxLayout *horizontalLayout;
    QFormLayout *rotateForm;
    QPushButton *zoomInBtn;
    QPushButton *arrowBtn;
    QPushButton *zoomOutBtn;
    QFormLayout *positionForm;
    QLabel *xLabel;
    QLabel *yLabel;
    QSpinBox *xValue;
    QSpinBox *yValue;
    QFormLayout *sizeForm;
    QLabel *hLabel;
    QSpinBox *hValue;
    QLabel *wLabel;
    QSpinBox *wValue;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(858, 599);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/Icons/app-icon.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        MainWindow->setWindowIcon(icon);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName("actionNew");
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName("actionOpen");
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName("actionSave");
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName("actionSave_As");
        actionPrint = new QAction(MainWindow);
        actionPrint->setObjectName("actionPrint");
        actionExit_App = new QAction(MainWindow);
        actionExit_App->setObjectName("actionExit_App");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        objectControls = new QGroupBox(centralwidget);
        objectControls->setObjectName("objectControls");
        verticalLayout_2 = new QVBoxLayout(objectControls);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_5);

        rectBtn = new QPushButton(objectControls);
        rectBtn->setObjectName("rectBtn");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/Icons/rectangle-icon.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        rectBtn->setIcon(icon1);

        verticalLayout->addWidget(rectBtn);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        circBtn = new QPushButton(objectControls);
        circBtn->setObjectName("circBtn");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/Icons/circle-icon.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        circBtn->setIcon(icon2);

        verticalLayout->addWidget(circBtn);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        triBtn = new QPushButton(objectControls);
        triBtn->setObjectName("triBtn");
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/Icons/triangle-icon.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        triBtn->setIcon(icon3);

        verticalLayout->addWidget(triBtn);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        brushBtn = new QPushButton(objectControls);
        brushBtn->setObjectName("brushBtn");
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/Icons/brush-icon.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        brushBtn->setIcon(icon4);

        verticalLayout->addWidget(brushBtn);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);

        colorDlgBtn = new QPushButton(objectControls);
        colorDlgBtn->setObjectName("colorDlgBtn");
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/Icons/colorPalette-icon.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        colorDlgBtn->setIcon(icon5);

        verticalLayout->addWidget(colorDlgBtn);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_6);


        verticalLayout_2->addLayout(verticalLayout);


        gridLayout->addWidget(objectControls, 1, 0, 1, 1);

        svgTabs = new QTabWidget(centralwidget);
        svgTabs->setObjectName("svgTabs");
        svgTabs->setTabPosition(QTabWidget::TabPosition::South);
        Viewer = new QWidget();
        Viewer->setObjectName("Viewer");
        horizontalLayout_3 = new QHBoxLayout(Viewer);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        viewerGraphicsView = new CustomGraphicsView(Viewer);
        viewerGraphicsView->setObjectName("viewerGraphicsView");

        horizontalLayout_3->addWidget(viewerGraphicsView);

        svgTabs->addTab(Viewer, QString());
        Editor = new QWidget();
        Editor->setObjectName("Editor");
        horizontalLayout_2 = new QHBoxLayout(Editor);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        editorGraphicsView = new CustomGraphicsView(Editor);
        editorGraphicsView->setObjectName("editorGraphicsView");

        horizontalLayout_2->addWidget(editorGraphicsView);

        svgTabs->addTab(Editor, QString());

        gridLayout->addWidget(svgTabs, 1, 1, 1, 1);

        fileControls = new QGroupBox(centralwidget);
        fileControls->setObjectName("fileControls");
        horizontalLayout = new QHBoxLayout(fileControls);
        horizontalLayout->setObjectName("horizontalLayout");
        rotateForm = new QFormLayout();
        rotateForm->setObjectName("rotateForm");
        zoomInBtn = new QPushButton(fileControls);
        zoomInBtn->setObjectName("zoomInBtn");
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/Icons/zoom-in-icon.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        zoomInBtn->setIcon(icon6);

        rotateForm->setWidget(0, QFormLayout::FieldRole, zoomInBtn);

        arrowBtn = new QPushButton(fileControls);
        arrowBtn->setObjectName("arrowBtn");
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icons/Icons/pointer-icon.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        arrowBtn->setIcon(icon7);

        rotateForm->setWidget(1, QFormLayout::LabelRole, arrowBtn);

        zoomOutBtn = new QPushButton(fileControls);
        zoomOutBtn->setObjectName("zoomOutBtn");
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/icons/Icons/zoom-out-icon.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        zoomOutBtn->setIcon(icon8);

        rotateForm->setWidget(1, QFormLayout::FieldRole, zoomOutBtn);


        horizontalLayout->addLayout(rotateForm);

        positionForm = new QFormLayout();
        positionForm->setObjectName("positionForm");
        xLabel = new QLabel(fileControls);
        xLabel->setObjectName("xLabel");

        positionForm->setWidget(0, QFormLayout::LabelRole, xLabel);

        yLabel = new QLabel(fileControls);
        yLabel->setObjectName("yLabel");

        positionForm->setWidget(1, QFormLayout::LabelRole, yLabel);

        xValue = new QSpinBox(fileControls);
        xValue->setObjectName("xValue");
        xValue->setMaximum(9999);

        positionForm->setWidget(0, QFormLayout::FieldRole, xValue);

        yValue = new QSpinBox(fileControls);
        yValue->setObjectName("yValue");
        yValue->setMaximum(9999);

        positionForm->setWidget(1, QFormLayout::FieldRole, yValue);


        horizontalLayout->addLayout(positionForm);

        sizeForm = new QFormLayout();
        sizeForm->setObjectName("sizeForm");
        hLabel = new QLabel(fileControls);
        hLabel->setObjectName("hLabel");

        sizeForm->setWidget(0, QFormLayout::LabelRole, hLabel);

        hValue = new QSpinBox(fileControls);
        hValue->setObjectName("hValue");
        hValue->setMaximum(9999);

        sizeForm->setWidget(0, QFormLayout::FieldRole, hValue);

        wLabel = new QLabel(fileControls);
        wLabel->setObjectName("wLabel");

        sizeForm->setWidget(1, QFormLayout::LabelRole, wLabel);

        wValue = new QSpinBox(fileControls);
        wValue->setObjectName("wValue");
        wValue->setMaximum(9999);

        sizeForm->setWidget(1, QFormLayout::FieldRole, wValue);


        horizontalLayout->addLayout(sizeForm);


        gridLayout->addWidget(fileControls, 0, 0, 1, 2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 858, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuFile->addSeparator();
        menuFile->addAction(actionPrint);
        menuFile->addSeparator();
        menuFile->addAction(actionExit_App);

        retranslateUi(MainWindow);

        svgTabs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "INK MUSE", nullptr));
        actionNew->setText(QCoreApplication::translate("MainWindow", "New", nullptr));
        actionOpen->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
        actionSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        actionSave_As->setText(QCoreApplication::translate("MainWindow", "Save As", nullptr));
        actionPrint->setText(QCoreApplication::translate("MainWindow", "Print", nullptr));
        actionExit_App->setText(QCoreApplication::translate("MainWindow", "Exit App", nullptr));
        objectControls->setTitle(QString());
        rectBtn->setText(QString());
        circBtn->setText(QString());
        triBtn->setText(QString());
        brushBtn->setText(QString());
        colorDlgBtn->setText(QString());
        svgTabs->setTabText(svgTabs->indexOf(Viewer), QCoreApplication::translate("MainWindow", "Viewer", nullptr));
        svgTabs->setTabText(svgTabs->indexOf(Editor), QCoreApplication::translate("MainWindow", "Editor", nullptr));
        fileControls->setTitle(QString());
        zoomInBtn->setText(QString());
        arrowBtn->setText(QString());
        zoomOutBtn->setText(QString());
        xLabel->setText(QCoreApplication::translate("MainWindow", "X:", nullptr));
        yLabel->setText(QCoreApplication::translate("MainWindow", "Y:", nullptr));
        hLabel->setText(QCoreApplication::translate("MainWindow", "Height:", nullptr));
        wLabel->setText(QCoreApplication::translate("MainWindow", "Width:", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
