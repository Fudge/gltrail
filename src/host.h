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

  Host(QObject *parent) : QObject(parent) {};
  ~Host() {};

  void setHost(const QString &h)    { host = h; };
  void setDomain(const QString &d)  { domain = d; };
  void setUser(const QString &u)    { user = u; };
  void setPort(const QString &p)    { port = p; };
  void setCommand(const QString &c) { command = c; };
  void setArgs(const QString &a)    { args = a; };
  void setPattern(const QString &p) { pattern = p; };
  void setIgnore(const QString &i) { ignore = i; };
  void setColor(const QString &c)   { color = QColor(c); };
  void setIgnoreQueryParameters(bool p) { ignoreParams = p; };

  void addReplacement(const QString &p, const QString &s) {
    replacementPatterns << p;
    replacementStrings << s;
  };

  void setGLWidget(GLWidget *glw) { gl = glw; };
  GLWidget *getGLWidget() const { return gl; };

  QString getCommandString( void );
  QString getPattern( void ) { return pattern; };
  QString getIgnore( void ) { return ignore; };
  bool    ignoreQueryParameters( void ) { return ignoreParams; };

  QString getDomain( void ) const { return domain; };
  QColor getColor( void ) const { return color; };

  void start();
  void end();

public slots:

  void readFromStdout(void);
  void readFromStderr(void);

 private:
  QProcess *proc;

  GLWidget *gl;

  QString host;
  QString domain;
  QString user;
  QString port;
  QString command;
  QString args;

  QString ignore;
  bool    ignoreParams;
  QString pattern;
  QColor  color;

  QStringList replacementPatterns;
  QStringList replacementStrings;

};

typedef QList<Host *> Hosts;

#endif
