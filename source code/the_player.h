//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H

#include <QApplication>
#include <QMediaPlayer>
#include "the_button.h"
#include <vector>
#include <QTimer>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QDialog>
#include <QVideoWidget>
#include <QString>
#include <QIcon>

class ThePlayer : public QMediaPlayer {

Q_OBJECT
public:
    //ADDED many public widgets to be used in tomeo
    QLabel* vidduration =  new QLabel();
    QWidget * tw = new QWidget;
    QLabel* curduration =  new QLabel();
    QLabel* setVol =  new QLabel("Volume: 0");
    QPushButton* pausplay = new QPushButton;
    QMediaPlayer* player = new QMediaPlayer(this);
    QMouseEvent *event;
    QSlider* slide;
    QSlider* slider = new QSlider(Qt::Horizontal);
    QVideoWidget* videoWidget = new QVideoWidget(tw);
    QSlider* volslider = new QSlider(Qt::Horizontal);
    QPushButton* pbbtn = new QPushButton("Playback Speed: 1x");
    QSlider *brightnessSlider = new QSlider(Qt::Horizontal);
    QSlider *contrastSlider = new QSlider(Qt::Horizontal);
    QSlider *hueSlider = new QSlider(Qt::Horizontal);
    QSlider *saturationSlider = new QSlider(Qt::Horizontal);
    QFormLayout *layoutt = new QFormLayout();
    QPushButton *button1 = new QPushButton("Color Options..");
    QDialog *cd = new QDialog();
    QPushButton *m_fullScreenButton = new QPushButton();
    QPushButton *seekForward = new QPushButton();
    QPushButton *seekBackward = new QPushButton();

private:
    std::vector<TheButtonInfo>* infos;
    std::vector<TheButton*>* buttons;
    QTimer* mTimer;
    //added a slider

   qint64 m_duration;

   //addded variable that stores the current state of the video
   //by default the video is playing so we set it to playing state
   QMediaPlayer::State m_playerState = QMediaPlayer::PlayingState;
    long updateCount = 0;
    int pbSpeed = 0;
    int seekFor = 0;
    int seekBack = 0;

public:
    ThePlayer(QWidget* parent) : QMediaPlayer(parent) {
        QVideoWidget* videoWidget = new QVideoWidget(parent);
         // be slightly less annoying
        //ADDED additional connections here
        connect(this, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (playStateChanged(QMediaPlayer::State)) );
        connect(this,&QMediaPlayer::videoAvailableChanged,this,&ThePlayer::videoAvailableChanged);
        //add connections here
        //these execute function when duration and currentposition change
        //for media playback
        connect(this, SIGNAL (durationChanged(qint64)), this, SLOT (updateDurationInfo()));
        connect(this, SIGNAL (durationChanged(qint64)), this, SLOT (DurationChanged(qint64)));
        connect(this,SIGNAL(positionChanged(qint64)),this,SLOT(PositionChanged(qint64)));
        connect(slider,SIGNAL(sliderMoved(int)),this,SLOT(seek(int)));
        connect(this->seekForward,SIGNAL(clicked()),this,SLOT(seekForwardBtn()));
        seekForward->setIcon(QIcon("/home/csunix/sc21ak/cw2/source code/icons/fast-forward.png"));
        connect(this->seekBackward,SIGNAL(clicked()),this,SLOT(seekBackwardBtn()));
        seekBackward->setIcon(QIcon("/home/csunix/sc21ak/cw2/source code/icons/barrows.png"));
        mTimer = new QTimer(NULL);
        mTimer->setInterval(1000); // 1000ms is one second between ...
        mTimer->start();
        pausplay->setIcon(QIcon("/home/csunix/sc21ak/cw2/source code/icons/pause.png"));
        volslider-> setFixedWidth(40);
        connect(mTimer, SIGNAL (timeout()), SLOT (shuffle()));
        //connections for buttons
        connect(this->pausplay, SIGNAL(clicked()), this, SLOT(pauseplay()));// ...running shuffle method
        connect(this->pbbtn, SIGNAL(clicked()), this, SLOT(playBackSpeedChanged()));
        connect( this->volslider ,SIGNAL(sliderMoved(int)),this, SLOT (setVolume(int)));
        connect( this->volslider ,SIGNAL(sliderMoved(int)),this, SLOT (volchanged(int)));
        m_fullScreenButton->setIcon(QIcon("/home/csunix/sc21ak/cw2/source code/icons/fullscreen.png"));
        //palette for the color options dialog
        QPalette palC = QPalette();

        // set black background
        palC.setColor(QPalette::Button, Qt::darkGray);

        button1->setPalette(palC);
        //add color options sliders to the new window
        layoutt->addRow(("Brightness"), brightnessSlider);
        layoutt->addRow(("Contrast"), contrastSlider);
        layoutt->addRow(("Hue"), hueSlider);
        layoutt->addRow(("Saturation"), saturationSlider);

        cd->setWindowTitle("Color Options");
        cd->setStyleSheet("background:#aeadac");
        cd->setLayout(layoutt);
        cd->setPalette(palC);
        connect(button1 ,SIGNAL(clicked()),cd,SLOT(open()));
        //set silder ranges for all sliders except media slider
        volslider->setRange(0, 100);
        brightnessSlider->setRange(-100, 100);
        hueSlider->setRange(-100, 100);
        contrastSlider->setRange(-100, 100);
        saturationSlider->setRange(-100, 100);
        //connect color options sliders with video widget built - in slots
        connect(brightnessSlider, SIGNAL(sliderMoved(int)), this->videoWidget,SLOT(setBrightness(int)));
        connect(contrastSlider, SIGNAL(sliderMoved(int)), this->videoWidget,SLOT(setContrast(int)));
        connect(hueSlider, SIGNAL(sliderMoved(int)), this->videoWidget,SLOT(setHue(int)));
        connect(saturationSlider, SIGNAL(sliderMoved(int)), this->videoWidget,SLOT(setSaturation(int)));
    }
    // all buttons have been setup, store pointers here
    void setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i);
    //added public function here

private slots:

    // change the image and video for one button every one second
    void shuffle();
    //these are additional functionality
    void pauseplay();
    void seek(int position);
    void volchanged(int vol);
    void PositionChanged(qint64 position);
    void playStateChanged (QMediaPlayer::State ms);
    void playBackSpeedChanged();
    void updateDurationInfo();
    void DurationChanged(qint64 position);
    void AvailableDialog(bool availabled);
    void videoAvailableChanged(bool available);
    void seekForwardBtn();
    void seekBackwardBtn();

public slots:

    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);
};
#endif //CW2_THE_PLAYER_H

