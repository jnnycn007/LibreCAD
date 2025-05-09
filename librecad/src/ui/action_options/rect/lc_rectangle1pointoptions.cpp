/****************************************************************************
**
* Options widget for "Rectangle1Point" action.

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
#include "lc_actiondrawrectangle1point.h"
#include "lc_rectangle1pointoptions.h"
#include "ui_lc_rectangle1pointoptions.h"

LC_Rectangle1PointOptions::LC_Rectangle1PointOptions() :
    LC_ActionOptionsWidgetBase(RS2::ActionDrawRectangle1Point, "Draw","Rectangle1Point"),
    ui(new Ui::LC_Rectangle1PointOptions),
    m_action(nullptr){
    ui->setupUi(this);

    connect(ui->leWidth, &QLineEdit::editingFinished, this, &LC_Rectangle1PointOptions::onWidthEditingFinished);
    connect(ui->leHeight, &QLineEdit::editingFinished, this, &LC_Rectangle1PointOptions::onHeightEditingFinished);
    connect(ui->leRadius, &QLineEdit::editingFinished, this, &LC_Rectangle1PointOptions::onRadiusEditingFinished);
    connect(ui->leLenY, &QLineEdit::editingFinished, this, &LC_Rectangle1PointOptions::onLenYEditingFinished);
    connect(ui->leX, &QLineEdit::editingFinished, this, &LC_Rectangle1PointOptions::onLenXEditingFinished);

    connect(ui->leAngle, &QLineEdit::editingFinished, this, &LC_Rectangle1PointOptions::onAngleEditingFinished);
    connect(ui->cbCorners, &QComboBox::currentIndexChanged, this, &LC_Rectangle1PointOptions::onCornersIndexChanged);
    connect(ui->cbSnapPoint, &QComboBox::currentIndexChanged, this,&LC_Rectangle1PointOptions::onSnapPointIndexChanged);

    connect(ui->cbPolyline, &QCheckBox::clicked ,this, &LC_Rectangle1PointOptions::onUsePolylineClicked);
    connect(ui->cbSnapRadiusCenter, &QCheckBox::clicked, this, &LC_Rectangle1PointOptions::onSnapToCornerArcCenterClicked);
    connect(ui->cbInnerSize, &QCheckBox::clicked, this, &LC_Rectangle1PointOptions::onInnerSizeClicked);
    connect(ui->cbEdges, &QComboBox::currentIndexChanged, this, &LC_Rectangle1PointOptions::onEdgesIndexChanged);
    connect(ui->chkFixedBaseAngle, &QCheckBox::clicked, this, &LC_Rectangle1PointOptions::onBaseAngleFixedClicked);
    connect(ui->cbFreeAngle, &QCheckBox::clicked, this, &LC_Rectangle1PointOptions::onFreeAngleClicked);
}

LC_Rectangle1PointOptions::~LC_Rectangle1PointOptions(){
    delete ui;
    m_action = nullptr;
}

void LC_Rectangle1PointOptions::doSaveSettings(){
    save("Width", ui->leWidth->text());
    save("Height", ui->leHeight->text());
    save("Angle", ui->leAngle->text());
    save("SnapMode", ui->cbSnapPoint->currentIndex());
    save("Corners", ui->cbCorners->currentIndex());
    save("Radius", ui->leRadius->text());
    save("LengthX", ui->leX->text());
    save("LengthY", ui->leLenY->text());
    save("Polyline", ui->cbPolyline->isChecked());
    save("RadiusSnap", ui->cbSnapRadiusCenter->isChecked());
    save("SizeInner", ui->cbInnerSize->isChecked());
    save("Edges", ui->cbEdges->currentIndex());
    save("BaseAngleIsFixed", ui->chkFixedBaseAngle->isChecked());
    save("BaseAngleIsFree", ui->cbFreeAngle->isChecked());
}

void LC_Rectangle1PointOptions::languageChange(){
    ui->retranslateUi(this);
}

void LC_Rectangle1PointOptions::doSetAction(RS_ActionInterface * a, bool update){
        m_action = dynamic_cast<LC_ActionDrawRectangle1Point *>(a);

        QString width;
        QString height;
        QString angle;
        QString radius;
        QString lenX;
        QString lenY;

        int cornersMode;
        int snapMode;
        bool usePolyline;
        bool snapRadiusCenter;
        bool sizeIsInner;
        int edges;
        bool hasBaseAngle;
        bool baseAngleIsFree;

        if (update){
            cornersMode = m_action->getCornersMode();
            snapMode = m_action->getInsertionPointSnapMode();
            usePolyline = m_action->isUsePolyline();
            edges = m_action->getEdgesDrawMode();

            double w = m_action->getWidth();
            double h = m_action->getHeight();
            double an = m_action->getUcsAngleDegrees();
            double r  = m_action->getRadius();
            double lX = m_action->getLengthX();
            double lY = m_action->getLengthY();

            width = fromDouble(w);
            height = fromDouble(h);
            angle = fromDouble(an);
            radius = fromDouble(r);
            lenX = fromDouble(lX);
            lenY = fromDouble(lY);
            snapRadiusCenter = m_action->isSnapToCornerArcCenter();
            sizeIsInner = m_action->isSizeInner();
            hasBaseAngle = m_action->hasBaseAngle();
            baseAngleIsFree = m_action->isBaseAngleFree();
        }
        else{
            width = load("Width", "10");
            height = load("Height", "10");
            angle = load("Angle", "0");
            snapMode = loadInt("SnapMode", 0);
            cornersMode = loadInt("Corners", 0);
            radius = load("Radius", "0.0");
            lenX = load("LengthX", "5");
            lenY = load("LengthY", "5");
            usePolyline = loadBool("Polyline", true);
            snapRadiusCenter = loadBool("RadiusSnap", true);
            sizeIsInner = loadBool("SizeInner", true);
            edges = loadInt("Edges", 0);
            hasBaseAngle = loadBool("BaseAngleIsFixed", false);
            baseAngleIsFree = loadBool("BaseAngleIsFree", false);
        }

        setWidthToActionAnView(width);
        setHeightToActionAnView(height);
        setAngleToActionAndView(angle);
        setRadiusToActionAnView(radius);
        setLenXToActionAnView(lenX);
        setLenYToActionAnView(lenY);
        setCornersModeToActionAndView(cornersMode);
        setSnapPointModeToActionAndView(snapMode);
        setUsePolylineToActionAndView(usePolyline);
        setSnapToCornerArcCenterToActionAndView(snapRadiusCenter);
        setSizeInnerToActionAndView(sizeIsInner);
        setEdgesModeToActionAndView(edges);
        setFreeAngleToActionAndView(baseAngleIsFree);
        setBaseAngleFixedToActionAndView(hasBaseAngle);
}

void LC_Rectangle1PointOptions::onCornersIndexChanged(int index){
    if (m_action != nullptr){
        setCornersModeToActionAndView(index);
    }
}

void LC_Rectangle1PointOptions::updateUI(int mode) {
     if (mode == UPDATE_ANGLE){
         double angle = m_action->getUcsAngleDegrees();
         ui->leAngle->blockSignals(true);
         ui->leAngle->setText(fromDouble(angle));
         ui->leAngle->blockSignals(false);
     }
}

void LC_Rectangle1PointOptions::setCornersModeToActionAndView(int index){
    m_action->setCornersMode(index);
    bool round = index == LC_AbstractActionDrawRectangle::CORNER_RADIUS;
    bool bevel = index == LC_AbstractActionDrawRectangle::CORNER_BEVEL;

    ui->lblRadius->setVisible(round);
    ui->leRadius->setVisible(round);
    ui->cbSnapRadiusCenter->setVisible(round);
    ui->cbInnerSize->setVisible(round);

    ui->lblLenY->setVisible(bevel);
    ui->lblX->setVisible(bevel);
    ui->leLenY->setVisible(bevel);
    ui->leX->setVisible(bevel);

    ui->cbCorners->setCurrentIndex(index);

    bool straight = index == LC_AbstractActionDrawRectangle::CORNER_STRAIGHT;
    ui->lblEdges->setVisible(straight);
    ui->cbEdges->setVisible(straight);

}

void LC_Rectangle1PointOptions::onLenYEditingFinished(){
     if (m_action != nullptr){
         QString value = ui->leLenY->text();
         setLenYToActionAnView(value);
     }
}

void LC_Rectangle1PointOptions::onLenXEditingFinished(){
    if (m_action != nullptr){
        QString value = ui->leX->text();
        setLenXToActionAnView(value);
    }
}

void LC_Rectangle1PointOptions::onRadiusEditingFinished(){
    if (m_action != nullptr){
        QString value = ui->leRadius->text();
        setRadiusToActionAnView(value);
    }
}

void LC_Rectangle1PointOptions::onHeightEditingFinished(){
    if (m_action != nullptr){
        QString value = ui->leHeight->text();
        setHeightToActionAnView(value);
    }
}

void LC_Rectangle1PointOptions::onWidthEditingFinished(){
    if (m_action != nullptr){
        QString value = ui->leWidth->text();
        setWidthToActionAnView(value);
    }
}

void LC_Rectangle1PointOptions::onSnapPointIndexChanged(int index){
  if (m_action != nullptr){
      setSnapPointModeToActionAndView(index);
  }
}

void LC_Rectangle1PointOptions::setSnapPointModeToActionAndView(int index){
    m_action->setInsertionPointSnapMode(index);
    ui->cbSnapPoint->setCurrentIndex(index);
}

void LC_Rectangle1PointOptions::onAngleEditingFinished(){
    const QString &expr = ui->leAngle->text();
    setAngleToActionAndView(expr);
}

void LC_Rectangle1PointOptions::setAngleToActionAndView(const QString &val){
    double angle;
    if (toDoubleAngleDegrees(val, angle, 0.0, false)){
        m_action->setUcsAngleDegrees(angle);
        ui->leAngle->setText(fromDouble(angle));
    }
}

void LC_Rectangle1PointOptions::setLenYToActionAnView(const QString& value){
    double y;
    if (toDouble(value, y, 1.0, true)){
        m_action->setLengthY(y);
        ui->leLenY->setText(fromDouble(y));
    }
}

void LC_Rectangle1PointOptions::setLenXToActionAnView(const QString& value){
    double y;
    if (toDouble(value, y, 1.0, true)){
        m_action->setLengthX(y);
        ui->leX->setText(fromDouble(y));
    }
}

void LC_Rectangle1PointOptions::setRadiusToActionAnView(const QString& value){
    double y;
    if (toDouble(value, y, 1.0, true)){
        m_action->setRadius(y);
        ui->leRadius->setText(fromDouble(y));
    }
}

void LC_Rectangle1PointOptions::setHeightToActionAnView(const QString& value){
    double y;
    if (toDouble(value, y, 0, true)){
        m_action->setHeight(y);
        ui->leHeight->setText(fromDouble(y));
    }
}

void LC_Rectangle1PointOptions::setWidthToActionAnView(const QString& value){
    double y;
    if (toDouble(value, y, 0, true)){
        m_action->setWidth(y);
        ui->leWidth->setText(fromDouble(y));
    }
}

void LC_Rectangle1PointOptions::onUsePolylineClicked(bool value){
   if (m_action != nullptr){
       setUsePolylineToActionAndView(value);
   }
}

void LC_Rectangle1PointOptions::onSnapToCornerArcCenterClicked(bool value){
    if (m_action != nullptr){
        setSnapToCornerArcCenterToActionAndView(value);
    }
}

void LC_Rectangle1PointOptions::onInnerSizeClicked(bool value){
    if (m_action != nullptr){
        setSizeInnerToActionAndView(value);
    }
}

void LC_Rectangle1PointOptions::onFreeAngleClicked(bool value) {
    if (m_action != nullptr){
        setFreeAngleToActionAndView(value);
    }
}

void LC_Rectangle1PointOptions::onBaseAngleFixedClicked(bool value){
    if (m_action != nullptr){
        setBaseAngleFixedToActionAndView(value);
    }
}

void LC_Rectangle1PointOptions::setBaseAngleFixedToActionAndView(bool value){
    ui->chkFixedBaseAngle->setChecked(value);
    m_action->setBaseAngleFixed(value);
    ui->leAngle->setEnabled(value && !ui->cbFreeAngle->isChecked());
    ui->cbFreeAngle->setEnabled(value);
}

void LC_Rectangle1PointOptions::setUsePolylineToActionAndView(bool value){
   m_action->setUsePolyline(value);
   ui->cbPolyline->setChecked(value);
}

void LC_Rectangle1PointOptions::setSnapToCornerArcCenterToActionAndView(bool value){
   m_action->setSnapToCornerArcCenter(value);
   ui->cbSnapRadiusCenter->setChecked(value);
}

void LC_Rectangle1PointOptions::setSizeInnerToActionAndView(bool value){
   m_action->setSizeInner(value);
   ui->cbInnerSize->setChecked(value);
}

void LC_Rectangle1PointOptions::setFreeAngleToActionAndView(bool value){
   m_action->setBaseAngleFree(value);
   ui->cbFreeAngle->setChecked(value);
   ui->leAngle->setEnabled(!value);
}

void LC_Rectangle1PointOptions::onEdgesIndexChanged(int index){
    if (m_action != nullptr){
        setEdgesModeToActionAndView(index);
    }
}

void LC_Rectangle1PointOptions::setEdgesModeToActionAndView(int index){
    m_action->setEdgesDrawMode(index);
    ui->cbEdges->setCurrentIndex(index);
}
