#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QLabel>
#include <QToolBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include "spritesheetparamsdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUI();
    m_awesomeIcon.reset(new QAwesomeIcon(this));
    m_awesomeIcon->attachToWidget(this);
    setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    auto tb = addToolBar("Controls");
    auto aLoadGif = tb->addAction("Load GIF");
    auto aLoadSprite = tb->addAction("Load Sprite");
    auto aLoadSvgSeq = tb->addAction("Load SVG Seq");
    tb->addSeparator();
    auto aPlay = tb->addAction("Play");
    auto aPause = tb->addAction("Pause");
    auto aStop = tb->addAction("Stop");

    connect(aLoadGif, &QAction::triggered, this, &MainWindow::onLoadGif);
    connect(aLoadSprite, &QAction::triggered, this, &MainWindow::onLoadSpriteSheet);
    connect(aLoadSvgSeq, &QAction::triggered, this, &MainWindow::onLoadSvgSequence);
    connect(aPlay, &QAction::triggered, this, &MainWindow::onPlay);
    connect(aPause, &QAction::triggered, this, &MainWindow::onPause);
    connect(aStop, &QAction::triggered, this, &MainWindow::onStop);

    m_statusLabel = new QLabel(this);
    statusBar()->addPermanentWidget(m_statusLabel);

    // Create a simple central widget with a preview label
    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central);
    m_previewLabel = new QLabel("Preview will appear here", central);
    m_previewLabel->setAlignment(Qt::AlignCenter);
    m_previewLabel->setMinimumSize(200, 200);
    m_previewLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_previewLabel->setStyleSheet("background: #222; color: #aaa; border: 1px solid #444;");
    layout->addWidget(m_previewLabel);
    central->setLayout(layout);
    setCentralWidget(central);

    updateStatus("Ready");
}

void MainWindow::setupConnections()
{
    connect(m_awesomeIcon.data(), &QAwesomeIcon::frameChanged, this, &MainWindow::onFrameChanged);
    connect(m_awesomeIcon.data(), &QAwesomeIcon::finished, this, &MainWindow::onAnimationFinished);
    connect(m_awesomeIcon.data(), &QAwesomeIcon::errorOccurred, this, &MainWindow::onAnimationError);
    m_awesomeIcon->setIconSizes({ QSize(128,128), QSize(256,256) });
}

void MainWindow::updateStatus(const QString& text)
{
    if (m_statusLabel) m_statusLabel->setText(text);
}

void MainWindow::onLoadGif()
{
    QString f = QFileDialog::getOpenFileName(this, "Open GIF", QString(), "GIF Files (*.gif)");
    if (f.isEmpty()) return;
    if (m_awesomeIcon->loadFromFile(f, QAwesomeAnimationType::GIF)) {
        updateStatus("GIF loaded: " + f);
    } else {
        updateStatus("Failed to load GIF");
    }
}

void MainWindow::onLoadSpriteSheet()
{
    QString f = QFileDialog::getOpenFileName(this, "Open Sprite Sheet", QString(), "Images (*.png *.jpg *.jpeg)");
    if (f.isEmpty()) return;
    
    // Show dialog to get sprite sheet parameters
    SpriteSheetParamsDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) return;
    
    QSize frameSize = dialog.getFrameSize();
    int frameCount = dialog.getFrameCount();
    int framesPerRow = dialog.getFramesPerRow();
    
    if (m_awesomeIcon->loadSpriteSheet(f, frameSize, frameCount, framesPerRow)) {
        updateStatus(QString("Sprite sheet loaded: %1 (%2x%3, %4 frames, %5 per row)")
                    .arg(f).arg(frameSize.width()).arg(frameSize.height()).arg(frameCount).arg(framesPerRow));
    } else {
        updateStatus("Failed to load sprite sheet");
    }
}

void MainWindow::onLoadSvgSequence()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Open SVG Sequence", QString(), "SVG Files (*.svg)");
    if (files.isEmpty()) return;
    if (m_awesomeIcon->loadSvgSequence(files)) {
        updateStatus("SVG sequence loaded");
    } else {
        updateStatus("Failed to load SVG sequence");
    }
}

void MainWindow::onPlay()
{
    m_awesomeIcon->play(QAwesomeLoopMode::InfiniteLoop);
    updateStatus("Playing");
}

void MainWindow::onPause()
{
    m_awesomeIcon->pause();
    updateStatus("Paused");
}

void MainWindow::onStop()
{
    m_awesomeIcon->stop();
    updateStatus("Stopped");
}

void MainWindow::onFrameChanged(int index)
{
    statusBar()->showMessage(QString("Frame: %1").arg(index), 2000);
    // Try to fetch the largest icon pixmap and show in preview
    if (m_previewLabel) {
        QIcon icon = windowIcon();
        QSize sz = m_previewLabel->size().boundedTo(QSize(512, 512));
        QPixmap pm = icon.pixmap(sz);
        if (!pm.isNull()) {
            m_previewLabel->setPixmap(pm);
        }
    }
}

void MainWindow::onAnimationFinished()
{
    updateStatus("Finished");
}

void MainWindow::onAnimationError(const QString& message)
{
    updateStatus("Error: " + message);
}

void MainWindow::onFrameReady(const QImage& image)
{
    if (!m_previewLabel) return;
    if (image.isNull()) return;
    QPixmap pm = QPixmap::fromImage(image.scaled(m_previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_previewLabel->setPixmap(pm);
}
