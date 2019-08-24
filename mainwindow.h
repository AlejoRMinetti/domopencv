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

private:
    Ui::MainWindow *ui;
    MainVideoCapture *mOpenCV_videoCapture;
};

#endif // MAINWINDOW_H
