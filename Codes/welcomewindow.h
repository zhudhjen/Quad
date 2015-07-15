#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>

namespace Ui {
class WelcomeWindow;
}

class WelcomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WelcomeWindow(QWidget *parent = 0);
    ~WelcomeWindow();

    void showButton(bool b);
    void selectGameMode();
    void selectLevel();

private slots:
    void on_exitBtn_clicked();

    void on_singlePlayerBtn_clicked();

    void on_backMenuBtn_clicked();

private:
    Ui::WelcomeWindow *ui;
};

#endif // WELCOMEWINDOW_H
