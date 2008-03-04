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
#include <sys/time.h>
#include <math.h>

Elements elements;
Nodes    nodes;


int frames = 0;
int last_time = 0;

GLWidget::GLWidget(QWidget *parent, Hosts *h)
  : QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::Rgba | QGL::DirectRendering | QGL::AlphaChannel), parent)
{
  //  startTimer(1);
  startTimer(16);
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true);

  BackgroundReader *bg = new BackgroundReader(h, &elements);
  bg->start();

  lines = false;
  forces = false;
  statsMode = false;
  sizeMode = 0;

  aspect = 1.0;

  x = -10.0;
  y = -10.0;
  lastTick = 0;
  maxSize = 0.0;

  stipple_in  = 0x0001;
  stipple_out = 0x8000;

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
   return QSize(512,512);
}

void GLWidget::initializeGL()
{
  QFont font("Terminus");
  font.setPointSize(8);
  font.setStyleHint(QFont::System, QFont::PreferBitmap);
  font.setFixedPitch(true);
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

   glDisable(GL_TEXTURE_2D);

   glDisable(GL_CULL_FACE);
   glDisable(GL_DEPTH_TEST);

   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

   srand(time(NULL));

   circle = glGenLists(1);
   glNewList(circle, GL_COMPILE);

   GLfloat r = 0.414 / 100.0;
   GLfloat vy1 = r;
   GLfloat vx1 = 0.0;

   glBegin(GL_LINE_STRIP);
   for(GLfloat angle = 0.0f; angle <= (2.0f*M_PI); angle += 0.5f) {
     glVertex3f(vx1, vy1, 0.0);
     vx1 =  r * sin(angle);
     vy1 =  r * cos(angle);
   }
   glVertex3f(vx1, vy1, 0.0);
   glVertex3f(0.0, r, 0.0);
   glEnd();
   glEndList();

}

void GLWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glPushMatrix();

  glTranslatef(0.0, 0.0, 0.0);
  glColor3f(1.0, 0.4, 0.4);

  for(int i = 1; i < STAT_MAX; i++ ) {
    stats[i] = 0;
  }

  if( !dragging() ) {
    selected = NULL;
  }

  stipple_in  = stipple_in  << 1;
  stipple_out = stipple_out >> 1;

  if( stipple_in > 0x8000 ) {
    stipple_in = 0x0001;
  }

  if( stipple_out < 0x0001 ) {
    stipple_out = 0x8000;
  }

  Nodes::iterator iter;
  Nodes::iterator it;

  Nodes::iterator iter2;

  for(iter = nodes.begin(); iter != nodes.end(); ++iter) {
    stats[STAT_ELEMENTS] += 1;

    Element *e = (*iter);

//     QSet<Element*> *nodeMapEntries = &nodeMap[e->nodeX()][e->nodeY()];

//     for(QSet<Element *>::iterator iter2 = nodeMapEntries->begin(); iter2 != nodeMapEntries->end(); iter2++ ) {
//       (*iter2)->repulsive_check(this, e);
//       stats[STAT_REPULSIVE_CHECKS] += 1;
//     }

    iter2 = iter;
    iter2++;

    while( iter2 != nodes.end() ) {
      if( e->contains(this,*iter2) || (*iter2)->contains(this,*iter) ) {
        e->repulsive_check(this, *iter2);
        stats[STAT_REPULSIVE_CHECKS] += 1;
      }
      ++iter2;
    }

     for(it = e->nodes_in.begin(); it != e->nodes_in.end(); ++it) {
       e->attractive_check(this, *it);
       stats[STAT_ATTRACTIVE_CHECKS] += 1;
       stats[STAT_LINKS] += 1;
     }

     for(it = e->nodes_out.begin(); it != e->nodes_out.end(); ++it) {
       e->attractive_check(this, *it);
       stats[STAT_ATTRACTIVE_CHECKS] += 1;
     }

     e->update(this);

     if( button == Qt::LeftButton && selected != NULL && selected == (*iter) ) {
       selected->x = x;
       selected->y = y;
       selected->vx = 0;
       selected->vy = 0;
       selected->ax = 0;
       selected->ay = 0;
     }

    e->render(this);
  }

  for(iter = nodes.begin(); iter != nodes.end(); ++iter) {
    Element *e = (*iter);

    if( e->expired() ) {
      cout << "Expired[" << e->name().toStdString() << "][" << e->rate << "]" << endl;

      for(iter2 = nodes.begin(); iter2 != nodes.end(); ++iter2) {
        Element *e2 = (*iter2);

        //      cout << "Checking vs [" << e2->name().toStdString() << "]" << endl;

        if( e2 == e )
          continue;

        for( it = e2->nodes_in.begin(); it != e2->nodes_in.end(); ++it ) {

          //          cout << "Comparing [" << (e->host->getDomain() + e->name()).toStdString() << "] to [" << ((*it)->host->getDomain() + (*it)->name()).toStdString() << "]" << endl;
          if( e == *it ) {
            cout << "removing[" << e2->name().toStdString() << "] nodes_in" << endl;
            it = e2->nodes_in.erase(it);
            e2->in.remove(e2->name());
          }
        }

        for( it = e2->nodes_out.begin(); it != e2->nodes_out.end(); ++it ) {
          if( e == *it ) {
            cout << "removing[" << e2->name().toStdString() << "] nodes_out" << endl;
            it = e2->nodes_out.erase(it);
            e2->out.remove(e2->name());
          }
        }

        for( it = e2->activities.begin(); it != e2->activities.end(); ++it ) {
          if( e == *it ) {
            cout << "removing[" << e2->name().toStdString() << "] activitites" << endl;
            it = e2->activities.erase(it);
          }
        }

      }

      elements.remove( e->host->getDomain() + e->name() );

      iter = nodes.erase(iter);
      delete e;
    }
  }

  glPopMatrix();

  frames++;

  glColor4f(1.0, 1.0, 1.0, 1.0);
  int text_y = 10;
  QString text("%1 %2");

  if( showStats() ) {
    for( int i = 0; i < STAT_MAX; i++ ) {
      renderText(2, text_y, text.arg( QString::number(stats[i]), 5, QChar('0')).arg(statNames[i]));
      text_y += 11;
    }
  } else {
    renderText(2, text_y, text.arg( QString::number(stats[0]), 5, QChar('0')).arg(statNames[0]));
  }

  text = QString("Mode [%1]");
  switch(sizeMode) {
  case 0:
    text = text.arg("Rate");
    break;
  case 1:
    text = text.arg("Links IN");
    break;
  case 2:
    text = text.arg("Links OUT");
    break;
  case 3:
    text = text.arg("Links TOTAL");
    break;
  }

  renderText(2, (int) (height * 0.9), text);

  timeval tim;
  gettimeofday(&tim, NULL);
  double curTick = tim.tv_sec * 1000 + tim.tv_usec / 1000;
  if( curTick - lastTick > 5000 ) {
    stats[STAT_FPS] = (int) (frames / 5.0);
    lastTick = curTick;
    frames = 0;
  }


}

