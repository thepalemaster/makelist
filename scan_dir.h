#pragma once
#include <QDir>
#include <QString>
#include <functional>

void scanDir(const QString&, std::function<bool(QString&)>,
             std::function<void(QString&)>);
