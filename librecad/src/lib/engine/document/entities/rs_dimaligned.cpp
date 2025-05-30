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

#include <iostream>

#include "rs_dimaligned.h"

#include "rs_constructionline.h"
#include "rs_debug.h"
#include "rs_graphic.h"
#include "rs_line.h"
#include "rs_math.h"
#include "rs_settings.h"
#include "rs_units.h"


/**
 * Constructor with initialisation.
 *
	* @para extensionPoint1 Definition point. Startpoint of the
 *         first extension line.
	* @para extensionPoint2 Definition point. Startpoint of the
 *         second extension line.
 */
RS_DimAlignedData::RS_DimAlignedData(const RS_Vector& _extensionPoint1,
                                     const RS_Vector& _extensionPoint2):
    extensionPoint1(_extensionPoint1)
    ,extensionPoint2(_extensionPoint2){
}

std::ostream& operator << (std::ostream& os,const RS_DimAlignedData& dd) {
    os << "(" << dd.extensionPoint1 << "/" << dd.extensionPoint1 << ")";
    return os;
}

/**
 * Constructor.
 *
 * @para parent Parent Entity Container.
 * @para d Common dimension geometrical data.
 * @para ed Extended geometrical data for aligned dimension.
 */
RS_DimAligned::RS_DimAligned(RS_EntityContainer* parent,
                             const RS_DimensionData& d,
                             const RS_DimAlignedData& ed)
    : RS_Dimension(parent, d), m_dimAlignedData(ed) {
    updateDimensions();
    RS_DimAligned::updateDim();
    calculateBorders();
}

RS_Entity* RS_DimAligned::clone() const{
    auto d = new RS_DimAligned(getParent(), getData(), getEData());
	d->setOwner(isOwner());
	return d;
}

RS_VectorSolutions RS_DimAligned::getRefPoints() const {
    return RS_VectorSolutions({
        m_dimAlignedData.extensionPoint1, m_dimAlignedData.extensionPoint2,
        m_dimGenericData.definitionPoint, m_dimGenericData.middleOfText
    });
}

/**
 * @return Automatically created label for the default
 * measurement of this dimension.
 */
QString RS_DimAligned::getMeasuredLabel() {
	double distance = m_dimAlignedData.extensionPoint1.distanceTo(m_dimAlignedData.extensionPoint2);

    double dist = prepareLabelLinearDistance(distance);
    QString distanceLabel =  createLinearMeasuredLabel(dist);
    return distanceLabel;
}

RS_DimAlignedData const& RS_DimAligned::getEData() const {
    return m_dimAlignedData;
}

RS_Vector const& RS_DimAligned::getExtensionPoint1() const {
    return m_dimAlignedData.extensionPoint1;
}

RS_Vector const& RS_DimAligned::getExtensionPoint2() const {
    return m_dimAlignedData.extensionPoint2;
}

/**
 * Updates the sub entities of this dimension. Called when the
 * text or the position, alignment, .. changes.
 *
 * @param autoText Automatically reposition the text label
 */
void RS_DimAligned::doUpdateDim() {
    RS_DEBUG->print("RS_DimAligned::update");

    double dimscale = getGeneralScale();
    double dimexo = getExtensionLineOffset()*dimscale;
    double dimexe = getExtensionLineExtension()*dimscale;
    // text height (DIMTXT)
    //double dimtxt = getTextHeight();
    // text distance to line (DIMGAP)
    //double dimgap = getDimensionLineGap();

    // Angle from extension endpoints towards dimension line
    double extAngle = m_dimAlignedData.extensionPoint2.angleTo(m_dimGenericData.definitionPoint);
    // extension lines length
    double extLength = m_dimAlignedData.extensionPoint2.distanceTo(m_dimGenericData.definitionPoint);

    if (getFixedLengthOn()){
        double dimfxl = getFixedLength()*dimscale;
        if (extLength-dimexo > dimfxl) {
            dimexo =  extLength - dimfxl;
        }
    }

    RS_Vector extLineOffsetVector = RS_Vector::polar(dimexo, extAngle);
    RS_Vector extLineExtensionVector = RS_Vector::polar(dimexe, extAngle);
    RS_Vector extLineDirectionVector = RS_Vector::polar(1.0, extAngle);

    auto extensionLineLengthVector = extLineDirectionVector*extLength;
    auto dimLineStart = m_dimAlignedData.extensionPoint1 + extensionLineLengthVector;
    auto dimLineEnd = m_dimAlignedData.extensionPoint2 + extensionLineLengthVector;

    // Extension lines
    addDimExtensionLine(m_dimAlignedData.extensionPoint1 + extLineOffsetVector, dimLineStart + extLineExtensionVector);
    addDimExtensionLine(m_dimAlignedData.extensionPoint2 + extLineOffsetVector, dimLineEnd + extLineExtensionVector);

    // Dimension line:
    createDimensionLine(dimLineStart,dimLineEnd,true, true, m_dimGenericData.autoText);
}

