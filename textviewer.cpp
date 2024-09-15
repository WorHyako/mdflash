#include <QApplication>
#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMenuBar>
#include <QFont>
#include <QFile>
#include <QTextStream>

class TextViewer : public QMainWindow {
public:
    TextViewer() {
        setWindowTitle("Текстовый просмотрщик");

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
                textEdit->setPlainText(in.readAll());
                file.close();
            }
        }
    }

private:
    void createMenus() {
        QMenu *fileMenu = menuBar()->addMenu("Файл");

        QAction *openAction = new QAction("Открыть", this);
        connect(openAction, &QAction::triggered, this, &TextViewer::openFileDialog);
        fileMenu->addAction(openAction);

        QAction *exitAction = new QAction("Выход", this);
        connect(exitAction, &QAction::triggered, this, &QWidget::close);
        fileMenu->addAction(exitAction);
    }

    void openFileDialog() {
        QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", "", "Текстовые файлы (*.txt);;Все файлы (*)");
        openFile(fileName);
    }

    QTextEdit *textEdit;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    TextViewer viewer;
    viewer.show();

    if (argc > 1) {
        viewer.openFile(QString::fromLocal8Bit(argv[1]));
    }

    return app.exec();
}
