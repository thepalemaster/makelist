#pragma once
#include <QString>
#include <functional>
#include <optional>
#include <vector>

enum class ConditionType {
    TYPE_OR,
    TYPE_AND,
    TYPE_NOT,
    TYPE_XOR,  //добавляется комбинацией префиксов '+' и 'n'
    TYPE_FILTER,
    TYPE_NONE
};

enum class Commands { ENDS, START, CONTAINS, REGEXP, NONE };

struct ConditionElemet {
    ConditionType type;
    std::function<bool(QString &)> condition;
};

struct PrefixData {
    bool deleteFlag;
    bool ignoreCase;
    ConditionType type;
    int length;
};

void conditionsFromArgs(int, char *[], std::vector<ConditionElemet> &);
