#ifndef WINDOWTITLEWATCHER_H
#define WINDOWTITLEWATCHER_H

#include "watcher.h"

class WindowTitleWatcher : public Watcher
{
  public:
    WindowTitleWatcher();
    int eventTypeId() const override;

  protected:
    std::string pollEvent() override;
};

#endif  // WINDOWTITLEWATCHER_H
