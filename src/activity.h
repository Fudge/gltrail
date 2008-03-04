/***************************************************************************
 *   Copyright (C) 2008 by Erlend Simonsen                                 *
 *   mr@fudgie.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <list>
#include "glwidget.h"
#include "activity.h"

class Element;

/**
        @author Erlend Simonsen <mr@fudgie.org>
*/
class Activity{

public:
  Activity() {};
  Activity(Element *src, Element *target);

  virtual ~Activity();

  bool render(GLWidget *gl);

  float x; // X Pos
  float y; // Y Pos

  float vx; // X Velocity
  float vy; // Y Velocity

  float ax; // X Acceleration
  float ay; // Y Acceleration

  QColor color;
  Element *target;

};


#endif
