
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtWidgets/QDialog>
#include <QtMultimedia/qmediaencodersettings.h>

class QComboBox;

class QMediaRecorder;



namespace Ui {
    class SettingsUi;
}

class Settings : public QDialog {
    Q_OBJECT
public:
    Settings(QMediaRecorder *mediaRecorder, QWidget *parent = 0);
    ~Settings();

    QAudioEncoderSettings audioSettings() const;
    void setAudioSettings(const QAudioEncoderSettings&);

    QVideoEncoderSettings videoSettings() const;
    void setVideoSettings(const QVideoEncoderSettings&);

    QString format() const;
    void setFormat(const QString &format);

protected:
    void changeEvent(QEvent *e);

private:
    QVariant boxValue(const QComboBox*) const;
    void selectComboBoxItem(QComboBox *box, const QVariant &value);

    //Ui::Settings *ui;
    Ui::SettingsUi* ui;
    QMediaRecorder *mediaRecorder;
};

#endif // SETTINGS_H
