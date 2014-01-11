#ifndef RECORDER_H
#define RECORDER_H

#include <QtMultimedia/qcamera.h>
#include <QtMultimedia/qmediarecorder.h>
#include <QCameraViewfinder>

namespace Ui {
   class CameraCapture;
}

#include <QtWidgets/QMainWindow>
#include <QtCore/QDir>


class CameraCapture : public QMainWindow
{
   Q_OBJECT
public:
   CameraCapture(QWidget *parent = 0);
   ~CameraCapture();

private slots:
   void setCamera(const QByteArray &cameraDevice);

   void toggleCamera();

   void record();
   void pause();
   void stop();

   void takeImage();

   void settings();

   void displayErrorMessage();

   void updateCameraDevice(QAction*);
   void updateAudioDevice(QAction*);

   void updateCameraState(QCamera::State);
   void updateRecorderState(QMediaRecorder::State state);

   void updateRecordTime();

   //void processCapturedImage(const QString& fname, const QImage& img);

private:
   Ui::CameraCapture *ui;

   QDir outputDir;
   QMediaRecorder* mediaRecorder;
   QCamera *camera;
   QMediaService *service;
   QWidget *videoWidget;
   QCameraViewfinder* viewfinder;
};

#endif
