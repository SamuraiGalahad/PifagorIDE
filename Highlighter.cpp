#include "Highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::magenta);
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywordPatterns[] = {
        QStringLiteral("\\bchar\\b"), QStringLiteral("\\bconst\\b"),
        QStringLiteral("\\bint\\b"), QStringLiteral("\\bsignal\\b"),
        QStringLiteral("\\btypedef\\b"), QStringLiteral("\\btype\\b"),
        QStringLiteral("\\bbool\\b"), QStringLiteral("\\bblock\\b"),
        QStringLiteral("\\bbreak\\b"), QStringLiteral("\\bdup\\b"),
        QStringLiteral("\\bdatalist\\b"), QStringLiteral("\\bdelaylist\\b"),
        QStringLiteral("\\belse\\b"), QStringLiteral("\\berror\\b"),
        QStringLiteral("\\bfalse\\b"), QStringLiteral("\\bfloat\\b"),
        QStringLiteral("\\bfunc\\b"), QStringLiteral("\\bfuncdef\\b"),
        QStringLiteral("\\bnil\\b"), QStringLiteral("\\bparlist\\b"),
        QStringLiteral("\\bprefunc\\b"), QStringLiteral("\\breturn\\b"),
        QStringLiteral("\\btrue\\b")
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    quotationFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::green);
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::darkGreen);

    commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));


}

void Highlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : std::as_const(highlightingRules)) {
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
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
