#include "mainwindow.h"
#include "examples/ui_mainwindow.h"
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
