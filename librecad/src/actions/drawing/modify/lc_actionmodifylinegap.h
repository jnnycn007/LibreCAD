/****************************************************************************
**
* Action that creates a gap in selected line and so creates two line segments

Copyright (C) 2024 LibreCAD.org
Copyright (C) 2024 sand1024

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**********************************************************************/

#ifndef LC_ACTIONMODIFYLINEGAP_H
#define LC_ACTIONMODIFYLINEGAP_H

#include "lc_abstractactionwithpreview.h"

class LC_ActionModifyLineGap:public LC_AbstractActionWithPreview{
    Q_OBJECT

    /**
     * entity states
     */
    enum{
        SetEntity,
        SetGapEndPoint
    };

public:
    LC_ActionModifyLineGap(LC_ActionContext *actionContext);

    double getGapSize()const{return m_gapSize;};
    void setGapSize(double value){m_gapSize =value;};

    int getLineSnapMode() const{return m_lineSnapMode;};
    void setLineSnapMode(int value){m_lineSnapMode = value;};

    double getSnapDistance() const{return m_snapDistance;};
    void setSnapDistance(double value){m_snapDistance = value;};

    bool isFreeGapSize() const{return m_freeGapSize;};
    void setFreeGapSize(bool value){m_freeGapSize = value;}

    bool getGapSnapMode() const {return m_gapSnapMode;};
    void setGapSnapMode(int mode){m_gapSnapMode = mode;};
protected:

    /**
    * Snapping for gap - if gap's size is fixed (not free)
    */
    enum{
        GAP_SNAP_START, //start point of gap in line snap point
        GAP_SNAP_MIDDLE, // middle point of gap in line snap point
        GAP_SNAP_END // end point of gap is in line snap point
    };

    /**
     * length of gap for fixed size mode
     */
    double m_gapSize = 0.0;

    /**
     * snap mode that controls in which part of line the gap will be placed.
     * In alternative action mode (SHIFT pressed with mouse), snap mode for line
     * and gap is mirrored (so they are applied to another end of line)
     */
    int m_lineSnapMode = LINE_SNAP_START;

    /**
     * distance of gap snap point from line snap point (if not free gap mode)
     */
    double m_snapDistance = 0.0;

    /**
     * if true - gap size is selected by the user, if false - it is defined by the option
     */
    bool m_freeGapSize = false;

    /**
     * controls how the gap is positioned to gap snap point
     */
    int m_gapSnapMode = GAP_SNAP_START;

    struct GapData{
        RS_Line* originalLine;
        RS_Vector startPoint;
        RS_Vector endPoint;
    };

    /**
     * information of gap needed for trigger
     */
    GapData* m_gapData = nullptr;

    LC_ActionOptionsWidget* createOptionsWidget() override;
    void doPreparePreviewEntities(LC_MouseEvent *e, RS_Vector &snap, QList<RS_Entity *> &list, int status) override;
    RS_Vector obtainLineSnapPointForMode(const RS_Line *targetLine, const RS_Vector &snap) const;
    GapData *prepareGapData(RS_Line *line, const RS_Vector &snap, const RS_Vector &startPoint) const;
    void performTriggerDeletions() override;
    void doOnLeftMouseButtonRelease(LC_MouseEvent *e, int status, const RS_Vector &snapPoint) override;
    bool doCheckMayTrigger() override;
    void doAfterTrigger() override;
    bool isSetActivePenAndLayerOnTrigger() override;
    void doPrepareTriggerEntities(QList<RS_Entity *> &list) override;
    void doFinish(bool updateTB) override;
    void createPreviewEntities(GapData *data, QList<RS_Entity *> &list, bool startPointNoSelected) const;
    void updateMouseButtonHints() override;

    RS2::CursorType doGetMouseCursor(int status) override;
};

#endif // LC_ACTIONMODIFYLINEGAP_H
