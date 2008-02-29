#ifndef HOST_H
#define HOST_H

#include <QObject>
#include <QList>
#include <QProcess>
#include <QColor>

#include "glwidget.h"

class Host : public QObject {

  Q_OBJECT

 public: 

  Host(QObject *parent) : QObject(parent) {};
  ~Host() {};

  void setHost(const QString &h)    { host = h; };
  void setDomain(const QString &d)  { domain = d; };
  void setUser(const QString &u)    { user = u; };
  void setPort(const QString &p)    { port = p; };
  void setCommand(const QString &c) { command = c; };
  void setArgs(const QString &a)    { args = a; };
  void setPattern(const QString &p) { pattern = p; };
  void setColor(const QString &c)   { color = QColor(c); };

  void setGLWidget(GLWidget *glw) { gl = glw; };

  QString getCommandString( void );
  QString getPattern( void ) { return pattern; };

  QString getDomain( void ) const { return domain; };
  QColor getColor( void ) const { return color; };

  void start();

public slots:
  void readFromStdout(void);

 private: 
  QProcess *proc;

  GLWidget *gl;

  QString host;
  QString domain;
  QString user;
  QString port;
  QString command;
  QString args;

  QString pattern;
  QColor  color;

};

typedef QList<Host *> Hosts;

#endif
