#ifndef HOST_H
#define HOST_H

#include <QObject>
#include <QList>
#include <QProcess>
#include <QColor>


class GLWidget;

class Host : public QObject {

  Q_OBJECT

 public:

  Host(QObject *parent) : QObject(parent) { maxSize = 0.0; };
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
  GLWidget *getGLWidget() const { return gl; };

  QString getCommandString( void );
  QString getPattern( void ) { return pattern; };

  QString getDomain( void ) const { return domain; };
  QColor getColor( void ) const { return color; };

  float getMaxSize() const { return (maxSize > 1.0 ? maxSize : 1.0); };
  void setMaxSize(float s) { maxSize = s; };
  void decayMax(void) { maxSize = maxSize * 0.99; };

  void start();

public slots:
  void readFromStdout(void);
  void readFromStderr(void);

 private:
  QProcess *proc;

  GLWidget *gl;
  float     maxSize;

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
