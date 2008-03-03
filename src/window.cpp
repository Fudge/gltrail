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

#include <QtGui>
#include <iostream>
#include <QDebug>

#include "window.h"
#include "host.h"

Hosts hosts;

Window::Window(QWidget *parent)
 : QWidget(parent)
{

  glWidget = new GLWidget(this, &hosts);

  QHBoxLayout* layout = new QHBoxLayout();
  layout->setContentsMargins(0,0,0,0);
  layout->addWidget(glWidget);
  setLayout(layout);

  glWidget->show();

  setWindowTitle("GLTrail 0.01");

  readSettings(glWidget);
}


Window::~Window()
{
}

void Window::readSettings(GLWidget *gl) {
  QSettings settings("gltrail.ini", QSettings::IniFormat);

  settings.beginGroup("hosts");
  QStringList rels = settings.allKeys();
  settings.endGroup();

  for( int i = 0; i < rels.size(); i++ ) {
    QString hostName = rels.at(i);

    settings.beginGroup("hosts");
    QString domain = settings.value(hostName).toString();
    std::cout << "Reading [" << domain.toStdString() << "]" << std::endl;
    settings.endGroup();

    Host *h = new Host(this);
    settings.beginGroup(domain);
    h->setHost(hostName);
    h->setDomain(domain);
    h->setUser( settings.value("user").toString() );
    h->setPort( settings.value("port").toString() );
    h->setCommand( settings.value("command").toString() );
    h->setArgs( settings.value("args").toString() );
    h->setPattern( settings.value("pattern").toString() );
    h->setIgnore( settings.value("ignore").toString() );
    h->setColor( settings.value("color").toString() );

    h->setGLWidget(gl);

    settings.endGroup();
    std::cout << "Read [" << hostName.toStdString() << "]" << std::endl;

    hosts << h;
    h->start();
  }

}

