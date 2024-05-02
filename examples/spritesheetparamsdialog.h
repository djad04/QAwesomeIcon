#ifndef SPRITESHEETPARAMSDIALOG_H
#define SPRITESHEETPARAMSDIALOG_H

#include <QDialog>
#include <QSize>

class QSpinBox;
class QDialogButtonBox;

class SpriteSheetParamsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SpriteSheetParamsDialog(QWidget* parent = nullptr);

    QSize getFrameSize() const;
    int getFrameCount() const;
    int getFramesPerRow() const;

private:
    QSpinBox* m_widthSpin{nullptr};
    QSpinBox* m_heightSpin{nullptr};
    QSpinBox* m_frameCountSpin{nullptr};
    QSpinBox* m_framesPerRowSpin{nullptr};
    QDialogButtonBox* m_buttonBox{nullptr};

    void setupUi();
};

#endif // SPRITESHEETPARAMSDIALOG_H