/***************************************************************************
 *   Copyright (C) 2008 by Erlend Simonsen   *
 *   mr@fudgie.org   *
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
#include "element.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

Element::Element(Host *h, QString name)
{
        x = 1.0 - rand() % 1000 / 500.0;
        y = 1.0 - rand() % 1000 / 500.0;

        vx = 0.0;
        vy = 0.0;

        ax = 0.0000;
        ay = 0.0000;

        size = (rand() % 5000) / 1000.0 + 1.0;
        size = 1.0;

        messages = 0;
        rate = 1.0 / 60.0;

	host = h;
        m_name = name;
        cout << "Element [" << name.toStdString() << "] created." << endl;
}

Element::~Element()
{
}

void Element::add_link_in(Element *e) {
  if( e != NULL && !in.contains( e->name() ) ) {
    in[e->name()] = e;
    cout << "Rel [" << name().toStdString() << "] <- [" << e->name().toStdString() << "] created." << endl;
  }

  messages++;
}

void Element::add_link_out(Element *e) {
  if( e != NULL && !out.contains( e->name() ) ) {
    out[e->name()] = e;
    cout << "Rel [" << name().toStdString() << "] -> [" << e->name().toStdString() << "] created." << endl;
  }

  messages++;
}

void Element::update_stats(void) {

  rate = (rate * 299.0 + messages) / 300.0;
  messages = 0;

  size = rate * 60.0 / 8.0;
  //  size = in.size() * 0.2;

  if( m_name == "/tasks/update_sheet_info" ) {
    cout << "Size[" << size << "], Rate[" << rate << "]" << endl;
  }

  if( size > 5.0 )
    size = 5.0;
  if( size < 1.0 )
    size = 1.0;
}

void Element::update(void) {

  vx += ax / SMOOTHING;
  vy += ay / SMOOTHING;

  vx *= DAMPENING;
  vy *= DAMPENING;

  float size2 = size * size;

  x += vx / (size2);
  y += vy / (size2);

  ax = 0.0;
  ay = 0.0;

  if( x > 1.0 ) {
    x = 1.0;
    vx = -vx;
    ax = -ax;
  } else if( x < -1.0 ) {
    x = -1.0;
    vx = -vx;
    ax = -ax;
  }

  if( y > 1.0 ) {
    y = 1.0;
    vy = -vy;
    ay = -ay;
  } else if( y < -1.0 ) {
    y = -1.0;
    vy = -vy;
    ay = -ay;
  }
}

void Element::render(GLWidget *gl) {
  GLfloat r = size / 250;
  GLfloat vy1 = y + r;
  GLfloat vx1 = x;

  gl->qglColor( host->getColor() );
  glBegin(GL_LINE_STRIP);

  for(GLfloat angle = 0.0f; angle <= (2.0f*3.14159); angle += 0.01f) {
    glVertex3f(vx1, vy1, 0.0);
    vx1 = x + r * sin(angle);
    vy1 = y + r * cos(angle);
  }

  glEnd();

  gl->qglColor( host->getColor().dark(300) );

  for(Elements::iterator it = in.begin(); it != in.end(); ++it) {
    glBegin(GL_LINES);
    glVertex3f(x,y,0.0);
    glVertex3f((*it)->x, (*it)->y, 0);
    glEnd();
  }

  for(Elements::iterator it = out.begin(); it != out.end(); ++it) {
    glBegin(GL_LINES);
    glVertex3f(x,y,0.0);
    glVertex3f((*it)->x, (*it)->y, 0);
    glEnd();
  }

}

void Element::repulsive_check(Element *e) {
//      cout << "Check: " << x << ":" << y << " vs " << e->x << ":" << e->y << endl;
  float dx = (e->x - x) * 5;
  float dy = (e->y - y) * 5;

  float d2 = dx * dx + dy * dy;

  if(d2 < 0.001) {
    dx = rand() % 500 / 1000.0 * 0.05;
    dy = rand() % 500 / 1000.0 * 0.05;
    d2 = dx * dx + dy * dy;
  }

  if(d2 < 0.001) {
    d2 = 0.001;
  }

  double d = sqrt(d2);

//      cout << "Distance[" << d << "]" << endl;

  if( d < CUTOFF * size) {
    repulsive_force(e, d, dx, dy);
  }
  if( d < CUTOFF * e->size ) {
    e->repulsive_force(this, d, -dx, -dy);
  }
}

void Element::attractive_check(Element *e) {
  float dx = (e->x - x);
  float dy = (e->y - y);

  float d2 = dx * dx + dy * dy;

  if(d2 < 0.001) {
    dx = rand() % 500 / 1000.0 * 0.05;
    dy = rand() % 500 / 1000.0 * 0.05;
    d2 = dx * dx + dy * dy;
  }

  if(d2 < 0.001) {
    d2 = 0.001;
  }

  double d = sqrt(d2);

  if( d > CUTOFF/4  ) {
    attractive_force(e, d, dx, dy);
    //    e->attractive_force(this, d, -dx, -dy);
  }
}

void Element::repulsive_force(Element *e, double d, float dx, float dy) {
  float ed = (d - CUTOFF * size);
  ed *= log(size) * 0.05 + 1.0;

  float fx = ed / d * dx;
  float fy = ed / d * dy;

  e->ax -= fx;
  e->ay -= fy;

  ax += fx;
  ay += fy;

}

void Element::attractive_force(Element *e, double d, float dx, float dy) {
  float ed = d - ((CUTOFF*size)/4);
  ed *= log(size) * 0.5 + 1.0;

  float fx = dx / d;
  float fy = dy / d;

  fx *= K * ed;
  fy *= K * ed;

  e->ax -= fx;
  e->ay -= fy;

  ax += fx;
  ay += fy;
}
