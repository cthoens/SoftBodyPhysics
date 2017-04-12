#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void TogglePlay(bool play);
    void Step();
    void SliderChanged(int value);
    void ShowStretchForces(bool value);
    void ShowBendForces(bool value);
    void ToggleForces();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
