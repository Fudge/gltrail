#include "inputs/twitter.h"

#include <QStringList>
#include <QTextDocumentFragment>
#include <iostream>
#include "glwidget.h"

using namespace std;

Twitter::Twitter(QObject *parent) {
  http = new QHttp(this);

  connect(http, SIGNAL(done(bool)),
          this, SLOT(parse(bool)) );
  curId = 0;
  fetch = 0;
}

//http://twitter.com/statuses/public_timeline.rss

void Twitter::fetchTweets( void ) {
  QUrl url;
  url.setPath("/statuses/public_timeline.xml");

  QHttpRequestHeader header("GET", url.toString() );

  header.setValue("Host", "twitter.com");
  header.setValue("User-Agent", "glTrail v0.01 (http://www.fudgie.org/gltrail.html)" );

  http->setHost("twitter.com");

  cout << "Fetching[" << url.toString().toStdString() << "]" << endl;

  http->request(header);
}

void Twitter::run( void ) {

  fetchTweets();
  forever {
    if( tweets.size() > 0 ) {
      cout << tweets.size() << " pending tweets" << endl;

      Tweet t = tweets.last();

      cout << t.id << " < " << curId << endl;

//       if( t.id > curId + 5 ) {
//         curId = t.id;
//       }

      if( t.id > curId && tweets.size() > 0 ) {

        curId = t.id;

        gl->addRelation(this, t.title, t.user, true);

        if(t.title.contains("@")) {
          // Tweet directed at someone
          QString targetPattern("@([^: ]+)");
          QRegExp rx(targetPattern);
          if( rx.indexIn( t.title ) > -1 ) {
            QString target = rx.cap(1);

            gl->addRelation(this, target, t.title, true);
          }
        }

        tweets.removeLast();
        if( tweets.size() > 0 ) {
          t = tweets.last();
        }
      } else {
        while( tweets.last().id <= curId && tweets.size() > 0 ) {
          tweets.removeLast();
        }
      }

    }

    fetch++;

    if( fetch % 30 == 0 ) {
      fetchTweets();
    }

    //    if( curId > 0 ) {
    //      curId += 1;
    //    }


    sleep(1);
  }
}

void Twitter::parse( bool error ) {
  if( !error ) {
    QString result = http->readAll();
    // cout << "Got " << result.toStdString() << endl;

    QStringList lines = result.split('\n');

    //    cout << "lines: " << lines.size() << endl;

    QString textPattern( "<text>(.*)</text>");
    QRegExp rx(textPattern);

    QString userPattern( "<screen_name>(.*)</screen_name>");
    QRegExp rxUser(userPattern);

    QString idPattern( "    <id>(\\d+)</id>");
    QRegExp rxId(idPattern);

    Tweet t;

    for( int i = 0; i < lines.size(); i++ ) {

      if( lines[i].contains("<status>") ) {
        t.id = 0;
        t.title = "";
        t.user = "";
      }

      if( rx.indexIn( lines[i] ) > -1 ) {
        t.title = QTextDocumentFragment::fromHtml(rx.cap(1)).toPlainText();
      } else if( rxUser.indexIn( lines[i] ) > -1 ) {
        cout << "<screen_name>" << endl;
        t.user = rxUser.cap(1);
      } else if( rxId.indexIn( lines[i] ) > -1 && t.id == 0) {
        t.id = rxId.cap(1).toLong();
      }

      if( lines[i].contains("</status>") ) {

        cout << t.id << "," << t.user.toStdString() << "," << t.title.toStdString() << endl;

        tweets << t;
      }

    }

  } else {
    cout << "Http error" << endl;
  }

}
