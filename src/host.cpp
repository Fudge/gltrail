#include "host.h"

#include <QStringList>
#include <iostream>

void Host::start(void) {
  proc = new QProcess( this );
  connect( proc, SIGNAL(readyRead()), this, SLOT(readFromStdout()) );
  QString cmd = getCommandString();
  std::cout << "exec[" << cmd.toStdString() << "]" << std::endl;
  std::cout << "rgb[" << color.red() << "," << color.green() << "," << color.blue() << "]" << std::endl;
  proc->start( getCommandString() );
  proc->waitForStarted();
}  


QString Host::getCommandString() {
  QStringList cmd;
  cmd << "ssh";
  cmd << user + "@" + host;

  if( !port.isEmpty() )
    cmd << "-p" << port;

  cmd << command << args;

  return cmd.join(" ");
}

void Host::readFromStdout(void ) {
  char buf[2048];
  if( proc->canReadLine() ) {
    qint64 len = proc->readLine(buf, sizeof(buf));
    if( len != -1 ) {

      //      QString apache_pattern("([\\d\\S.]+) (\\S+) (\\S+) \\[([^\\]]+)\\] \"([^\"]*)\" (\\d+) ([\\S]+) \"([^\"]+)\" \"([^\"]+)\"");
      QRegExp rx(pattern);

      if( rx.indexIn(buf) > -1 ) {
        QString url = rx.cap(5).split(" ")[1];
        QString referrer = rx.cap(8);

	url = url.split("?")[0];

	if( referrer.contains( QRegExp("http://(www.)?" + domain) ) || referrer.startsWith("/") )  {
	    referrer = referrer.split("?")[0];
	}

	referrer.replace("http://", "");
	referrer.replace( QRegExp("^(.*\\.)?" + domain), "");

        std::cout << "[ssh] " <<  (const char*) url.toLatin1() << " <- " << (const char*) referrer.toLatin1() << std::endl;

	if( url == referrer ) {
	  referrer = "";
	}

        gl->addRelation(this, url, referrer);

      } else {
        std::cout << "[ssh] " << buf;
      }
      //      glWidget->addRandomElement();
    }
  }
}
