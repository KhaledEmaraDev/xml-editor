#include <QtWidgets>
#include <QTextStream>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "mainwindow.h"

#include "lib/xmltree.h"
#include "lib/json.h"
#include "compress/huffman.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupEditor();
    setCentralWidget(tabber);

    setupActions();
    setupStatusBar();

    setWindowTitle("XML Editor[*]");

#ifndef QT_NO_SESSIONMANAGER
    QGuiApplication::setFallbackSessionManagementEnabled(false);
    connect(qApp, &QGuiApplication::commitDataRequest,
            this, &MainWindow::commitData);
#endif

    setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        xmlEditor->clear();
        setCurrentFile(QString());
    }
}

void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open File"),
                                                        "../xml-editor/samples/",
                                                        "XML files (*.xml);;Compressed XML files (*.hxml)"
                                                       );
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QFileDialog dialog(this,
                       tr("Save File As"),
                       "../xml-editor/samples/",
                       "XML files (*.xml);;Compressed XML files (*.hxml)"
                      );
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().constFirst());
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About XML Editor"),
                       tr("<p>This <b>XML Editor</b> is part of a project under the " \
                   "<b>Programming with Data Structures</b> course.</p>"));
}

void MainWindow::documentWasModified()
{
    setWindowModified(xmlEditor->document()->isModified());
}

bool MainWindow::checkSyntax()
{
    xmlEditor->clearErrors();

    QString str = xmlEditor->toPlainText();
    QTextStream in(&str, QIODevice::ReadOnly);
#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    try {
        XMLTree::syntax_check(in);

#ifndef QT_NO_CURSOR
        QGuiApplication::restoreOverrideCursor();
#endif

        return true;
    } catch (const QVector<QPair<int, QString>> &ex) {
        bool modified = xmlEditor->document()->isModified();
        for (const auto & error: ex) {
            xmlEditor->displayError(error.first, error.second);
        }
        xmlEditor->document()->setModified(modified);
        documentWasModified();
    } catch (const std::exception &ex) {
        statusBar()->showMessage(tr(ex.what()));
    } catch (const std::string &ex) {
        statusBar()->showMessage(tr(ex.c_str()));
    } catch (const QString &ex) {
        statusBar()->showMessage(ex);
    } catch (...) {
        statusBar()->showMessage(tr("An unexpected error occurred"));
    }
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif

    return false;
}

void MainWindow::minify()
{
    QString str = xmlEditor->toPlainText();
    QTextStream in(&str, QIODevice::ReadOnly);

    if (checkSyntax()) {
#ifndef QT_NO_CURSOR
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        try {
            XMLTree tree;
            tree.load(in);
            xmlEditor->setPlainText(tree.dump());
            xmlEditor->document()->setModified(true);
        } catch (const std::exception &ex) {
            statusBar()->showMessage(tr(ex.what()));
        } catch (const std::string &ex) {
            statusBar()->showMessage(tr(ex.c_str()));
        } catch (const QString &ex) {
            statusBar()->showMessage(ex);
        } catch (...) {
            statusBar()->showMessage(tr("An unexpected error occurred"));
        }
#ifndef QT_NO_CURSOR
        QGuiApplication::restoreOverrideCursor();
#endif
    }
}

void MainWindow::prettify()
{
    QString str = xmlEditor->toPlainText();
    QTextStream in(&str, QIODevice::ReadOnly);

    if (checkSyntax()) {
#ifndef QT_NO_CURSOR
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        try {
            XMLTree tree;
            tree.load(in);
            xmlEditor->setPlainText(tree.dump(2));
            xmlEditor->document()->setModified(true);
        } catch (const std::exception &ex) {
            statusBar()->showMessage(tr(ex.what()));
        } catch (const std::string &ex) {
            statusBar()->showMessage(tr(ex.c_str()));
        } catch (const QString &ex) {
            statusBar()->showMessage(ex);
        } catch (...) {
            statusBar()->showMessage(tr("An unexpected error occurred"));
        }
#ifndef QT_NO_CURSOR
        QGuiApplication::restoreOverrideCursor();
#endif
    }
}

