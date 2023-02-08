//
// Created by twak on 11/11/2019.
//

#include "the_player.h"
#include <QTime>

// all buttons have been setup, store pointers here

void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;

    jumpTo(buttons -> at(0) -> info);
}

// change the image and video for one button every one second
void ThePlayer::shuffle() {
    TheButtonInfo* i = & infos -> at (rand() % infos->size() );
//        setMedia(*i->url);
    buttons -> at( updateCount++ % buttons->size() ) -> init( i );
}


void ThePlayer::playStateChanged (QMediaPlayer::State ms) {
    switch (ms) {
        case QMediaPlayer::State::StoppedState:
        //reset the playback rate and variables to the initial values.
            this->setPlaybackRate(this->playbackRate() - pbSpeed);
            pbSpeed = 0;
            setVol->setText("Volume: 0");
            this->pbbtn->setText("Playback Speed: 1x");
            this->setVolume(0);
            this->volslider->setValue(0);
            pausplay->setIcon(QIcon("/home/csunix/sc21ak/cw2/source code/icons/pause.png"));

            brightnessSlider->setValue(0);
            videoWidget->setBrightness(0);

            hueSlider->setValue(0);
            videoWidget->setHue(0);

            contrastSlider->setValue(0);
            videoWidget->setContrast(0);

            saturationSlider->setValue(0);
            videoWidget->setSaturation(0);

            //reset player state aswell to ensure consistent playback
            m_playerState = QMediaPlayer::PlayingState;
            play(); // starting playing again...
            break;
    default:
        break;
    }
}
//created switch cases to pause and play the video
void ThePlayer::pauseplay()
{
    switch (m_playerState) {
    case QMediaPlayer::StoppedState:
    //if video is in paused state
    case QMediaPlayer::PausedState:
        //switch the state to playing state
        m_playerState = QMediaPlayer::PlayingState;
        pausplay->setIcon(QIcon("/home/csunix/sc21ak/cw2/source code/icons/pause.png"));
        //play the video
        emit play();
        break;
    //if video is in playing state
    case QMediaPlayer::PlayingState:
        //switch the state variable to paused state
        m_playerState = QMediaPlayer::PausedState;
        pausplay->setIcon(QIcon("/home/csunix/sc21ak/cw2/source code/icons/play.png"));
        //pause the video
        emit pause();
        break;
    }
}

//function to increase player playback speed
void ThePlayer::playBackSpeedChanged()
{
    //uses a switch case using an inte defined in the header file
    switch (pbSpeed) {
    //when user clicks it for first time
    case 0 :
        //increase the playback rate and counter value
        pbbtn->setText("Playback Speed: 1.5x");
        this->setPlaybackRate(this->playbackRate()+1);
        pbSpeed+=1;
        break;
    //when user clicks button for the second time
    case 1 :
        pbbtn->setText("Playback Speed: 2x");
        // further increase playback speed and counter vale
        this->setPlaybackRate(this->playbackRate() + 1);
        pbSpeed+=1;
        break;
    //when user clicks button for the third time
    case 2 :
        pbbtn->setText("Playback Speed: 2.5x");
        // increase playback rate and counter value
        this ->setPlaybackRate(this->playbackRate()+1);
        pbSpeed+=1;
        break;
    //when user clicks button again
    case 3:
        pbbtn->setText("Playback Speed: 1x");
        //reset the playback rate to its initial value
        this->setPlaybackRate(this->playbackRate()-3);
        //reset the counter value
        pbSpeed=0;
        break;
    }
}

//ADDED FUNCTIONS TILL END
//supposed to change the slider value when the vid is playing
void ThePlayer::PositionChanged(qint64 position)
{
    //adjusts player value in real time
    //converts it to time format and sets it to a label
    qint64 duration = position;

    int seconds = (duration/1000) % 60;
    int minutes = (duration/60000) % 60;
    int hours = (duration/3600000) % 24;

    QTime time(hours, minutes,seconds);
    curduration->setText(time.toString()+ " /");
    if (!slider->isSliderDown())
        slider->setValue(position/1000);
}

void ThePlayer::updateDurationInfo()
{
    //when the next video is selected reset the duration and convert it to time format
    qint64 duration = this->duration();
    //stores it to video time label
    int seconds = (duration/1000) % 60;
    int minutes = (duration/60000) % 60;
    int hours = (duration/3600000) % 24;
    QTime time(hours, minutes,seconds);    vidduration->setText(time.toString());
}



//supposed to set range of slider for the
void ThePlayer::DurationChanged(qint64 duration)
{
    //gets the duration and sets maxvalue of slides
    m_duration = duration /*/ 1000*/;
//    slider->setRange(0,m_duration);
    this->slider->setMaximum((this->duration()/1000));

}

//function to reopen color dialog
void ThePlayer::AvailableDialog(bool availabled){
        if (!availabled) {
            cd->close();
        }
        else {
            if(button1->isChecked()){
                cd->show();
            }
        }
}

//controls the positison of the media player
void ThePlayer::seek(int position)
{
    //to set the postition on the mediaplayer
    this->setPosition(position*1000);
}

//function that sets the posision of the player by +5 seconds
void ThePlayer::seekForwardBtn()
{
    int pos = this->position();
    this->setPosition(pos+5000);
}

//function that sets the posision of the player by -5 seconds
//if the video is in its first 5 seconds
//reset it to play from beginning
void ThePlayer::seekBackwardBtn()
{
    int pos = this->position();
    if(pos>=5000){
        this->setPosition(pos-5000);
    }
    else{
        this->setPosition(0);
    }
}

//function to set the volume label
void ThePlayer::volchanged(int vol){
    setVol -> setText("Volume: " + QString::number(vol));

}

void ThePlayer::jumpTo (TheButtonInfo* button) {
    setMedia(* button -> url);

    play();
}

//slot to toggle full screen
void ThePlayer::videoAvailableChanged(bool available)
{
    if (!available) {
        //if toggled off , the connections are removed and widget is set to show normal
        disconnect(m_fullScreenButton, &QPushButton::clicked, this->videoWidget, &QVideoWidget::setFullScreen);
        disconnect(this->videoWidget, &QVideoWidget::fullScreenChanged, m_fullScreenButton, &QPushButton::setChecked);
        videoWidget->showNormal();

    }
    else
    {
        //reinstate the connections and show widget in fullscreen mode
        connect(m_fullScreenButton, &QPushButton::clicked, this->videoWidget, &QVideoWidget::setFullScreen);
        connect(this->videoWidget, &QVideoWidget::fullScreenChanged, m_fullScreenButton, &QPushButton::setChecked);

        if (m_fullScreenButton->isChecked()){

            videoWidget->showFullScreen();
            videoWidget->setWindowState(Qt::WindowFullScreen);
        }
    }
}
