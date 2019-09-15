#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainVideoCapture;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Tabs_tabBarClicked(int index);

    void on_pushButton_config_cam1_released();

private:
    Ui::MainWindow *ui;
    MainVideoCapture *videoCapture;
};

#endif // MAINWINDOW_H
