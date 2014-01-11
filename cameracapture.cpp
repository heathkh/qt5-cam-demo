#include "cameracapture.h"
#include "ui_cameracapture.h"
#include "camera-capture-settings.h"

#include <QtMultimedia/qmediaservice.h>
#include <QtMultimedia/qmediarecorder.h>
#include <QtMultimedia/qcamera.h>
#include <QtMultimediaWidgets/qvideowidget.h>


#include <QtMultimedia/QCameraImageCapture>

#include <QtWidgets>

CameraCapture::CameraCapture(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CameraCapture),
    mediaRecorder(0),
    camera(0),
    service(0),
    videoWidget(0)
{
    ui->setupUi(this);

    outputDir = QDir::currentPath();

    //camera devices
    QByteArray cameraDevice;

    ui->actionCamera->setMenu(new QMenu(this));
    QActionGroup *videoDevicesGroup = new QActionGroup(this);
    videoDevicesGroup->setExclusive(true);
    foreach(const QByteArray &deviceName, QCamera::availableDevices()) {
        QString description = deviceName+" "+camera->deviceDescription(deviceName);
        QAction *videoDeviceAction = new QAction(description, videoDevicesGroup);
        videoDeviceAction->setCheckable(true);
        videoDeviceAction->setData(QVariant(deviceName));
        if (cameraDevice.isEmpty()) {
            cameraDevice = deviceName;
            videoDeviceAction->setChecked(true);
        }
        ui->actionCamera->menu()->addAction(videoDeviceAction);
    }

    connect(videoDevicesGroup, SIGNAL(triggered(QAction*)), this, SLOT(updateCameraDevice(QAction*)));

    ui->actionAudio->setMenu(new QMenu(this));

    setCamera(cameraDevice);
}

CameraCapture::~CameraCapture()
{
    delete mediaRecorder;
    delete videoWidget;
    delete camera;
}

void CameraCapture::setCamera(const QByteArray &cameraDevice)
{
    delete mediaRecorder;
    delete videoWidget;
    delete camera;

    if (cameraDevice.isEmpty())
        camera = new QCamera;
    else
        camera = new QCamera(cameraDevice);

    connect(camera, SIGNAL(stateChanged(QCamera::State)), this, SLOT(updateCameraState(QCamera::State)));

    mediaRecorder = new QMediaRecorder(camera);
    connect(mediaRecorder, SIGNAL(stateChanged(QMediaRecorder::State)), this, SLOT(updateRecorderState(QMediaRecorder::State)));

    service = camera->service();

    //audio devices
    ui->actionAudio->menu()->clear();
    QActionGroup *audioDevicesGroup = new QActionGroup(this);
    audioDevicesGroup->setExclusive(true);

    if (service) {
      /*
        foreach(const QString deviceName, service-> ->supportedEndpoints(QMediaService::AudioDevice)) {
            QString description = service->endpointDescription(QMediaService::AudioDevice, deviceName);
            QAction *audioDeviceAction = new QAction(deviceName+" "+description, audioDevicesGroup);
            audioDeviceAction->setData(QVariant(deviceName));
            audioDeviceAction->setCheckable(true);

            ui->actionAudio->menu()->addAction(audioDeviceAction);

            if (service->activeEndpoint(QMediaService::AudioDevice) == deviceName)
                audioDeviceAction->setChecked(true);
        }
        */
    } else {
        qWarning() << "Camera service is not available";
        exit(0);

    }


    connect(audioDevicesGroup, SIGNAL(triggered(QAction*)), this, SLOT(updateAudioDevice(QAction*)));

    mediaRecorder->setOutputLocation(QUrl("test.mkv"));

    connect(mediaRecorder, SIGNAL(durationChanged(qint64)), this, SLOT(updateRecordTime()));
    connect(mediaRecorder, SIGNAL(error(QMediaRecorder::Error)), this, SLOT(displayErrorMessage()));

    //camera->setMetaData(QtMedia::Title, QVariant(QLatin1String("Test Title")));

    viewfinder = new QCameraViewfinder();
    viewfinder->show();

    camera->setViewfinder(viewfinder);


    ui->stackedWidget->addWidget(viewfinder);

    updateCameraState(camera->state());
    updateRecorderState(mediaRecorder->state());

    connect(camera, SIGNAL(availabilityChanged (bool)), ui->imageCaptureBox, SLOT(setEnabled(bool)));
    //connect(camera, SIGNAL(imageCaptured(QString,QImage)), this, SLOT(processCapturedImage(QString,QImage)));

}

