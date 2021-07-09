#include "scan_dir.h"

void scanDir(const QString& directory, std::function<bool(QString&)> checker,
             std::function<void(QString&)> writer) {
    QDir dir = QDir(directory);
    dir.setFilter(QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Files |
                  QDir::Dirs);
    QList<QFileInfo> fileList = dir.entryInfoList();
    for (QList<QFileInfo>::const_iterator constIter = fileList.constBegin();
         constIter != fileList.constEnd(); ++constIter) {
        if (constIter->isDir()) {
            scanDir(constIter->absoluteFilePath(), checker, writer);
        } else if (constIter->isFile()) {
            QString name = constIter->fileName();
            if (checker(name)) writer(name);
        }
    }
}
