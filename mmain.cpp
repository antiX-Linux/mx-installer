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

MInstall *minstall;
bool firstShow;

MMain::MMain()
{
    setupUi(this);
    minstall = new MInstall(mainFrame);
    minstall->move(5,0);
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
    qDebug() << "close event";
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
        minstall->adjustSize();
    }
}

void MMain::resizeEvent(QResizeEvent *)
{
    minstall->resize(mainFrame->size());
    mainHelp->resize(tab->size());
    helpbackdrop->resize(mainHelp->size());
}