void RS_DimAligned::getDimPoints(RS_Vector& dimP1, RS_Vector& dimP2){
    double extAngle = m_dimAlignedData.extensionPoint2.angleTo(m_dimGenericData.definitionPoint);
    RS_Vector e1 = RS_Vector::polar(1.0, extAngle);
    double extLength = m_dimAlignedData.extensionPoint2.distanceTo(m_dimGenericData.definitionPoint);
    dimP1 = m_dimAlignedData.extensionPoint1 + e1*extLength;
    dimP2 = m_dimAlignedData.extensionPoint2 + e1*extLength;
}


double RS_DimAligned::getDistanceToPoint(const RS_Vector& coord,
                          RS_Entity** entity,
                          RS2::ResolveLevel level,
                          double solidDist) const
{
    return RS_EntityContainer::getDistanceToPoint(
        coord,
        entity,
        level,
        solidDist);
}

void RS_DimAligned::updateDimPoint(){
    // temporary construction line
    RS_ConstructionLine tmpLine( nullptr,
                                 RS_ConstructionLineData(m_dimAlignedData.extensionPoint1, m_dimAlignedData.extensionPoint2));

    RS_Vector tmpP1 = tmpLine.getNearestPointOnEntity(m_dimGenericData.definitionPoint);
    m_dimGenericData.definitionPoint += m_dimAlignedData.extensionPoint2 - tmpP1;
}

bool RS_DimAligned::hasEndpointsWithinWindow(const RS_Vector& v1, const RS_Vector& v2) const{
    return (m_dimAlignedData.extensionPoint1.isInWindow(v1, v2) ||
            m_dimAlignedData.extensionPoint2.isInWindow(v1, v2));
}

void RS_DimAligned::move(const RS_Vector& offset) {
    RS_Dimension::move(offset);

    m_dimAlignedData.extensionPoint1.move(offset);
    m_dimAlignedData.extensionPoint2.move(offset);
    update();
}

void RS_DimAligned::rotate(const RS_Vector& center, double angle) {
   rotate(center,RS_Vector(angle));
}

void RS_DimAligned::rotate(const RS_Vector& center, const RS_Vector& angleVector) {
    RS_Dimension::rotate(center, angleVector);
    m_dimAlignedData.extensionPoint1.rotate(center, angleVector);
    m_dimAlignedData.extensionPoint2.rotate(center, angleVector);
    update();
}

void RS_DimAligned::scale(const RS_Vector& center, const RS_Vector& factor) {
    RS_Dimension::scale(center, factor);

    m_dimAlignedData.extensionPoint1.scale(center, factor);
    m_dimAlignedData.extensionPoint2.scale(center, factor);
    update();
}

void RS_DimAligned::mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2) {
    RS_Dimension::mirror(axisPoint1, axisPoint2);

    m_dimAlignedData.extensionPoint1.mirror(axisPoint1, axisPoint2);
    m_dimAlignedData.extensionPoint2.mirror(axisPoint1, axisPoint2);
    update();
}

