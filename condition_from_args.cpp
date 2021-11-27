#include "condition_from_args.h"

#include <QDebug>
#include <QRegExp>
#include <QTextCodec>

std::optional<PrefixData> getPrefixData(char* command) {
    if (command[0] != '-') {
        return std::nullopt;
    }
    int position = 1;
    if (command[position] == '-') {
        ++position;
    }
    ConditionType type = ConditionType::TYPE_AND;
    bool deleteFlag{false};
    bool ignoreCase{true};
    bool prefixNotEnds{true};
    while (prefixNotEnds) {
        switch (command[position]) {
            case '+':
                if (type == ConditionType::TYPE_NOT)
                    type = ConditionType::TYPE_XOR;
                else
                    type = ConditionType::TYPE_OR;
                ++position;
                break;
            case 'n':
                if (type == ConditionType::TYPE_OR)
                    type = ConditionType::TYPE_XOR;
                else
                    type = ConditionType::TYPE_NOT;
                ++position;
                break;
            case 'd':
                deleteFlag = true;
                ++position;
                break;
            case 'i':
                ignoreCase = true;
                ++position;
                break;
            case 'I':
                ignoreCase = false;
                ++position;
                break;
            case 'f':
                type = ConditionType::TYPE_FILTER;
                deleteFlag = true;
                ++position;
                break;
            default:
                prefixNotEnds = false;
                break;
        }
    }
    return PrefixData{deleteFlag, ignoreCase, type, position};
};

Commands getCommand(char* command) {
    Commands resultFunc;
    if (!strcmp("start", command) || !strcmp("s", command)) {
        resultFunc = Commands::START;
    } else if (!strcmp("ends", command) || !strcmp("e", command)) {
        resultFunc = Commands::ENDS;
    } else if (!strcmp("contains", command) || !strcmp("c", command)) {
        resultFunc = Commands::CONTAINS;
    } else if (!strcmp("regexp", command) || !strcmp("r", command)) {
        resultFunc = Commands::REGEXP;
    } else {
        resultFunc = Commands::NONE;
    }
    return resultFunc;
};

std::function<bool(QString&)> lambdaFactory(Commands command, QString& param,
                                            bool ignoreCase) {
    Qt::CaseSensitivity sensetive;
    if (ignoreCase)
        sensetive = Qt::CaseInsensitive;
    else
        sensetive = Qt::CaseSensitive;
    std::function<bool(QString&)> func;
    switch (command) {
        case (Commands::ENDS):
            func = [param, sensetive](QString& line) {
                return line.endsWith(param, sensetive);
            };
            break;
        case (Commands::START):
            func = [param, sensetive](const QString& line) {
                return line.startsWith(param, sensetive);
            };
            break;
        case (Commands::CONTAINS):
            func = [param, sensetive](const QString& line) {
                return line.contains(param, sensetive);
            };
            break;
        case (Commands::REGEXP):
            func = [regular = QRegExp(param, sensetive)](const QString& line) {
                return line.contains(regular);
            };
            break;
        case (Commands::NONE):
            break;
    }
    return func;
}

std::function<bool(QString&)> mutableLambdaFactory(Commands command,
                                                   QString& param,
                                                   bool caseSensetive) {
    Qt::CaseSensitivity sensetive;
    if (caseSensetive)
        sensetive = Qt::CaseSensitive;
    else
        sensetive = Qt::CaseInsensitive;
    std::function<bool(QString&)> func;
    switch (command) {
        case (Commands::ENDS):
            func = [param, sensetive](QString& line) {
                bool condition = line.endsWith(param, sensetive);
                if (condition) {
                    line.chop(param.length());
                }
                return condition;
            };
            break;
        case (Commands::START):
            func = [param, sensetive](QString& line) {
                bool condition = line.startsWith(param, sensetive);
                if (condition) {
                    line.remove(0, param.length());
                }
                return condition;
            };
            break;
        case (Commands::CONTAINS):
            func = [param, sensetive](QString& line) {
                int pos = line.indexOf(param, sensetive);
                if (pos == -1) {
                    return false;
                }
                line.remove(pos, param.length());
                return true;
            };
            break;
        case (Commands::REGEXP):
            func = [regular = QRegExp(param, sensetive)](QString& line) {
                if (regular.indexIn(line) == -1) return false;
                if (regular.cap(1).isEmpty()) return true;
                line.remove(regular.pos(1), regular.cap(1).length());
                return true;
            };
            break;
        case (Commands::NONE):
            break;
    }
    return func;
}

void conditionsFromArgs(int argc, char* argv[],
                        std::vector<ConditionElemet>& conditions) {
    std::optional<PrefixData> prefix;
    QTextCodec* codec = QTextCodec::codecForLocale();
    Commands command;
    bool option{true};
    for (int i = 1; i < argc; ++i) {
        if (option) {
            prefix = getPrefixData(argv[i]);
            if (!prefix) continue;
            command =
                getCommand(argv[i] + prefix->length);  //address arithmetic
            if (command == Commands::NONE) continue;
            option = false;
        } else {
            QString argument(codec->toUnicode(argv[i]));
            std::function<bool(QString&)> func;
            if (prefix->deleteFlag) {
                func =
                    mutableLambdaFactory(command, argument, prefix->ignoreCase);
            } else {
                func = lambdaFactory(command, argument, prefix->ignoreCase);
            }
            conditions.push_back(ConditionElemet{prefix->type, func});
            option = true;
        }
    }
}
