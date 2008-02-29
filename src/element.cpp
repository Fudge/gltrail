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
  rate = 0.0;

  host = h;
  m_name = name;
  std::cout << "[" << host->getDomain().toStdString() << "] ";
  cout << "Element [" << name.toStdString() << "] created." << endl;
}

Element::~Element()
{
}

void Element::add_link_in(Element *e) {
  if( e != NULL ) {
    if(!in.contains( e->name() ) ) {
      in[e->name()] = e;
      std::cout << "[" << host->getDomain().toStdString() << "] ";
      cout << "Rel [" << name().toStdString() << "] <- [" << e->name().toStdString() << "] created." << endl;
    }
    activities << e;
  }
  messages++;
}

void Element::add_link_out(Element *e) {
  if( e != NULL && !out.contains( e->name() ) ) {
    out[e->name()] = e;
    std::cout << "[" << host->getDomain().toStdString() << "] ";
    cout << "Rel [" << name().toStdString() << "] -> [" << e->name().toStdString() << "] created." << endl;
  }

  messages++;
}

void Element::update_stats(void) {

  rate = (rate * 299.0 + messages) / 300.0;
  messages = 0;

  size = rate * 60.0 / 8.0;
  //  size = in.size() * 0.2;

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
  GLfloat r = size / 200;
  GLfloat vy1 = y + r;
  GLfloat vx1 = x;

  bool hover = fabs(gl->getX() - x) <= r*2 && fabs(gl->getY() - y) <= r*2;
  
  if(hover) {
    if( gl->getSelected() == NULL ) 
      gl->setSelected(this);
    glColor4f(1.0, 1.0, 1.0, 1.0);
  } else {
    gl->qglColor( host->getColor() );
  }

  glEnable(GL_LINE_SMOOTH);
  glBegin(GL_LINE_STRIP);

  for(GLfloat angle = 0.0f; angle <= (2.01f*M_PI); angle += 0.15f) {
    glVertex3f(vx1, vy1, 0.0);
    vx1 = x + r * sin(angle);
    vy1 = y + r * cos(angle);
  }

  glEnd();
  glDisable(GL_LINE_SMOOTH);

  if( gl->showLines() || hover ) {
    gl->qglColor( host->getColor().darker(300) );

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

  if( hover ) {
    glColor4f(1.0, 1.0, 1.0, 1.0);
    int xi =  (int) ((1.0 + x) / 2.0 * gl->getWidth() );
    int xy =  (int) (( gl->getAspect() - y) / (2 * gl->getAspect()) * gl->getHeight() - r - 5.0);
    gl->renderText(xi,xy, QString("[%1] %2").arg( QString::number(rate * 60.0).left(4) ).arg(name()) );
  }

  if( activities.size() > 0 && rand() % 30 == 1) {
    Element *e = activities.takeFirst();
    gl->qglColor( host->getColor().lighter(120) );
    glBegin(GL_LINES);
    glVertex3f(x,y,0.0);
    glVertex3f(e->x, e->y, 0);
    glEnd();
  }

}

void Element::repulsive_check(GLWidget *gl, Element *e) {
  bool shown = false;
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

    if( gl->showForces() ) {
      glColor3f(0.2, 0.2, 0.2);
      glBegin(GL_LINES);
      glVertex3f(x,y,0.0);
      glVertex3f(e->x, e->y, 0);
      glEnd();
      shown = true;
    }
  }
  if( d < CUTOFF * e->size) {
    e->repulsive_force(this, d, -dx, -dy);

    if( gl->showForces() && !shown ) {
      glColor3f(0.2, 0.2, 0.2);
      glBegin(GL_LINES);
      glVertex3f(x,y,0.0);
      glVertex3f(e->x, e->y, 0);
      glEnd();
    }
  }
}

void Element::attractive_check(GLWidget *gl, Element *e) {
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
  ed *= log(size) * 0.5 + 1.0;

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
