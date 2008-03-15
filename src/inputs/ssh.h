#ifndef HOST_H
#define HOST_H

#include <QObject>
#include <QList>
#include <QProcess>
#include <QColor>
#include "../input.h"

class GLWidget;

class SSH : public Input {

  Q_OBJECT

 public:

  SSH(QObject *parent) : Input(parent) {};
  ~SSH() {};

  void setHost(const QString &h)    { host = h; };
  void setUser(const QString &u)    { user = u; };
  void setPort(const QString &p)    { port = p; };
  void setCommand(const QString &c) { command = c; };
  void setArgs(const QString &a)    { args = a; };
  void setPattern(const QString &p) { pattern = p; };
  void setIgnore(const QString &i)  { ignore = i; };
  void setIgnoreQueryParameters(bool p) { ignoreParams = p; };
  void setAutoPurge(bool p)         {autoPurge = p; };

  void addReplacement(const QString &p, const QString &s) {
    replacementPatterns << p;
    replacementStrings << s;
  };

  QString getCommandString( void );
  QString getPattern( void ) { return pattern; };
  QString getIgnore( void ) { return ignore; };
  bool    ignoreQueryParameters( void ) { return ignoreParams; };

  void start();
  void end();

public slots:

  void readFromStdout(void);
  void readFromStderr(void);

 private:
  QProcess *proc;

  QString host;
  QString user;
  QString port;
  QString command;
  QString args;

  QString ignore;
  bool    ignoreParams;
  QString pattern;

  QStringList replacementPatterns;
  QStringList replacementStrings;

};

#endif
