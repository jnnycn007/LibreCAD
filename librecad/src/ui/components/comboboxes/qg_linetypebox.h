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

#ifndef QG_LINETYPEBOX_H
#define QG_LINETYPEBOX_H

#include <QComboBox>
#include "rs.h"

/**
 * A combobox for choosing a line type.
 */
class QG_LineTypeBox: public QComboBox {
    Q_OBJECT
public:
    QG_LineTypeBox(QWidget* parent=nullptr);
    QG_LineTypeBox(bool showByLayer, bool showUnchanged, 
        QWidget* parent=nullptr, const char* name=nullptr);
    ~QG_LineTypeBox() override;

    RS2::LineType getLineType() const {
        return m_currentLineType;
    }
    void setLineType(RS2::LineType w);
    void setLayerLineType(RS2::LineType w);

    void init(bool showByLayer, bool showUnchanged, bool showNoPen = false);
	
	bool isUnchanged() const {
		return m_unchanged;
	}
private slots:
    void slotLineTypeChanged(int index);
signals:
    void lineTypeChanged(RS2::LineType);
private:
    RS2::LineType m_currentLineType = RS2::SolidLine;
    bool m_showByLayer = false;
    bool m_showUnchanged = false;
    bool m_unchanged = false;
};

#endif
