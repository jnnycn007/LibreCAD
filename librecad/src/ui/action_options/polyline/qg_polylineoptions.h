/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software 
** Foundation and appearing in the file gpl-2.0.txt included in the
** packaging of this file.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** This copyright notice MUST APPEAR in all copies of the script!  
**
**********************************************************************/
#ifndef QG_POLYLINEOPTIONS_H
#define QG_POLYLINEOPTIONS_H

#include "lc_actionoptionswidgetbase.h"

class RS_ActionInterface;
class RS_ActionDrawPolyline;

namespace Ui {
    class Ui_PolylineOptions;
}

class QG_PolylineOptions : public LC_ActionOptionsWidgetBase{
    Q_OBJECT
public:
    QG_PolylineOptions();
    ~QG_PolylineOptions() override;
public slots:
    void close(); // fixme - rename
    void undo();
    void setModeToActionAndView(int m );
    void onAngleEditingFinished();
    void onRadiusEditingFinished();
    void onNegToggled(bool checked);
    void languageChange() override;
    void tbLineClicked();
    void tbTangentalClicked();
    void tbTanRadiusClicked();
    void tbTanAngleClicked();
    void tbArcAngleClicked();
protected:
    RS_ActionDrawPolyline* m_action = nullptr;
    void doSaveSettings() override;
    void doSetAction(RS_ActionInterface *a, bool update) override;
    bool checkActionRttiValid(RS2::ActionType actionType) override;
private:
	std::unique_ptr<Ui::Ui_PolylineOptions> ui;
    void setReversedToActionAndView(bool reversed);
    void setAngleToActionAndView(const QString &strVal);
    void setRadiusToActionAndView(const QString &strVal);
};

#endif // QG_POLYLINEOPTIONS_H
