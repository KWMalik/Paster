#include <QClipboard>
#include "PasterWindow.h"

#define NUM_LANGUAGES 15
#define NUM_EXPIRE_TIMES 5

typedef struct {
    QString language;
    QString display;
} Language_T;

typedef struct {
    QString display;
    QString value;
} ExpireTime_T;

static Language_T languages[] = {
    { "cpp", "C++" },
    { "csharp", "C#" },
    { "css", "CSS" },
    { "delphi", "Delphi" },
    { "groovy", "Groovy" },
    { "java", "Java" },
    { "jscript", "JavaScript" },
    { "perl", "Perl" },
    { "php", "PHP" },
    { "plain", "Plain Text" },
    { "python", "Python" },
    { "ruby", "Ruby" },
    { "sql", "SQL" },
    { "vb", "Visual Basic" },
    { "xml", "XML/HTML" }
};

static ExpireTime_T expireTimes[] = {
    { "Never", "0" },
    { "10 Minutes", "600" },
    { "1 Hour", "3600" },
    { "1 Day", "86400" },
    { "1 Month", "2592000" }
};


PasterWindow::PasterWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_textEdit = new QTextEdit;
    m_checkBoxPrivate = new QCheckBox("Private?");
    m_comboBoxLanguage = new QComboBox;
    m_comboBoxExpireTime = new QComboBox;
    m_paste = new QPushButton("Paste");
    m_manager = new QNetworkAccessManager;

    QObject::connect(m_paste, SIGNAL(clicked()), this, SLOT(paste()));
    QObject::connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(pasteFinished(QNetworkReply*)));

    for(int x = 0; x < NUM_LANGUAGES; x++) {
        m_comboBoxLanguage->addItem(languages[x].display);
    }

    for(int x = 0; x < NUM_EXPIRE_TIMES; x++) {
        m_comboBoxExpireTime->addItem(expireTimes[x].display);
    }

    m_splitter = new QSplitter;
    m_splitter->setOrientation(Qt::Vertical);

    QWidget* bottom = new QWidget;
    QHBoxLayout* hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(m_checkBoxPrivate);
    hBoxLayout->addWidget(m_comboBoxLanguage);
    hBoxLayout->addWidget(m_comboBoxExpireTime);
    hBoxLayout->addWidget(m_paste);
    bottom->setLayout(hBoxLayout);

    m_splitter->addWidget(m_textEdit);
    m_splitter->addWidget(bottom);

    setCentralWidget(m_splitter);
    setWindowTitle(tr("Paster"));
}

PasterWindow::~PasterWindow()
{

}

void PasterWindow::paste()
{
    QNetworkRequest request(QUrl("http://paste.strictfp.com/index.php"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray data;
    QUrl params;

    params.addQueryItem("paste", m_textEdit->toPlainText());
    params.addQueryItem("language", languages[m_comboBoxLanguage->currentIndex()].language);
    params.addQueryItem("submit", "Paste");
    params.addQueryItem("expires", expireTimes[m_comboBoxExpireTime->currentIndex()].value);

    if(m_checkBoxPrivate->checkState() == Qt::Checked) {
        params.addQueryItem("private", "Private");
    }

    data = params.encodedQuery();

    m_manager->post(request, data);
}

void PasterWindow::pasteFinished(QNetworkReply* reply)
{
    QString url = "http://paste.strictfp.com/" + reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl().toString();
    QApplication::clipboard()->setText(url);

    QMessageBox::information(this, "Success", "Paste link copied to clipboard.");
}