void GLWidget::timerEvent(QTimerEvent *event) {
  makeCurrent();
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
    cout << "Lines " << lines << endl;
  } else if( event->key() == Qt::Key_V ) {
    forces = !forces;
    cout << "Forces " << forces << endl;
  } else if( event->key() == Qt::Key_S ) {
    statsMode = !statsMode;
    cout << "statsMode " << statsMode << endl;
  } else if( event->key() == Qt::Key_B ) {
    sizeMode++;
    if( sizeMode > 3 ) {
      sizeMode = 0;
    }
    cout << "SizeMode " << sizeMode << endl;
  } else {
    event->ignore();
  }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
  button = event->button();
  if( button == Qt::NoButton ) {
    selected = NULL;
  }
  event->accept();
  cout << "Mouse press!" << endl;
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
  button = Qt::NoButton;
  selected = NULL;
  cout << "Mouse release!" << endl;
  event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
  x = 2.0 * event->x() / width - 1.0;
  y = aspect - (2.0 * aspect) * event->y() / (float) height;

  if(x > 1.0) {
    x = 1.0;
  }
  if(x < -1.0)
    x = -1.0;

  if(y > aspect)
    y = aspect;
  if(y < -aspect)
    y = -aspect;

  //  cout << "x: " << x << ", y: " << y << endl;
}

void GLWidget::addRelation(Host *h, QString &url, QString &ref, bool external) {

  if( elements.contains(h->getDomain() + url) == false ) {
    QColor color = h->getColor();
    elements[h->getDomain() + url] = new Element(h, url, color);
    nodes.push_back( elements[h->getDomain() + url] );
  }

  if( ref != "-" && !ref.isEmpty() ) {
    if( elements.contains(h->getDomain() + ref) == false ) {
      QColor color = h->getColor();
      if( external ) {
        color = color.darker(300);
      }

      elements[h->getDomain() + ref] = new Element(h, ref, color);
      nodes.push_back( elements[h->getDomain() + ref] );
    }

    elements[h->getDomain() + url]->add_link_in( elements[h->getDomain() + ref] );
    elements[h->getDomain() +ref]->add_link_out( elements[h->getDomain() + url] );
  } else if( !url.isEmpty() ) {
      elements[h->getDomain() + url]->add_link_in( NULL );
  }

}

