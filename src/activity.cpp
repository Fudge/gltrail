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
  source = s;
  color = s->color.lighter(140);

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

  float d = sqrt(dx * dx + dy * dy);

  ax = ax * 0.3 + dx / d / SMOOTHING;
  ay = ay * 0.3 + dy / d / SMOOTHING;

  vx = vx + ax;
  vy = vy + ay;

  vx *= 0.9;
  vy *= 0.9;

  x = x + vx;
  y = y + vy;

  gl->qglColor( color );
  glVertex3f(x,y,0.0);

  return ( fabs(dx) < 0.004 && fabs(dy) < 0.004 );
}

void Activity::fire() {
  float dx = target->x - source->x;
  float dy = target->y - source->y;

  float d = sqrt(dx * dx + dy * dy);

  source->vx -= (dx / d / DAMPENING) * 0.001;
  source->vy -= (dy / d / DAMPENING) * 0.001;

}

void Activity::impact() {
  target->vx += vx * 0.5;
  target->vy += vy * 0.5;

}
