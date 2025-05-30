/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2024 LibreCAD.org
** Copyright (C) 2024 Dongxu Li (dongxuli2011@gmail.com)
** Copyright (C) 2014 Pavel Krejcir (pavel@pamsoft.cz)

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

#ifndef LC_DlgParabola_H
#define LC_DlgParabola_H

#include "lc_entitypropertiesdlg.h"

class LC_Parabola;

namespace Ui
{
    class DlgParabola;
}

class LC_DlgParabola : public LC_EntityPropertiesDlg {
    Q_OBJECT
public:
    LC_DlgParabola(QWidget* parent, LC_GraphicViewport* pViewport, LC_Parabola* parabola);
    ~LC_DlgParabola() override;
public slots:
    void updatePoints();
    void updateEntity() override;
protected slots:
    void languageChange();
protected:
    void setEntity(LC_Parabola* b);
    LC_DlgParabola(LC_DlgParabola const&) = delete;
    LC_DlgParabola& operator =(LC_DlgParabola const&) = delete;
    LC_DlgParabola(LC_DlgParabola&&) = delete;
    LC_DlgParabola& operator =(LC_DlgParabola&&) = delete;
    LC_Parabola* m_entity = nullptr;
    std::unique_ptr<Ui::DlgParabola> ui;
};

#endif // LC_DlgParabola_H
