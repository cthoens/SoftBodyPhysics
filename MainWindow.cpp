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
    ui->actionStretchForces->setChecked(ui->renderWidget->renderer.showStretchForces);
    ui->actionBendForces->setChecked(ui->renderWidget->renderer.showBendForces);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::TogglePlay(bool play)
{
    ui->renderWidget->play = play;
}

void MainWindow::Step()
{
    ui->renderWidget->step = true;
}

void MainWindow::ShowStretchForces(bool value)
{
    ui->renderWidget->renderer.showStretchForces = value;
}

void MainWindow::ShowBendForces(bool value)
{
    ui->renderWidget->renderer.showBendForces = value;
}

void MainWindow::ToggleForces()
{
    bool show = !ui->actionBendForces->isChecked() && !ui->actionStretchForces->isChecked();
    ui->actionBendForces->setChecked(show);
    ui->actionStretchForces->setChecked(show);
}

void MainWindow::SliderChanged(int value)
{
    Body &body = ui->renderWidget->scene.bodies[0];
    if (sender() == ui->stretchStiffnessSlider)
        body.stiffness = float(value)/100.0f;
    if (sender() == ui->bendStiffnessSlider)
        body.bendStiffness = float(value)/100.0f;
}
