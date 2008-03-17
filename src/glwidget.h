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
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>

#include "input.h"

class Host;
class Element;

#define NODE_MAP_SIZE           20

#define STAT_FPS                 0
#define STAT_LINES               1
#define STAT_ELEMENTS            2
#define STAT_LINKS               3
#define STAT_LISTS               4
#define STAT_REPULSIVE_CHECKS    5
#define STAT_ATTRACTIVE_CHECKS   6
#define STAT_REPULSIVE_FORCE     7
#define STAT_ATTRACTIVE_FORCE    8
#define STAT_NODE_MAP_UPDATES    9
#define STAT_MAX                10

static QString statNames[] = {
  QString("FPS"),
  QString("Lines"),
  QString("Elements"),
  QString("Links"),
  QString("Lists"),
  QString("Repulsive Checks"),
  QString("Attractive Checks"),
  QString("Repulsive Force"),
  QString("Attractive Force"),
  QString("NodeMap Updates")
};


/**
        @author Erlend Simonsen <mr@fudgie.org>
*/
class GLWidget : public QGLWidget
{
   Q_OBJECT

public:
  GLWidget(QWidget *parent, QList<Input *> *h);
  ~GLWidget();

   QSize minimumSizeHint() const;
   QSize sizeHint() const;

   void addRelation(Input *h, QString &url, QString &ref, bool external = false, QImage *img = NULL);

   int  showLines() { return linesMode; };
   bool showForces() const { return forces; };
   int  showSize() const { return sizeMode; };
   bool showStats() const { return statsMode; };
   bool useRecoil() const { return recoil; };

   void setShowLines(int mode) { linesMode = mode; };
   void setShowSize(int mode) { sizeMode = mode; };

   float getX() const { return x; };
   float getY() const { return y; };

   float getWidth() const { return width; };
   float getHeight() const { return height; };
   float getAspect() const { return aspect; };

   bool dragging() const { return selected != NULL && button == Qt::LeftButton; };
   void setSelected(Element *e) { selected = e; };
   Element *getSelected(void) { return selected; };

   int stats[STAT_MAX];

   GLuint circle;
   QSet<Element *> nodeMap[NODE_MAP_SIZE][NODE_MAP_SIZE];

  float getMaxSize() const { return (maxSize > 1.0 ? maxSize : 1.0); };
  void  setMaxSize(float s) { maxSize = s; };

  float getMaxHits() const { return maxHits; };
  void  setMaxHits(float h) { maxHits = (float) h; };

  void  decayMax(void) {
    maxSize = maxSize * 0.99;
    maxHits = maxHits * 0.99;
  };


  int   stipple_in;
  int   stipple_out;

protected:
   void initializeGL();
   void paintGL();
   void resizeGL(int width, int height);
   void mousePressEvent(QMouseEvent *event);
   void mouseReleaseEvent(QMouseEvent *event);
   void mouseMoveEvent(QMouseEvent *event);
   void keyPressEvent(QKeyEvent *event);

   void timerEvent(QTimerEvent *event);

   float aspect;
   int width;
   int height;

   float x;
   float y;

   int  button;

   int   linesMode;
   bool  forces;
   bool  recoil;
   bool  statsMode;
   int   sizeMode;
   float maxSize;
   float maxHits;

   Element *selected;

   double lastTick;

   QList<Input *> *hosts;


};

#endif
