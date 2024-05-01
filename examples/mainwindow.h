#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qawesomeicon.h"
#include <QScopedPointer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QScopedPointer<QAwesomeIcon> m_awesomeIcon;
    QLabel* m_statusLabel;
    QLabel* m_previewLabel;
};

#endif // MAINWINDOW_H
