//
//  Copyright (C) 2003-2009 by Warren Woodford
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//

#include "mmain.h"
#include "minstall.h"

#include "QDebug"

MInstall *minstall;
bool firstShow;

MMain::MMain()
{
    setupUi(this);
    minstall = new MInstall(mainFrame);
    minstall->resize(mainFrame->size());
    mainHelp->resize(tab->size());
    helpbackdrop->resize(mainHelp->size());

    //setup system variables

    PROJECTNAME=getCmdOut("grep PROJECT_NAME /usr/share/installer-data/installer.conf |cut -d= -f2");
    PROJECTSHORTNAME=getCmdOut("grep PROJECT_SHORTNAME /usr/share/installer-data/installer.conf |cut -d= -f2");
    PROJECTVERSION=getCmdOut("grep VERSION /usr/share/installer-data/installer.conf |cut -d= -f2");
    setWindowTitle(PROJECTNAME + " " + tr("Installer"));
    firstShow = true;
}

MMain::~MMain() {
}

/////////////////////////////////////////////////////////////////////////

void MMain::setHelpText(const QString &text)
{
    mainHelp->setText(text);
}

void MMain::closeEvent(QCloseEvent *e)
{
    system("umount -l /mnt/antiX/home >/dev/null 2>&1");
    system("umount -l /mnt/antiX >/dev/null 2>&1");
    system("rm -r /mnt/antiX >/dev/null 2>&1");
    system("su -c 'xfconf-query --channel thunar-volman --property /automount-drives/enabled --set true' demo");
    QWidget::closeEvent(e);
}

/////////////////////////////////////////////////////////////////////////
// public slots

void MMain::closeClicked()
{
    close();
}

void MMain::showEvent(QShowEvent *)
{
    if (firstShow) {
        firstShow = false;
        minstall->firstRefresh(this);
    }
}

void MMain::resizeEvent(QResizeEvent *)
{
    minstall->resize(mainFrame->size());
    mainHelp->resize(tab->size());
    helpbackdrop->resize(mainHelp->size());
}

// util functions

QString MMain::getCmdOut(QString cmd)
{
    char line[260];
    const char* ret = "";
    FILE* fp = popen(cmd.toUtf8(), "r");
    if (fp == NULL) {
        return QString (ret);
    }
    int i;
    if (fgets(line, sizeof line, fp) != NULL) {
        i = strlen(line);
        line[--i] = '\0';
        ret = line;
    }
    pclose(fp);
    return QString (ret);
}
