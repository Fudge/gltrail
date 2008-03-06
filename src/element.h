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
#ifndef ELEMENT_H
#define ELEMENT_H

#include <list>
#include <QWidget>
#include <QtOpenGL>
#include "host.h"
#include "glwidget.h"
#include "activity.h"
#include "relation.h"

#define CUTOFF 0.25
#define K 1.75
#define SMOOTHING 2000
#define DAMPENING 0.96

/**
        @author Erlend Simonsen <mr@fudgie.org>
*/
class Element{

public:
  Element() {};
  Element(Host *h, QString name, QColor col);

  virtual ~Element();

  void render(GLWidget *gl);
  void renderRelations(GLWidget *gl);

  void update(GLWidget *gl);
  void update_stats(void);
  bool contains(GLWidget *gl, Element *e);
  void repulsive_check(GLWidget *gl, Element *e);
  void attractive_check(GLWidget *gl, Element *e);
  void repulsive_force(Element *e, double d, float dx, float dy);
  void attractive_force(Element *e, double d, float dx, float dy);

  bool expired( void ) { return (totalMessages > 0 && rate < 0.001); };

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
  float wantedSize;
  float radius;
  float realSize;

  float minX;
  float maxX;
  float minY;
  float maxY;

  float lastSize;

  int   showInfo;

  int   messages;
  int   totalMessages;
  float rate;

  Host *host;
  QString m_name;
  QColor color;

  QHash<QString,Element *> in;
  QHash<QString,Element *> out;

  QLinkedList<Relation *> relations_in;
  QLinkedList<Relation *> relations_out;

  QLinkedList<Activity *> activities;

};

typedef QHash<QString,Element *> Elements;
typedef QLinkedList<Element *> Nodes;
typedef QLinkedList<Activity *> Activities;
typedef QLinkedList<Relation *> Relations;

inline bool operator==(const Element &e, const Element &f) {
  return f.name() == e.name() && e.host->getDomain() == f.host->getDomain();
}

inline uint qHash(const Element &e) {
  return qHash(e.host->getDomain() + e.name());
}


#endif
