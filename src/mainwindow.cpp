#include "../include/mainwindow.hpp"


MainWindow::MainWindow(QWidget *parent, const char *config_file) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    stop_ET = true;
    stop_labrecorder = true;

    QObject::connect(ui->action_quit, SIGNAL(triggered()), this, SLOT(close()));
    QObject::connect(ui->connect_ET, SIGNAL(clicked()), this, SLOT(connectET()));
    QObject::connect(ui->connect_labrecorder, SIGNAL(clicked()), this, SLOT(connectLabrecorder()));
    QObject::connect(ui->action_load_configuration, SIGNAL(triggered()), this, SLOT(loadConfigDialog()));
    QObject::connect(ui->action_save_configuration, SIGNAL(triggered()), this, SLOT(saveConfigDialog()));
}

MainWindow::~MainWindow() noexcept = default;

void MainWindow::loadConfigDialog() {
    QString sel = QFileDialog::getOpenFileName(this,"Load Configuration File","","Configuration Files (*.cfg)");

    if (!sel.isEmpty())
        loadConfig(sel.toStdString());
}

void MainWindow::saveConfigDialog() {
    QString sel = QFileDialog::getSaveFileName(this,"Save Configuration File","","Configuration Files (*.cfg)");
    
    if (!sel.isEmpty())
        saveConfig(sel.toStdString());
}

void MainWindow::loadChanlocsDialog() {

}

void MainWindow::connectLabrecorder() {
    if (!stop_labrecorder) {
        try {
            record->setLSLSharing(false);
            record->setupLSLSharing(false);
            stop_labrecorder = true;
        } catch(std::exception &e) {
            QMessageBox::critical(this,"Error",(std::string("Could not stop the background processing: ")+=e.what()).c_str(),QMessageBox::Ok);
        
            return;
        }

        ui->connect_labrecorder->setText("Link");
    } else if(!stop_ET) {
        stop_labrecorder = false;
        record->setupLSLSharing(true);
        record->setLSLSharing(true);

        ui->connect_labrecorder->setText("Unlink");
    } else {
      QMessageBox::information(this, "Warning", "You need to start the ET connection before.", QMessageBox::Ok);
    }
}

void MainWindow::connectET() { 
    if (!stop_ET && stop_labrecorder) {
        try {
            stop_ET = true;
            
            record->setRecording(false);

            ET_thread->interrupt();
            ET_thread->join();
            ET_thread.reset();

            delete record;
        } catch(std::exception &e) {
            QMessageBox::critical(this,"Error", (std::string("Could not stop the background processing: ") += e.what()).c_str(),QMessageBox::Ok);
            
            return;
        }

        ui->connect_ET->setText("Connect");
    } else if(stop_ET) {
        try {
            record = new Recording(ui->ET_serial->text().toStdString(), ui->reference_channels->text().toStdString());
            record->getData();
        } catch(std::exception &e) {
            QMessageBox::critical(this,"Error", (std::string("Unable to start connection with the ET") += e.what()).c_str(), QMessageBox::Ok);

            return;
        }
        
        stop_ET = false;

        ET_thread.reset(new boost::thread(&MainWindow::acquisitionET, this));
        ui->connect_ET->setText("Disconnect");
    } else {
        QMessageBox::information(this, "Warning", "You need to unlik before.", QMessageBox::Ok);
    }
}

void MainWindow::closeEvent(QCloseEvent *ev) {
    if (ET_thread)
        ev->ignore();	
}

void MainWindow::loadConfig(const std::string filename) {
    QMessageBox::information(this, "Warning", "Not yet implemented", QMessageBox::Ok);
    //TODO loadConfig
}

void MainWindow::saveConfig(const std::string filename) {
    QMessageBox::information(this, "Warning", "Not yet implemented", QMessageBox::Ok);
    //TODO saveConfig
}

void MainWindow::acquisitionET() {
    record->recordData();
}