#ifndef INPUT_H
#define INPUT_H

#include <QObject>
#include <QList>
#include <QProcess>
#include <QColor>


class GLWidget;

class Input : public QObject {

  Q_OBJECT

 public:

  Input(QObject *parent) : QObject(parent) {};
  ~Input() {};

  void setDomain(const QString &d)  { domain = d; };
  void setColor(const QString &c)   { color = QColor(c); };

  QString getDomain( void ) const { return domain; };
  QColor  getColor( void ) const { return color; };

  void setGLWidget(GLWidget *glw) { gl = glw; };
  GLWidget *getGLWidget() const { return gl; };

  bool    doPurge( void ) const { return autoPurge; };

  virtual void start() {};
  virtual void end() {};
 protected:
  GLWidget *gl;
  QString domain;
  QColor  color;

  bool    autoPurge;

};

typedef QList<Input *> Inputs;

#endif
