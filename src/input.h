#ifndef INPUT_H
#define INPUT_H

#include <QObject>
#include <QList>
#include <QProcess>
#include <QColor>


class GLWidget;

class Input  {

 public:

  Input() {};
  virtual ~Input() {};

  void setDomain(const QString &d)  { domain = d; };
  void setColor(const QString &c)   { color = QColor(c); };

  QString getDomain( void ) const { return domain; };
  QColor  getColor( void ) const { return color; };

  void setGLWidget(GLWidget *glw) { gl = glw; };
  GLWidget *getGLWidget() const { return gl; };

  void    setAutoPurge(bool p)  {autoPurge = p; };
  bool    doPurge( void ) const { return autoPurge; };

  virtual void startProcessing();
  virtual void endProcessing();
 protected:
  GLWidget *gl;
  QString domain;
  QColor  color;

  bool    autoPurge;

};

typedef QList<Input *> Inputs;

#endif
