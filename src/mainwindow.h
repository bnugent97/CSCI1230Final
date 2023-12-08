#pragma once

#include <QMainWindow>
#include <QCheckBox>
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include "realtime.h"
#include "utils/aspectratiowidget/aspectratiowidget.hpp"

// Include QRadioButton and QButtonGroup
#include <QRadioButton>
#include <QButtonGroup>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    void initialize();
    void finish();

private:
    void connectUIElements();
    void connectParam1();
    void connectParam2();
    void connectNear();
    void connectFar();
    void connectPerPixelFilter();
    void connectKernelBasedFilter();

    void connectExtraCredit();

    Realtime *realtime;
    AspectRatioWidget *aspectRatioWidget;
    QCheckBox *filter1;
    QCheckBox *filter2;
    QSlider *p1Slider;
    QSlider *p2Slider;
    QSpinBox *p1Box;
    QSpinBox *p2Box;


    // Extra Credit:
    QRadioButton *ec1;
    QRadioButton *ec2;
    QRadioButton *ec3;
    QButtonGroup *ecGroup;

private slots:
    void onPerPixelFilter();
    void onKernelBasedFilter();
    void onValChangeP1(int newValue);
    void onValChangeP2(int newValue);

    // Extra Credit:
    void onExtraCredit1();
    void onExtraCredit2();
    void onExtraCredit3();

};
