#ifndef PASTERWINDOW_H
#define PASTERWINDOW_H

#include <QtGui>
#include <QtNetwork>

class PasterWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    PasterWindow(QWidget *parent = 0);
    ~PasterWindow();

private slots:
    void paste();
    void pasteFinished(QNetworkReply* reply);

private:
    QTextEdit* m_textEdit;
    QCheckBox* m_checkBoxPrivate;
    QSplitter* m_splitter;
    QComboBox* m_comboBoxLanguage;
    QComboBox* m_comboBoxExpireTime;
    QPushButton* m_paste;

    QNetworkAccessManager* m_manager;
};

#endif // PASTERWINDOW_H
