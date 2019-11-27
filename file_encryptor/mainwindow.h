#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_browseButton_clicked();

    void on_saveButton_clicked();

    void on_encryptButton_clicked();

    void on_decryptButton_clicked();

    void on_exitButton_clicked();

private:
    Ui::MainWindow *ui;

    struct file_info_t {  // Info of the selected file
        std::string file_name;
        std::string file_text;
        std::string path;
        qint64 size;
        std::string ext;
    }file_info;

    std::string data;   // Encrypted/Decrypted data
};

#endif // MAINWINDOW_H
