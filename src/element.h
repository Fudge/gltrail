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
#ifndef ELEMENT_H
#define ELEMENT_H

#include <list>
#include <QWidget>
#include <QtOpenGL>
#include "host.h"
#include "glwidget.h"

#define CUTOFF 0.25
#define K 1.5
#define SMOOTHING 2000
#define DAMPENING 0.95

/**
        @author Erlend Simonsen <mr@fudgie.org>
*/
class Element{

public:
  Element() {};
  Element(Host *h, QString name);

  virtual ~Element();

  void render(GLWidget *gl);
  void update(void);
  void update_stats(void);
  void repulsive_check(Element *e);
  void attractive_check(Element *e);
  void repulsive_force(Element *e, double d, float dx, float dy);
  void attractive_force(Element *e, double d, float dx, float dy);

  void add_link_in(Element *e);
  void add_link_out(Element *e);

  inline QString name(void) const { return m_name; };

  float x; // X Pos
  float y; // Y Pos

  float vx; // X Velocity
  float vy; // Y Velocity

  float ax; // X Acceleration
  float ay; // Y Acceleration
  float size;

  int   messages;
  float rate;

  Host *host;
  QString m_name;

  QHash<QString,Element *> in;
  QHash<QString,Element *> out;
};

typedef QHash<QString,Element *> Elements;

inline bool operator==(const Element &e, const Element &f) {
  return f.name() == e.name();
}

inline uint qHash(const Element &e) {
  return qHash(e.name());
}


#endif
