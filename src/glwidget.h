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

class Host;
class Element;

/**
        @author Erlend Simonsen <mr@fudgie.org>
*/
class GLWidget : public QGLWidget
{
   Q_OBJECT

public:
  GLWidget(QWidget *parent, QList<Host *> *h);
  ~GLWidget();

   QSize minimumSizeHint() const;
   QSize sizeHint() const;

   void addRelation(Host *h, QString &url, QString &ref, bool external = false);

   bool showLines() const { return lines; };
   bool showForces() const { return forces; };
   int  showSize() const { return sizeMode; };

   float getX() const { return x; };
   float getY() const { return y; };

   float getWidth() const { return width; };
   float getHeight() const { return height; };
   float getAspect() const { return aspect; };

   bool dragging() const { return selected != NULL && button == Qt::LeftButton; };
   void setSelected(Element *e) { selected = e; };
   Element *getSelected(void) { return selected; };

   QMap<QString,int> stats;

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

   bool lines;
   bool forces;
   int  sizeMode;

   Element *selected;

   double lastTick;

};

#endif
