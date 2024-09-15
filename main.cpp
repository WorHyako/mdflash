#include <QApplication>
#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMenuBar>
#include <QFont>
#include <QFile>
#include <QTextStream>
#include "markdown_processor.h"

class MarkdownViewer : public QMainWindow {
public:
    MarkdownViewer() {
        setWindowTitle("mdflash - Markdown Viewer");
        
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);
        
        textEdit = new QTextEdit(this);
        textEdit->setReadOnly(true);
        textEdit->setFont(QFont("Arial", 12));
        layout->addWidget(textEdit);
        
        createMenus();
        
        resize(800, 600);
    }

    void openFile(const QString &fileName) {
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            in.setCodec("UTF-8");
            QString content = in.readAll();
            textEdit->setHtml(MarkdownProcessor::processContent(content));
            file.close();
        }
    }
}


private:
    void createMenus() {
        QMenu *fileMenu = menuBar()->addMenu("File");

        QAction *openAction = new QAction("Open", this);
        connect(openAction, &QAction::triggered, this, &MarkdownViewer::openFileDialog);
        fileMenu->addAction(openAction);

        QAction *exitAction = new QAction("Exit", this);
        connect(exitAction, &QAction::triggered, this, &QWidget::close);
        fileMenu->addAction(exitAction);
    }

    void openFileDialog() {
        QString fileName = QFileDialog::getOpenFileName(this, "Open Markdown File", "", "Markdown Files (*.md);;All Files (*)");
        openFile(fileName);
    }

    QTextEdit *textEdit;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MarkdownViewer viewer;
    viewer.show();

    if (argc > 1) {
        viewer.openFile(QString::fromLocal8Bit(argv[1]));
    }

    return app.exec();
}
