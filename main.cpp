#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "condition_from_args.h"
#include "scan_dir.h"

int main(int argc, char* argv[]) {
    std::vector<ConditionElemet> conditions;
    conditions.reserve(argc / 2);
    conditionsFromArgs(argc, argv, conditions);
    if (conditions.size() == 0) {
        qDebug() << "Параметры не заданы!!\n";
        return 2;
    }
    QFile file("Список.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Не удалось открыть файл!\n";
        return 1;
    }
    QTextStream stream(&file);
    std::function<void(QString&)> writeToFile{
        [&stream](QString& line) -> void { stream << line << '\n'; }};
    std::function<bool(QString&)> checkCondition{
        [&conditions](QString& name) -> bool {
            for (auto f : conditions) {
                switch (f.type) {
                    case ConditionType::TYPE_AND:
                        if (!f.condition(name)) {
                            return false;
                        }
                        break;
                    case ConditionType::TYPE_OR:
                        if (f.condition(name)) {
                            return true;
                        }
                        break;
                    case ConditionType::TYPE_FILTER:
                        f.condition(name);
                        break;
                    case ConditionType::TYPE_XOR:
                        if (f.condition(name)) {
                            return true;
                        }
                    case ConditionType::TYPE_NOT:
                        if (f.condition(name)) {
                            return false;
                        }
                        break;
                    default:
                        return false;
                }
            }
            return true;
        }};
    stream << QString("Сгенерированный список\n");
    scanDir(QDir::currentPath(), checkCondition, writeToFile);

    file.close();
    return 0;
}
