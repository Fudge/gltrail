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
#include "glwidget.h"
#include "element.h"
#include <QHash>

using namespace std;

#include <iostream>
#include <stdlib.h>
#include <list>
#include "background_reader.h"
#include "host.h"

Elements elements;

int frames = 0;
int last_time = 0;

GLWidget::GLWidget(QWidget *parent)
 : QGLWidget(parent)
{
  startTimer(16);
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true);

  BackgroundReader *bg = new BackgroundReader(&elements);
  bg->start();

  lines = false;
  forces = false;

  aspect = 1.0;
}


GLWidget::~GLWidget()
{
}

QSize GLWidget::minimumSizeHint() const
{
   return QSize(256, 256);
}

QSize GLWidget::sizeHint() const
{
   return QSize(1024,1024);
}

void GLWidget::initializeGL()
{
  QFont font;
  font.setPointSize(8);

  setFont(font);

  qglClearColor( QColor::fromRgbF(0,0,0,0) );
  glShadeModel(GL_FLAT);

  GLfloat light_pos[] = {5.0, 5.0, 0.0, 0.0};
  float light_ambient[] = {0.0, 0.0, 0.0, 1.0};
  float ambient[] = {0.1, 0.1, 0.1, 1.0};

   glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
//      glEnable(GL_LIGHTING);
//      glEnable(GL_LIGHT0);

   glEnable(GL_TEXTURE_2D);

   glDisable(GL_CULL_FACE);
   glDisable(GL_DEPTH_TEST);

   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

   srand(time(NULL));

   //   for( int i = 0; i < NUM; i++ )
   //     elements.push_back( Element() );

//    last_time = glutGet(GLUT_ELAPSED_TIME);

}

void GLWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glPushMatrix();

  glTranslatef(0.0, 0.0, 0.0);
  glColor3f(1.0, 0.4, 0.4);

  for(Elements::iterator iter = elements.begin(); iter != elements.end(); ++iter) {
    Elements::iterator iter2 = iter;
    iter2++;
    while( iter2 != elements.end() ) {
      (*iter)->repulsive_check(this, *iter2);
      ++iter2;
    }

    for( Elements::iterator it = (*iter)->in.begin(); it != (*iter)->in.end(); ++it) {
      (*iter)->attractive_check(this, *it);
    }

    for( Elements::iterator it = (*iter)->out.begin(); it != (*iter)->out.end(); ++it) {
      (*iter)->attractive_check(this, *it);
    }

    (*iter)->update();
    (*iter)->render(this);
  }

  glBegin(GL_POINTS);
  glEnd();

  glPopMatrix();

  frames++;

//   if( glutGet(GLUT_ELAPSED_TIME) - last_time > 1000 ) {
//     cout << "FPS: " << frames << endl;
//     frames = 0;
//     last_time = glutGet(GLUT_ELAPSED_TIME);
//     int num = rand() % elements.size();

//     Element e;

//     list<Element>::iterator it = elements.begin();

//     for( int i = 0; i < num; i++ ) {
//       it++;
//     }
//     e.add_link_in( *it );
//     elements.push_back( e );
//     cout << "Elements[" << elements.size() << "]" << endl;
//   }

  glColor4f(1.0, 1.0, 1.0, 1.0);
  renderText(10, 10, QString("Elements : ").append(QString::number(elements.size()) ) );

}

void GLWidget::timerEvent(QTimerEvent *event) {
  updateGL();

}

void GLWidget::resizeGL(int width, int height) {
  aspect = height / (float) width;

  cout << "reshape[" << width << "x" << height << "] = " << aspect << endl;

  this->width = width;
  this->height = height;

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -aspect, aspect, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, 0.0);

}

void GLWidget::keyPressEvent(QKeyEvent *event) {
  if( event->key() == Qt::Key_Escape ) {
    exit(1);
  } else if( event->key() == Qt::Key_Space ) {
    lines = !lines;
  } else if( event->key() == Qt::Key_V ) {
    forces = !forces;
    cout << "Forces " << forces << endl;
  } else {
    event->ignore();
  }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{

}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
  x = 2.0 * event->x() / width - 1.0;
  y = aspect - (2.0 * aspect) * event->y() / (float) height;

  //  cout << "x: " << x << ", y: " << y << endl;
}

void GLWidget::addRelation(Host *h, QString &url, QString &ref) {

  if( elements.contains(h->getDomain() + url) == false ) {
    elements[h->getDomain() + url] = new Element(h, url);
  }

  if( ref != "-" && !ref.isEmpty() ) {
    if( elements.contains(h->getDomain() + ref) == false ) 
      elements[h->getDomain() + ref] = new Element(h, ref);

    elements[h->getDomain() + url]->add_link_in( elements[h->getDomain() + ref] );
    elements[h->getDomain() +ref]->add_link_out( elements[h->getDomain() + url] );
  } else if( !url.isEmpty() ) {
      elements[h->getDomain() + url]->add_link_in( NULL );
  }

}

