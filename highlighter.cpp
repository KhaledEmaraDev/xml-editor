#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    tagFormat.setForeground(Qt::darkBlue);
    tagFormat.setFontWeight(QFont::Bold);
    rule.format = tagFormat;
    rule.pattern = QRegularExpression(QStringLiteral("</?[^\\/>]+\\b"));
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("[/\\?]?>"));
    highlightingRules.append(rule);

    commentFormat.setForeground(Qt::red);

    valueFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("\"[^\"]*\""));
    rule.format = valueFormat;
    highlightingRules.append(rule);

    attributeFormat.setFontWeight(QFont::Bold);
    attributeFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression(QStringLiteral("\\b\\w*:?\\w+="));
    rule.format = attributeFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegularExpression(QStringLiteral("<!--"));
    commentEndExpression = QRegularExpression(QStringLiteral("-->"));
}

void Highlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                    + match.capturedLength();
        }
        setFormat(startIndex, commentLength, commentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