void MainWindow::convertToJson()
{
    QString str = xmlEditor->toPlainText();
    QTextStream in(&str, QIODevice::ReadOnly);

    if (checkSyntax()) {
#ifndef QT_NO_CURSOR
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        try {
            XMLTree tree;
            tree.load(in);
            jsonEditor->setPlainText(JSON::xml2json(tree, 2));
            tabber->setCurrentIndex(1);
        } catch (const std::exception &ex) {
            statusBar()->showMessage(tr(ex.what()));
        } catch (const std::string &ex) {
            statusBar()->showMessage(tr(ex.c_str()));
        } catch (const QString &ex) {
            statusBar()->showMessage(ex);
        } catch (...) {
            statusBar()->showMessage(tr("An unexpected error occurred"));
        }
#ifndef QT_NO_CURSOR
        QGuiApplication::restoreOverrideCursor();
#endif
    }
}

void MainWindow::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    xmlEditor = new CodeEditor;
    xmlEditor->setFont(font);

    xmlHighlighter = new XmlHighlighter(xmlEditor->document());

    connect(xmlEditor->document(), &QTextDocument::contentsChanged,
            this, &MainWindow::documentWasModified);

    jsonEditor = new CodeEditor;
    jsonEditor->setFont(font);

    jsonHighlighter = new JsonHighlighter(jsonEditor->document());

    tabber = new QTabWidget(this);
    tabber->addTab(xmlEditor, "XML File");
    tabber->addTab(jsonEditor, "Json File");
}

void MainWindow::setupActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));

    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);

    const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
    QAction *saveAsAct = fileMenu->addAction(saveAsIcon, tr("Save &As..."), this, &MainWindow::saveAs);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));

    fileMenu->addSeparator();

    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QToolBar *editToolBar = addToolBar(tr("Edit"));
#ifndef QT_NO_CLIPBOARD
    const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
    QAction *cutAct = new QAction(cutIcon, tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                                "clipboard"));
    connect(cutAct, &QAction::triggered, xmlEditor, &CodeEditor::cut);
    editMenu->addAction(cutAct);
    editToolBar->addAction(cutAct);

    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/images/copy.png"));
    QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                                 "clipboard"));
    connect(copyAct, &QAction::triggered, xmlEditor, &CodeEditor::copy);
    editMenu->addAction(copyAct);
    editToolBar->addAction(copyAct);

    const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
    QAction *pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                                  "selection"));
    connect(pasteAct, &QAction::triggered, xmlEditor, &CodeEditor::paste);
    editMenu->addAction(pasteAct);
    editToolBar->addAction(pasteAct);

    menuBar()->addSeparator();
#endif // !QT_NO_CLIPBOARD

    QMenu *codeMenu = menuBar()->addMenu(tr("&Code"));
    QToolBar *codeToolBar = addToolBar(tr("Code"));

    const QIcon minifyIcon = QIcon(":/images/minify.png");
    QAction *minifyAct = new QAction(minifyIcon, tr("&Minify"), this);
    minifyAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_M));
    minifyAct->setStatusTip(tr("Minify XML File"));
    connect(minifyAct, &QAction::triggered, this, &MainWindow::minify);
    codeMenu->addAction(minifyAct);
    codeToolBar->addAction(minifyAct);

    const QIcon prettifyIcon = QIcon(":/images/prettify.png");
    QAction *prettifyAct = new QAction(prettifyIcon, tr("&Prettify"), this);
    prettifyAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_P));
    prettifyAct->setStatusTip(tr("Prettify XML File"));
    connect(prettifyAct, &QAction::triggered, this, &MainWindow::prettify);
    codeMenu->addAction(prettifyAct);
    codeToolBar->addAction(prettifyAct);

    const QIcon checkSyntaxIcon = QIcon(":/images/check_syntax.png");
    QAction *checkSyntaxAct = new QAction(checkSyntaxIcon, tr("&Check Syntax"), this);
    checkSyntaxAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_C));
    checkSyntaxAct->setStatusTip(tr("Check XML File Syntax"));
    connect(checkSyntaxAct, &QAction::triggered, this, &MainWindow::checkSyntax);
    codeMenu->addAction(checkSyntaxAct);
    codeToolBar->addAction(checkSyntaxAct);

    const QIcon convertToJsonIcon = QIcon(":/images/convert_to_json.png");
    QAction *convertToJsonAct = new QAction(convertToJsonIcon, tr("Convert to &Json"), this);
    convertToJsonAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_J));
    convertToJsonAct->setStatusTip(tr("Convert XML File to JSON"));
    connect(convertToJsonAct, &QAction::triggered, this, &MainWindow::convertToJson);
    codeMenu->addAction(convertToJsonAct);
    codeToolBar->addAction(convertToJsonAct);

    menuBar()->addSeparator();

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));

    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));

