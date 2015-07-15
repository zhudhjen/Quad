#include "welcomewindow.h"
#include "ui_welcomewindow.h"

WelcomeWindow::WelcomeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WelcomeWindow)
{
    ui->setupUi(this);

    //窗口居中显示
    QDesktopWidget* pdw = QApplication::desktop();//获得桌面窗体
    move((pdw->width() - this->width())/2, (pdw->height() - this->height())/2);//设置窗口显示在屏幕中央
    raise();

    this->setWindowTitle("Quad");//window title

    selectGameMode();
}

WelcomeWindow::~WelcomeWindow()
{
    delete ui;
}

void WelcomeWindow::on_exitBtn_clicked()
{
    exit(0);//exit the program
}

void WelcomeWindow::showButton(bool b)
{
    //display three buttons
    ui->singlePlayerBtn->setVisible(b);
    ui->multiPlayerBtn->setVisible(b);
    ui->exitBtn->setVisible(b);
    //hide four buttons
    ui->easyBtn->setVisible(!b);
    ui->normalBtn->setVisible(!b);
    ui->hardBtn->setVisible(!b);
    ui->backMenuBtn->setVisible(!b);
}

void WelcomeWindow::selectGameMode()
{
    showButton(true);
}

void WelcomeWindow::selectLevel()
{
    showButton(false);
}

void WelcomeWindow::on_singlePlayerBtn_clicked()
{
    selectLevel();//选择游戏难度
}

void WelcomeWindow::on_backMenuBtn_clicked()
{
    selectGameMode();//选择游戏模式
}
