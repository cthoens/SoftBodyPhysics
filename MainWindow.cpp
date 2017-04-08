#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Body &body = ui->renderWidget->scene.bodies[0];
    ui->actionTogglePlay->setChecked(ui->renderWidget->play);
    ui->stretchStiffnessSlider->setValue(int(body.stiffness * 100.0f));
    ui->bendStiffnessSlider->setValue(int(body.bendStiffness * 100.0f));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::togglePlay(bool play)
{
    ui->renderWidget->play = play;
}

void MainWindow::sliderChanged(int value)
{
    Body &body = ui->renderWidget->scene.bodies[0];
    if (sender() == ui->stretchStiffnessSlider)
        body.stiffness = float(value)/100.0f;
    if (sender() == ui->bendStiffnessSlider)
        body.bendStiffness = float(value)/100.0f;
}
