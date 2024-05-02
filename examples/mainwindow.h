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

private slots:
    void onLoadGif();
    void onLoadSpriteSheet();
    void onLoadSvgSequence();
    void onPlay();
    void onPause();
    void onStop();
    void onFrameChanged(int index);
    void onAnimationFinished();
    void onAnimationError(const QString& message);
    void onFrameReady(const QImage& image); // New slot for preview


private:
    Ui::MainWindow *ui;
    QScopedPointer<QAwesomeIcon> m_awesomeIcon;
    QLabel* m_statusLabel;
    QLabel* m_previewLabel;

    void setupUI();
    void setupConnections();
    void updateStatus(const QString& text);
};

#endif // MAINWINDOW_H
