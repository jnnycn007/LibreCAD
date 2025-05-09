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


#ifndef RS_CREATION_H
#define RS_CREATION_H

#include <QString>
#include <memory>

#include "rs_preview.h"
#include "rs_vector.h"

class RS_Document;
class RS_EntityContainer;
class RS_GraphicView;
class RS_Graphic;
class RS_Entity;
class RS_Arc;
class RS_Circle;
class RS_Ellipse;
class RS_Line;
class LC_SplinePoints;
struct RS_BlockData;
struct RS_ImageData;
class RS_Image;
struct RS_InsertData;
class RS_Insert;
class RS_Block;
class QString;
class LC_GraphicViewport;

/**
 * Data needed to insert library items.
 */
struct RS_LibraryInsertData {
    QString file;
    RS_Vector insertionPoint;
    double factor = 0.;
    double angle = 0.;
    RS_Graphic * graphic;
};


/**
 * Class for the creation of new entities.
 * This class is bound to an entity container in which the
 * entities are stored. 
 *
 * @author Andrew Mustun
 */
class RS_Creation {
public:
    RS_Creation(RS_EntityContainer* container,
                LC_GraphicViewport* viewport = nullptr,
                bool handleUndo=true);

    ~RS_Creation()=default;

    RS_Entity* createParallelThrough(const RS_Vector& coord,
                                     int number,
                                     RS_Entity* e,
                                     bool symmetric);

    RS_Entity* createParallel(const RS_Vector& coord,
                              double distance,
                              int number,
                              RS_Entity* e, bool symmetric = false);

    RS_Line* createParallelLine(const RS_Vector& coord,
                                double distance, int number,
                                RS_Line* e, bool symmetric = false);

    RS_Arc* createParallelArc(const RS_Vector& coord,
                              double distance, int number,
                              RS_Arc* e);

    RS_Circle* createParallelCircle(const RS_Vector& coord,
                                    double distance, int number,
                                    RS_Circle* e);

    LC_SplinePoints* createParallelSplinePoints(const RS_Vector& coord,
                                                double distance, int number,
                                                LC_SplinePoints* e);

    RS_Line* createBisector(const RS_Vector& coord1,
                            const RS_Vector& coord2,
                            double length,
                            int num,
                            RS_Line* l1,
                            RS_Line* l2);

    RS_Line* createTangent1(const RS_Vector& coord,
                            const RS_Vector& point,
                            RS_Entity* circle,
                            RS_Vector& tangentPoint,
                            RS_Vector& altTangentPoint);
/**
 * create a tangent line which is orthogonal to the given RS_Line(normal)
 */
    std::unique_ptr<RS_Line> createLineOrthTan(const RS_Vector& coord,
                               RS_Line* normal,
                               RS_Entity* circle,
                               RS_Vector& alternativeTangent);
    std::vector<std::unique_ptr<RS_Line>> createTangent2(
                            RS_Entity* circle1,
                            RS_Entity* circle2);

    std::unique_ptr<RS_Line> createLineRelAngle(const RS_Vector& coord,
                                RS_Entity* entity,
                                double angle,
                                double length);

    RS_Line* createPolygon(const RS_Vector& center,
                           const RS_Vector& corner,
                           int number);

    RS_Line* createPolygon2(const RS_Vector& corner1,
                            const RS_Vector& corner2,
                            int number);

    RS_Line* createPolygon3(const RS_Vector& center,
                            const RS_Vector& tangent,
                            int number);

    RS_Insert* createInsert(const RS_InsertData* pdata);

    RS_Image* createImage(const RS_ImageData* pdata);

    RS_Block* createBlock(const RS_BlockData* data,
                          const RS_Vector& referencePoint,
                          const bool remove);

    RS_Insert* createLibraryInsert(RS_LibraryInsertData& data);

private:
    RS_EntityContainer* m_container{nullptr};
    RS_Graphic* m_graphic{nullptr};
    RS_Document* m_document{nullptr};
    LC_GraphicViewport*  m_viewport{nullptr};
    bool handleUndo = false;
    /**
    * @brief setupAndAddEntity - returns true, if ownership is taken by undo cycles
    * @param en
    * @return
    */
    bool setupAndAddEntity(RS_Entity* en) const;
};

#endif
