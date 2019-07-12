#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP


#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QComboBox>
#include <QSettings>
#include <QCloseEvent>
#include <QListWidget>
#include <QMainWindow>
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>

#include <lsl_cpp.h>

#include "../include/recording.hpp"

#include "../window/ui_mainwindow.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent, const char *config_file);
        ~MainWindow() noexcept override;
    
    private slots:
        void connectLabrecorder();
        void connectET();

        void loadConfigDialog();
        void saveConfigDialog();

        void loadChanlocsDialog();
        
        void acquisitionET();

        void closeEvent(QCloseEvent *ev) override;

    private:
        void loadConfig(const std::string filename);
        void saveConfig(const std::string filename);

        bool stop_labrecorder;
        bool stop_ET;

        boost::shared_ptr<boost::thread> ET_thread;

        std::unique_ptr<QTimer> timer;
        std::unique_ptr<Ui::MainWindow> ui;

        Recording *record;
};

#endif