void CameraCapture::updateRecordTime()
{
    QString str = QString("Recorded %1 sec").arg(mediaRecorder->duration()/1000);
    ui->statusbar->showMessage(str);
}



void CameraCapture::settings()
{
    Settings settingsDialog(mediaRecorder);

    settingsDialog.setAudioSettings(mediaRecorder->audioSettings());
    settingsDialog.setVideoSettings(mediaRecorder->videoSettings());
    //settingsDialog.setFormat(mediaRecorder->format());

    if (settingsDialog.exec()) {
        mediaRecorder->setEncodingSettings(
                settingsDialog.audioSettings(),
                settingsDialog.videoSettings(),
                settingsDialog.format());
    }
}

void CameraCapture::record()
{
    mediaRecorder->record();
    updateRecordTime();
}

void CameraCapture::pause()
{
    mediaRecorder->pause();
}

void CameraCapture::stop()
{
    mediaRecorder->stop();
}

void CameraCapture::takeImage()
{
    int lastImage = 0;
    foreach( QString fileName, outputDir.entryList(QStringList() << "img_*.jpg") ) {
        int imgNumber = fileName.mid(4, fileName.size()-8).toInt();
        lastImage = qMax(lastImage, imgNumber);
    }

    QCameraImageCapture* imageCapture = new QCameraImageCapture(camera);

    camera->setCaptureMode(QCamera::CaptureStillImage);
    camera->start();
    //on half pressed shutter button
    camera->searchAndLock();

    //on shutter button pressed
    imageCapture->capture(QString("img_%1.jpg").arg(lastImage+1,
                                                  4, //fieldWidth
                                                  10,
                                                  QLatin1Char('0')));

    //on shutter button released
    camera->unlock();


}

void CameraCapture::toggleCamera()
{
    if (camera->state() == QCamera::ActiveState)
        camera->stop();
    else
        camera->start();
}

void CameraCapture::updateCameraState(QCamera::State state)
{
    if (state == QCamera::ActiveState) {
        ui->actionCamera->setEnabled(false);
        ui->actionAudio->setEnabled(false);
        ui->actionSettings->setEnabled(false);

        ui->startCameraButton->setText(tr("Stop Camera"));
        ui->startCameraButton->setChecked(true);
        ui->videoCaptureBox->setEnabled(true);
    } else {
        ui->actionCamera->setEnabled(true);
        ui->actionAudio->setEnabled(true);
        ui->actionSettings->setEnabled(true);

        ui->startCameraButton->setText(tr("Start Camera"));
        ui->startCameraButton->setChecked(false);
        ui->imageCaptureBox->setEnabled(false);
        ui->videoCaptureBox->setEnabled(false);
    }

    if (camera->service()) {
        ui->startCameraButton->setEnabled(true);
    } else {
        ui->startCameraButton->setEnabled(false);
        ui->startCameraButton->setText(tr("Camera is not available"));
    }
}

void CameraCapture::updateRecorderState(QMediaRecorder::State state)
{
    switch (state) {
    case QMediaRecorder::StoppedState:
        ui->recordButton->setEnabled(true);
        ui->pauseButton->setEnabled(true);
        ui->stopButton->setEnabled(false);
        break;
    case QMediaRecorder::PausedState:
        ui->recordButton->setEnabled(true);
        ui->pauseButton->setEnabled(false);
        ui->stopButton->setEnabled(true);
        break;
    case QMediaRecorder::RecordingState:
        ui->recordButton->setEnabled(false);
        ui->pauseButton->setEnabled(true);
        ui->stopButton->setEnabled(true);
        break;
    }
}

void CameraCapture::displayErrorMessage()
{
    QMessageBox::warning(this, "Capture error", mediaRecorder->errorString());
}

void CameraCapture::updateCameraDevice(QAction *action)
{
    setCamera(action->data().toByteArray());
}

void CameraCapture::updateAudioDevice(QAction *action)
{
    //service->setActiveEndpoint(QMediaService::AudioDevice, action->data().toString());
}
