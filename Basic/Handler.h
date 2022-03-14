#pragma once

struct Handler
{
  const char* name;
  void (*handler)();
  unsigned long time;
  unsigned long last_run = 0;

  Handler(const char* _name, void (*func)(), unsigned long duration = 1000)
  {
    name = _name;
    handler = func;
    time = duration;
  }

  bool should_run()
  {
    return last_run + time >= millis();
  }

  void run() {
    last_run = millis();
    handler();
  }
};