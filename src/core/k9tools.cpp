//
// C++ Implementation: k9tools
//
// Description:
//
// Developer: Pauline123 <pauline123@users.sf.net>, (C) 2014
//
// Author: Jean-Michel PETIT <k9copy@free.fr>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "k9tools.h"
#include "k9common.h"
#include <QDir>
#include <QComboBox>
#include <QApplication>
#include <QStyle>
#ifdef __FreeBSD__
#include <sys/mount.h>
#else
#include <sys/vfs.h>
#endif

#include "k9dialogs.h"

#include <kdesu/su.h>
#include <KPasswordDialog>
#include <KUser>

#include <QDebug>
#include <stdlib.h>
#include <time.h>
#include <QProcess>
#include <QThread>

QStringList m_temporaryDirs;
QString g_password="";
bool gBatchCopy=false;
Qt::HANDLE g_mainThread;

void k9Tools::setMainThread(){
    g_mainThread=QThread::currentThreadId();
}

static bool isMainThread();

const QString  k9Tools::getTempPath() {
QString sPath;

sPath=QString("%1/%2/").arg(QDir::tempPath() + '/' + "k9copy/");

return sPath;
}

void k9Tools::addTemporaryDir(const QString & dir) {
    m_temporaryDirs << dir;
}

void k9Tools::processEvents() {
    if (QThread::currentThreadId()== g_mainThread)
        qApp->processEvents(QEventLoop::AllEvents,1);
    else
        qDebug() << "Not the main thread processEvents skipped";
}

bool k9Tools::getBatchCopy() {
    return gBatchCopy;
}

void k9Tools::setBatchCopy(bool _value) {
    gBatchCopy=_value;
}

QString k9Tools::findExe(const QString _program) {            
    QString path(qgetenv("PATH"));

    QStringList paths=path.split(":");
    foreach (QString p,paths) {
        QString fullPath=QDir::cleanPath(p+"/"+_program);
        if (QFile::exists(fullPath))
            return fullPath;
    }
    return _program;
}

bool k9Tools::checkProgram(QString _progName,bool _msg) {
    #if QT_VERSION >= 0x050000
    bool res=QStandardPaths::findExecutable( _progName) !=NULL ;
    #else // Qt4 Code
    bool res= KStandardDirs::findExe( _progName,NULL,false) !=NULL ;
    #endif
    if (_msg && !res)
        k9Dialogs::error(i18n("Error starting program %1", _progName),i18n("Running program"));
    return res;

}

bool k9Tools::runAsRoot(const QString &_command,const QString &_description) {

    KDESu::SuProcess suprocess("root",_command.toUtf8());
    int res=3;
    while (res==3) {
        if (suprocess.checkInstall(g_password.toUtf8())!=0) {
            KPasswordDialog pwd(k9Dialogs::getMainWidget(),KPasswordDialog::UsernameReadOnly  );
            KUser user;
            QString username=suprocess.useUsersOwnPassword()?user.loginName():QString("root");
            pwd.setUsername(username);
            pwd.setPrompt(i18n("Enter the root password"));
            pwd.addCommentLine("",_description);
            if (!pwd.exec())
                return false;
            g_password=pwd.password();
            res=suprocess.exec(g_password.toUtf8());
        } else
            res=suprocess.exec(g_password.toUtf8());
        if (res==3)
            k9Dialogs::error(i18n("The root password is not valid"),i18n("Running program"));
    }
    return res==0;
}

void k9Tools::clearOutput() {
    foreach(QString dir,m_temporaryDirs) {
        qDebug() << "clear " << dir;
        k9Tools::clearOutput(dir);
    }

}

const QString k9Tools::randomFileName() {
    static int prev=0;
    QTime t(0,0,0);
    srand(t.msecsTo(QTime::currentTime()));
    int res=rand();
    if (res==prev) res=rand();
    prev=res;
    return QString::number(res % 99999999);
}

void k9Tools::clearOutput(QString name) {
    QDir dir(name);
    //delete files in directory
    QStringList lst = dir.entryList(QStringList( "*"),QDir::Files |QDir::Hidden );
    foreach (QString c,lst) {
        //QString c(( *it ).latin1() );
        dir.remove (c);
    }
    //scanning subdir
    QStringList lstdir = dir.entryList( QStringList("*"),QDir::Dirs );
    foreach ( QString c,lstdir) {
        if ((c!=".") && c!="..") {
            clearOutput(dir.absoluteFilePath(c));
            dir.rmdir(c);
        }
    }

}

void k9Tools::setComboText(QComboBox *_cb,const QString &_text) {
    int i = _cb->findText(_text);
    if (i != -1)
        _cb->setCurrentIndex(i);
    else if (_cb->isEditable())
        _cb->setEditText(_text);
    else
        _cb->setItemText(_cb->currentIndex(), _text);
}

long k9Tools::getFreeSpace(const QString &_path) {
    struct statfs sfs;
#if QT_VERSION >= 0x050000
    if (statfs(_path.toLatin1(),&sfs) !=-1) {
#else
    if (statfs(_path.toAscii(),&sfs) !=-1) {
#endif
        uint64_t fs=((uint64_t)sfs.f_bavail*(uint64_t)sfs.f_bsize);
        fs/=(1024*1024);
        return fs;
    } else
        return -1;

}