#ifndef QT_NO_CLIPBOARD
    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect(xmlEditor, &CodeEditor::copyAvailable, cutAct, &QAction::setEnabled);
    connect(xmlEditor, &CodeEditor::copyAvailable, copyAct, &QAction::setEnabled);
#endif // !QT_NO_CLIPBOARD
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

bool MainWindow::maybeSave()
{
    if (!xmlEditor->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
            = QMessageBox::warning(this, tr("XML Editor"),
                                   tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFileInfo fileInfo(fileName);

    if (fileInfo.suffix() == "xml") {
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(fileName), file.errorString()));
            return;
        }

        QTextStream in(&file);
#ifndef QT_NO_CURSOR
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        xmlEditor->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
        QGuiApplication::restoreOverrideCursor();
#endif
    } else if (fileInfo.suffix() == "hxml") {
        std::filebuf fb;
        if (fb.open(fileName.toStdString(), std::ios_base::in | std::ios_base::binary)) {
            std::istream is(&fb);

            std::stringbuf sb;
            std::ostream os(&sb);

            huffman huff;
            huff.decode(is, os);

#ifndef QT_NO_CURSOR
            QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
            xmlEditor->setPlainText(QString::fromStdString(sb.str()));
#ifndef QT_NO_CURSOR
            QGuiApplication::restoreOverrideCursor();
#endif

            fb.close();
        }
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"));
    checkSyntax();
}

bool MainWindow::saveFile(const QString &fileName)
{
    QString errorMessage;

    QFileInfo fileInfo(fileName);

    if (fileInfo.suffix() == "xml") {
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);

        QSaveFile file(fileName);
        if (file.open(QFile::WriteOnly | QFile::Text)) {
            QTextStream out(&file);
            xmlEditor->clearErrors();
            out << xmlEditor->toPlainText();
            if (!file.commit()) {
                errorMessage = tr("Cannot write file %1:\n%2.")
                        .arg(QDir::toNativeSeparators(fileName), file.errorString());
            }
        } else {
            errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                    .arg(QDir::toNativeSeparators(fileName), file.errorString());
        }

        QGuiApplication::restoreOverrideCursor();
    } else if (fileInfo.suffix() == "hxml") {
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);

        std::filebuf fb;
        if (fb.open(fileName.toStdString(), std::ios_base::out | std::ios_base::binary)) {
            xmlEditor->clearErrors();

            std::ostream os(&fb);

            std::stringbuf sb(xmlEditor->toPlainText().toStdString(), std::ios_base::in);
            std::istream is(&sb);

            huffman huff;
            huff.encode(is, os);

            fb.close();
        } else {
            errorMessage = tr("Cannot open file %1 for writing.")
                    .arg(QDir::toNativeSeparators(fileName));
        }

        QGuiApplication::restoreOverrideCursor();
    } else {
        errorMessage = tr("Unkown file type");
    }

    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, tr("Application"), errorMessage);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"));
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    xmlEditor->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}

#ifndef QT_NO_SESSIONMANAGER
void MainWindow::commitData(QSessionManager &manager)
{
    if (manager.allowsInteraction()) {
        if (!maybeSave())
            manager.cancel();
    } else {
        // Non-interactive: save without asking
        if (xmlEditor->document()->isModified())
            save();
    }
}
#endif