void RS_DimAligned::stretch(const RS_Vector& firstCorner,
                            const RS_Vector& secondCorner,
                            const RS_Vector& offset) {

    //e->calculateBorders();
    if (getMin().isInWindow(firstCorner, secondCorner) &&
        getMax().isInWindow(firstCorner, secondCorner)) {

        move(offset);
    }
    else {
        //RS_Vector v = data.definitionPoint - edata.extensionPoint2;
        double len = m_dimAlignedData.extensionPoint2.distanceTo(m_dimGenericData.definitionPoint);
        double ang1 = m_dimAlignedData.extensionPoint1.angleTo(m_dimAlignedData.extensionPoint2)
                      + M_PI_2;

        if (m_dimAlignedData.extensionPoint1.isInWindow(firstCorner,secondCorner)) {
            m_dimAlignedData.extensionPoint1.move(offset);
        }
        if (m_dimAlignedData.extensionPoint2.isInWindow(firstCorner,secondCorner)) {
            m_dimAlignedData.extensionPoint2.move(offset);
        }

        double ang2 = m_dimAlignedData.extensionPoint1.angleTo(m_dimAlignedData.extensionPoint2)
                      + M_PI_2;

        double diff = RS_Math::getAngleDifference(ang1, ang2);
        if (diff>M_PI) {
            diff-=2*M_PI;
        }

        if (fabs(diff)>M_PI_2) {
            ang2 = RS_Math::correctAngle(ang2+M_PI);
        }

        RS_Vector v = RS_Vector::polar(len, ang2);
        m_dimGenericData.definitionPoint = m_dimAlignedData.extensionPoint2 + v;
    }
    updateDim(true);
}

void RS_DimAligned::moveRef(const RS_Vector& ref, const RS_Vector& offset) {

    if (ref.distanceTo(m_dimGenericData.definitionPoint)<1.0e-4) {
        RS_ConstructionLine l(nullptr,
                              RS_ConstructionLineData(m_dimAlignedData.extensionPoint1,
                                                      m_dimAlignedData.extensionPoint2));
        double d = l.getDistanceToPoint(m_dimGenericData.definitionPoint+offset);
        double a = m_dimAlignedData.extensionPoint2.angleTo(m_dimGenericData.definitionPoint);
        double ad = RS_Math::getAngleDifference(a,
                                                m_dimAlignedData.extensionPoint2.angleTo(m_dimGenericData.definitionPoint+offset));

        if (fabs(ad)>M_PI_2 && fabs(ad)<3.0/2.0*M_PI) {
            a = RS_Math::correctAngle(a+M_PI);
        }

        RS_Vector v = RS_Vector::polar(d, a);
        m_dimGenericData.definitionPoint = m_dimAlignedData.extensionPoint2 + v;
        updateDim(true);
    }
    else if (ref.distanceTo(m_dimGenericData.middleOfText)<1.0e-4) {
        m_dimGenericData.middleOfText.move(offset);
        updateDim(false);
    }
    else if (ref.distanceTo(m_dimAlignedData.extensionPoint1)<1.0e-4) {
        double a1 = m_dimAlignedData.extensionPoint2.angleTo(m_dimAlignedData.extensionPoint1);
        double a2 = m_dimAlignedData.extensionPoint2.angleTo(m_dimAlignedData.extensionPoint1+offset);
        double d1 = m_dimAlignedData.extensionPoint2.distanceTo(m_dimAlignedData.extensionPoint1);
        double d2 = m_dimAlignedData.extensionPoint2.distanceTo(m_dimAlignedData.extensionPoint1+offset);
        rotate(m_dimAlignedData.extensionPoint2, a2-a1);
        if (fabs(d1)>1.0e-4) {
            scale(m_dimAlignedData.extensionPoint2, RS_Vector(d2/d1, d2/d1));
        }
        updateDim(true);
    }
    else if (ref.distanceTo(m_dimAlignedData.extensionPoint2)<1.0e-4) {
        double a1 = m_dimAlignedData.extensionPoint1.angleTo(m_dimAlignedData.extensionPoint2);
        double a2 = m_dimAlignedData.extensionPoint1.angleTo(m_dimAlignedData.extensionPoint2+offset);
        double d1 = m_dimAlignedData.extensionPoint1.distanceTo(m_dimAlignedData.extensionPoint2);
        double d2 = m_dimAlignedData.extensionPoint1.distanceTo(m_dimAlignedData.extensionPoint2+offset);
        rotate(m_dimAlignedData.extensionPoint1, a2-a1);
        if (fabs(d1)>1.0e-4) {
            scale(m_dimAlignedData.extensionPoint1, RS_Vector(d2/d1, d2/d1));
        }
        updateDim(true);
    }
}

/**
 * Dumps the point's data to stdout.
 */
std::ostream& operator << (std::ostream& os, const RS_DimAligned& d) {
    os << " DimAligned: " << d.getData() << "\n" << d.getEData() << "\n";
    return os;
}
