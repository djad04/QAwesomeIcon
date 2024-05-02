#include "spritesheetparamsdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDialogButtonBox>

SpriteSheetParamsDialog::SpriteSheetParamsDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUi();
}

void SpriteSheetParamsDialog::setupUi()
{
    setWindowTitle("Sprite Sheet Parameters");

    auto* layout = new QVBoxLayout(this);

    // Frame size row
    {
        auto* row = new QHBoxLayout();
        row->addWidget(new QLabel("Frame Size (WxH):", this));

        m_widthSpin = new QSpinBox(this);
        m_widthSpin->setRange(1, 4096);
        m_widthSpin->setValue(32);
        row->addWidget(m_widthSpin);

        row->addWidget(new QLabel("x", this));

        m_heightSpin = new QSpinBox(this);
        m_heightSpin->setRange(1, 4096);
        m_heightSpin->setValue(32);
        row->addWidget(m_heightSpin);

        layout->addLayout(row);
    }

    // Frame count
    {
        auto* row = new QHBoxLayout();
        row->addWidget(new QLabel("Frame Count:", this));
        m_frameCountSpin = new QSpinBox(this);
        m_frameCountSpin->setRange(1, 100000);
        m_frameCountSpin->setValue(16);
        row->addWidget(m_frameCountSpin);
        layout->addLayout(row);
    }

    // Frames per row
    {
        auto* row = new QHBoxLayout();
        row->addWidget(new QLabel("Frames Per Row (0 = auto):", this));
        m_framesPerRowSpin = new QSpinBox(this);
        m_framesPerRowSpin->setRange(0, 100000);
        m_framesPerRowSpin->setValue(8);
        row->addWidget(m_framesPerRowSpin);
        layout->addLayout(row);
    }

    // Buttons
    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(m_buttonBox);
}

QSize SpriteSheetParamsDialog::getFrameSize() const
{
    return QSize(m_widthSpin->value(), m_heightSpin->value());
}

int SpriteSheetParamsDialog::getFrameCount() const
{
    return m_frameCountSpin->value();
}

int SpriteSheetParamsDialog::getFramesPerRow() const
{
    return m_framesPerRowSpin->value();
}