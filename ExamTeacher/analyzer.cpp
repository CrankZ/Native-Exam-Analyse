#include "Analyzer.h"


Analyzer::Analyzer(QObject *parent) : QObject(parent)
{


}

/**
 * 计算每题正确率
 */
void Analyzer::analyzeEachQuestionAccuracy(QMap<QString, QMap<int, bool>> stuAnsIsRight)
{
    int xx = 0;
    for(int i = 0; i < stuAnsIsRight.count();i++) {
        QString type = stuAnsIsRight.keys().at(i);
        QMap<int, bool> ansIsRight = stuAnsIsRight.values().at(i);
        double accuracy = 0;
        double rightNum = 0;
        double typeQuestionCount = ansIsRight.count();
        int questionNum = 0;
        QMap<int ,double> eachAccuracy;
        for (int j = 0; j < typeQuestionCount; j++) {
            xx++;
//            qDebug() << "xx" << xx;
            questionNum = ansIsRight.keys().at(j);
            bool isRight = ansIsRight.values().at(j);
            if (isRight) {
                accuracy = (++rightNum) / typeQuestionCount;
            }
            eachAccuracy.insert(questionNum, accuracy);
        }
        eachQuestionAccuracy.insert(type, eachAccuracy);
    }

}

/**
 * 目前采取的方案是来一个加一个
 */
void Analyzer::analyzeSlot(ExamInfo *examInfo)
{
    qDebug() << "Analyzer,analyzeSlot: " << QThread::currentThreadId();
    QString MAC = examInfo->stuMAC;
    QString stuNo = examInfo->stuNo;
    QString stuName = examInfo->stuName;
    QString stuClassName = examInfo->stuClass;
    int stuRealScore = examInfo->stuRealScore;
    int totalScore = examInfo->paperTotalScore;
    QMap<QString, QMap<int, bool>> stuAnsIsRight = examInfo->stuAnsIsRight;

    analyzeEachQuestionAccuracy(stuAnsIsRight);
    int currentStuNum = classStuNumMap.value(stuClassName);
    classStuNumMap.insert(stuClassName, currentStuNum+1);
    int classRealScore = classRealScoreMap.value(stuClassName);
    classRealScore += stuRealScore;

    qDebug() << "stuClassName:" << stuClassName
             << "stuRealScore:" << stuRealScore
             << "classRealScore:" << classRealScore;

    classRealScoreMap.insert(stuClassName, classRealScore);
    // 所有班级的个数
    int classNum = classStuNumMap.count();
    qDebug() << "班级个数：" << classNum;

    for(int i = 0;i < classNum;i++) {
        QString className = classStuNumMap.keys().at(i);
        if(stuClassName == className) {

            double classAverageScore = 0.00;
            double classPassRate = 0.00;
            // 每个班级及格的学生
            // 几个分数为总分的60%
            if(stuRealScore > totalScore*0.6) {
                classPassStuNumMap.insert(className, classPassStuNumMap.count() + 1);
            }
            // 当前班级学生数量
            int classStuNum = classStuNumMap.value(className);
            double classRealScore = classRealScoreMap.value(className);
            double classPassStuNum = classPassStuNumMap.value(className);
            qDebug() << "classStuNum:" << classStuNum
                     << "classRealScore:" << classRealScore
                     << "classPassStuNum:" << classPassStuNum;
            classAverageScore = classRealScore / classStuNum; // 平均分
            classPassRate = classPassStuNum / classStuNum;   // 及格率
            classAverageScoreMap.insert(className, classAverageScore);
            classPassRateMap.insert(className, classPassRate);
//        qDebug() << "classAverageScore:" << classAverageScore;
//        qDebug() << "classPassRate:" << classPassRate;
        }
    }

    qDebug() << "这里:" << classAverageScoreMap.count() << "," << classPassRateMap.count();
    emit makeWidgetClassInfoSignal(classAverageScoreMap, classPassRateMap);

}
