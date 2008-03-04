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
#include "activity.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "element.h"

Activity::Activity(Element *s, Element *t)
{

  target = t;
  color = s->color.lighter(120);

  x = s->x;
  y = s->y;

  vx = 0.0;
  vy = 0.0;

  ax = 0.0000;
  ay = 0.0000;

}

Activity::~Activity()
{
}

bool Activity::render(GLWidget *gl) {
  float dx = target->x - x;
  float dy = target->y - y;
  x = x + dx / 20.0;
  y = y + dy / 20.0;

  gl->qglColor( color );
  glVertex3f(x,y,0.0);

  return ( fabs(dx) < 0.004 && fabs(dy) < 0.004 );
}
