#ifndef XML_HIGHLIGHTER_H
#define XML_HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class XmlHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    XmlHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat tagFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat valueFormat;
    QTextCharFormat attributeFormat;
};

#endif // XML_HIGHLIGHTER_H
