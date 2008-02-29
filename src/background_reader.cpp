#include "background_reader.h"
#include <iostream>

BackgroundReader::BackgroundReader(Elements *e) {
  elements = e;
}

void BackgroundReader::run(void ) {
  forever {
    for(Elements::iterator iter = elements->begin(); iter != elements->end(); ++iter) {
      (*iter)->update_stats();
    }
    sleep(1);
  }
}

