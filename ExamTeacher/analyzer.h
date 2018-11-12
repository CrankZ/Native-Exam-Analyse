#ifndef Analyzer_H
#define Analyzer_H

#include <QObject>
#include <QThread>
#include "examinfo.h"

class Analyzer : public QObject
{
    Q_OBJECT
public:
    explicit Analyzer(QObject *parent = nullptr);

private:
    void analyzeEachQuestionAccuracy(QMap<QString, QMap<int, bool>> stuAnsIsRight);
private:
    QMap<QString, int> classStuNumMap;
    QMap<QString, int> classPassStuNumMap;
    QMap<QString, int> classRealScoreMap;

    QMap<QString, double> classAverageScoreMap;
    QMap<QString, double> classPassRateMap;
    QMap<QString, QMap<int ,double>> eachQuestionAccuracy;
    int globalQuestionNum = 0;

signals:
    void makeWidgetClassInfoSignal(QMap<QString, double>, QMap<QString, double>);

public slots:
    void analyzeSlot(ExamInfo *examInfo);
};

#endif // Analyzer_H
