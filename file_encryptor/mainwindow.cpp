#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <fstream>
#include <streambuf>
#include <iostream>
#include "aes_encryptor.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_browseButton_clicked()
{
    // Get parent directory
    QDir curr_dir = QDir::current();
    curr_dir.cdUp();

    // Select a file from a QFileDialog
    QString file = QFileDialog::getOpenFileName(this, "Select file", curr_dir.path(),
                                                     "Text files (*.txt);;All files (*.*)",
                                                      new QString("All files (*.*)"));

    // Get file information
    file_info.file_name = file.toStdString();
    QFileInfo info(file);
    file_info.path = info.absoluteFilePath().toStdString();
    file_info.size = info.size();
    file_info.ext = info.suffix().toStdString();

    // Save file content
    std::ifstream f(file_info.file_name);
    std::string text((std::istreambuf_iterator<char>(f)),
                     std::istreambuf_iterator<char>());
    file_info.file_text = text;

    // Show file info
    QString file_descrp = "File path: " + QString::fromStdString(file_info.path) + "\nFile size: " + QString::number(file_info.size) +
                          " bytes\nFile extension: ." + QString::fromStdString(file_info.ext);

    ui->fileDscrpLabel->setEnabled(true);
    ui->fileDscrpLabel->setStyleSheet("font: non-italic");
    ui->fileDscrpLabel->setText(file_descrp);

    // Enable encrypt/decrypt/save buttons
    ui->encryptButton->setEnabled(true);
    ui->decryptButton->setEnabled(true);
    ui->saveButton->setEnabled(true);
}

void MainWindow::on_saveButton_clicked()
{
    // Get parent directory
    QDir curr_dir = QDir::current();
    curr_dir.cdUp();

    // Save a file from a QFileDialog
    QString file_name = QFileDialog::getSaveFileName(this,"Save file", curr_dir.path());

    // Write encrypted/decrypted data
    std::ofstream out(file_name.toStdString());
    out << data;
    out.close();

    ui->updateLabel->setEnabled(true);
    ui->updateLabel->setStyleSheet("color: rgb(204,0,0)");
    ui->updateLabel->setText("File saved successfully");
}

void MainWindow::on_encryptButton_clicked()
{
    std::string msg = file_info.file_text;
    const size_t msg_size = msg.length();

    const std::string key = "12345678912345678912345678912345";   // AES Key
    const size_t key_size = 256;

    AES_Encryptor encryptor(key, key_size);

    size_t enc_msg_size = ((msg_size + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    unsigned char enc_buff[enc_msg_size];
    memset(enc_buff, 0, sizeof(enc_buff));

    encryptor.encrypt(file_info.file_text, enc_buff);

    std::cout << "Original message: \n" << msg << std::endl;
    std::cout << "Key: \n" << key << std::endl;
    std::cout << "Encrypted message: \n";
    for(size_t i = 0; i < enc_msg_size ;i++){
        std::cout << enc_buff[i];
    }

    // Save original message size + encrypted message
    data.assign(reinterpret_cast<const char*>(enc_buff), enc_msg_size);
    data = std::to_string(msg_size) + " " + data;

    ui->updateLabel->setEnabled(true);
    ui->updateLabel->setStyleSheet("color: rgb(204,0,0)");
    ui->updateLabel->setText("File encrypted successfully");
}

void MainWindow::on_decryptButton_clicked()
{
    if(file_info.ext != "aes") {
        ui->updateLabel->setEnabled(true);
        ui->updateLabel->setText("Error: Please select a .aes file for decryption");
        return;
    }

    // First token in file is the original message size
    int space_pos = file_info.file_text.find(" ");
    const size_t orig_msg_size = std::stoi(file_info.file_text.substr(0, space_pos));
    std::string msg = file_info.file_text.substr(space_pos+1);

    const std::string key = "12345678912345678912345678912345";   // AES Key
    const size_t key_size = 256;

    AES_Encryptor encryptor(key, key_size);

    unsigned char dec_buff[orig_msg_size];
    memset(dec_buff, 0, sizeof(dec_buff));

    encryptor.decrypt(msg, dec_buff);

    std::cout << "Encrypted message: \n" << msg << std::endl;
    std::cout << "Key: \n" << key << std::endl;
    std::cout << "Decrypted message: \n";
    for(size_t i = 0; i < orig_msg_size ;i++){
        std::cout << dec_buff[i];
    }

    // Save decrypted data
    data.assign(reinterpret_cast<const char*>(dec_buff), orig_msg_size);

    ui->updateLabel->setEnabled(true);
    ui->updateLabel->setStyleSheet("color: rgb(204,0,0)");
    ui->updateLabel->setText("File decrypted successfully");
}

void MainWindow::on_exitButton_clicked()
{
    this->close();
}
