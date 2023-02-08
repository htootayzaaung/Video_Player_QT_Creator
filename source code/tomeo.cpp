//    ______
//   /_  __/___  ____ ___  ___  ____
//    / / / __ \/ __ `__ \/ _ \/ __ \
//   / / / /_/ / / / / / /  __/ /_/ /
//  /_/  \____/_/ /_/ /_/\___/\____/
//              video for sports enthusiasts...
//
//

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include "the_player.h"
#include "the_button.h"
#include <QSlider>
#include <QFormLayout>
#include <QComboBox>


// read in videos and thumbnails to this directory
std::vector<TheButtonInfo> getInfoIn (std::string loc) {

    std::vector<TheButtonInfo> out =  std::vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    while (it.hasNext()) { // for all files

        QString f = it.next();

            if (f.contains("."))

#if defined(_WIN32)
            if (f.contains(".wmv"))  { // windows
#else
            if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif

            QString thumb = f.left( f .length() - 4) +".png";
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
                    }
                    else
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
        }
    }

    return out;
}


int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    std::vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn( std::string(argv[1]) );

    if (videos.size() == 0) {

        const int result = QMessageBox::information(
                    NULL,
                    QString("Tomeo"),
                    QString("no videos found! Add command line argument to \"quoted\" file location."));
        exit(-1);
    }
    //palettes for window and button
    QPalette pal = QPalette();
    QPalette palw = QPalette();
    QPalette palt = QPalette();

    // set black background
    pal.setColor(QPalette::Button, Qt::darkGray);
    palw.setColor(QPalette::Window, Qt::black);


    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer(videoWidget);
    player->setVideoOutput(player->videoWidget);


    // a row of buttons
    QWidget *buttonWidget = new QWidget();
    // a list of the buttons
    std::vector<TheButton*> buttons;
    // the buttons are arranged horizontally
    QHBoxLayout *layout = new QHBoxLayout();
    buttonWidget->setLayout(layout);

    //combo box to hold NON FUNCTIONAL menu items
    QComboBox *settings = new QComboBox;
    settings->setStyleSheet("background:#aeadac");
    settings->setPalette(pal);
    settings->addItem(("Menu"));
    settings->addItem(("Save File"));
    settings->addItem(("Open New File"));
    settings->addItem(("🛠️ - Settings"));
    settings->addItem(("Report Any Issues"));
    settings->addItem(("Quit"));

//define the buttons and slider

    player->m_fullScreenButton -> setCheckable(true);
    QHBoxLayout *buttons_Hlayout = new QHBoxLayout();
    buttons_Hlayout->addWidget(player->seekBackward);
    player->seekBackward->setPalette(pal);
    buttons_Hlayout->addWidget(player->pausplay);
    player->pausplay->setPalette(pal);
    buttons_Hlayout->addWidget(player->seekForward);
    player->seekForward->setPalette(pal);
    buttons_Hlayout->addWidget(player->pbbtn);
    player->pbbtn->setPalette(pal);
    buttons_Hlayout->addWidget(player->volslider);
    buttons_Hlayout->addWidget(player->setVol);
    player->setVol->setStyleSheet("QLabel{color:white;}");
    buttons_Hlayout->addWidget(player->m_fullScreenButton);
    player->m_fullScreenButton->setPalette(pal);


    QHBoxLayout *track = new QHBoxLayout();
    track->addWidget(player->slider);
    track->addWidget(player->curduration);
    player->curduration->setStyleSheet("QLabel{color:white;}");
    track->addWidget(player->vidduration);
    player->vidduration->setStyleSheet("QLabel{color:white;}");

    // create the four buttons
    for ( int i = 0; i < 4; i++ ) {
        TheButton *button = new TheButton(buttonWidget);
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo*))); // when clicked, tell the player to play.
        buttons.push_back(button);
        layout->addWidget(button);
        button->init(&videos.at(i));
        button->setPalette(pal);
    }

    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);

    // create the main window and layout
    QWidget window;
    QVBoxLayout *top = new QVBoxLayout();
    window.setLayout(top);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(800, 680);
    //layout for the video editor sliders
    //stored in a seperate widget






    // add the video and the buttons to the top level widget
    top->addWidget(settings);
    top->addWidget(player->videoWidget);

    //added some of the public widgets from the header file here
    //as we have specified some connections
    top->addLayout(track);
    top->addLayout(buttons_Hlayout);
    top->addWidget(buttonWidget);
    top->addWidget(player->button1);



    //set palette for window
    window.setPalette(palw);

    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
