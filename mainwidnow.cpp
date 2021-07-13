#include <QtWidgets>
#include <QTextStream>
#include <QDebug>

#include "mainwindow.h"

#include "lib/xmltree.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupFileMenu();
    setupToolBar();
    setupHelpMenu();
    setupEditor();

    setCentralWidget(editor);
    setWindowTitle("XML Editor");

#ifndef QT_NO_SESSIONMANAGER
    QGuiApplication::setFallbackSessionManagementEnabled(false);
    connect(qApp, &QGuiApplication::commitDataRequest,
            this, &MainWindow::commitData);
#endif
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::documentWasModified()
{
    setWindowModified(editor->document()->isModified());
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About XML Editor"),
                       tr("<p>This <b>XML Editor</b> is part of a project under the " \
                   "<b>Programming with Data Structures</b> course.</p>"));
}

void MainWindow::newFile()
{
    editor->clear();
}

void MainWindow::openFile(const QString &path)
{
    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "../xml-editor/samples/", "XML files (*.xml)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
#ifndef QT_NO_CURSOR
            QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
            editor->setPlainText(file.readAll());
            editor->document()->setModified(false);
            setWindowModified(false);
#ifndef QT_NO_CURSOR
            QGuiApplication::restoreOverrideCursor();
#endif

            file.seek(0);
            QTextStream in(&file);
            try {
                XMLTree::syntax_check(in);
            }  catch (const QVector<QPair<int, QString>> &ex) {
                for (const auto & error: ex) {
                    editor->displayError(error.first, error.second);
                }
            }
        } else {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(fileName), file.errorString()));
            return;
        }
    }
}

bool MainWindow::maybeSave()
{
    if (!editor->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
            = QMessageBox::warning(this, tr("Application"),
                                   tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return saveFile();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

bool MainWindow::saveFile(const QString &path)
{
    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", "XML files (*.xml)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::WriteOnly | QFile::Text)) { return true; }
    }

    return false;
}

bool MainWindow::checkSyntax()
{
    editor->clearErrors();

    QString str = editor->toPlainText();
    QTextStream in(&str, QIODevice::ReadOnly);
    try {
        XMLTree::syntax_check(in);

        return true;
    }  catch (const QVector<QPair<int, QString>> &ex) {
        for (const auto & error: ex) {
            editor->displayError(error.first, error.second);
        }

        return false;
    }
}

void MainWindow::minify()
{
    QString str = editor->toPlainText();
    QTextStream in(&str, QIODevice::ReadOnly);

    if (checkSyntax()) {
        XMLTree tree;
        tree.load(in);
        editor->setPlainText(tree.dump());
    }
}

void MainWindow::prettify()
{
    QString str = editor->toPlainText();
    QTextStream in(&str, QIODevice::ReadOnly);

    if (checkSyntax()) {
        XMLTree tree;
        tree.load(in);
        editor->setPlainText(tree.dump(2));
    }
}

void MainWindow::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    editor = new CodeEditor;
    editor->setFont(font);

    highlighter = new Highlighter(editor->document());

    connect(editor->document(), &QTextDocument::contentsChanged,
            this, &MainWindow::documentWasModified);
}

void MainWindow::setupFileMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    fileMenu->addAction(tr("&New"), this,
                        &MainWindow::newFile, QKeySequence::New);
    fileMenu->addAction(tr("&Open..."),
                        this, [this](){ openFile(); }, QKeySequence::Open);
    fileMenu->addAction(tr("&Save..."),
                        this, [this](){ openFile(); }, QKeySequence::Save);
    fileMenu->addAction(tr("E&xit"), qApp,
                        &QApplication::quit, QKeySequence::Quit);
}

void MainWindow::setupToolBar()
{
    QToolBar *editToolBar = addToolBar(tr("Edit"));

    const QIcon minifyIcon = QIcon(":/images/save.png");
    QAction *minifyAct = new QAction(minifyIcon, tr("&Minify..."), this);
    connect(minifyAct, &QAction::triggered, this, &MainWindow::minify);
    editToolBar->addAction(minifyAct);
}

void MainWindow::setupHelpMenu()
{
    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(helpMenu);

    helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
}

#ifndef QT_NO_SESSIONMANAGER
void MainWindow::commitData(QSessionManager &manager)
{
    if (manager.allowsInteraction()) {
        if (!maybeSave())
            manager.cancel();
    } else {
        if (editor->document()->isModified())
            saveFile();
    }
}
#endif
