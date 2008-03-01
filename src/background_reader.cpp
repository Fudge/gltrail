#include "background_reader.h"
#include <iostream>

BackgroundReader::BackgroundReader(Hosts *h, Elements *e) {
  hosts = h;
  elements = e;
}

void BackgroundReader::run(void ) {
  forever {
    for(Hosts::iterator it = hosts->begin(); it != hosts->end(); ++it ) {
      (*it)->decayMax();
    }

    int num = 0;
    for(Elements::iterator iter = elements->begin(); iter != elements->end(); ++iter) {
      (*iter)->update_stats();
      num ++;
    }

    std::cout << "Elements: " << num << std::endl;

    sleep(1);
  }
}

