#include <unistd.h>

#include "timer.hh"
#include "lib.hh"

static int ticks = 0;

// long timerGet(void)
// {
//   double long t;

//   struct timeval now, res;

//   gettimeofday(&now, 0);

//   t = (now.tv_sec * 1000) + (now.tv_usec / 1000);

//   return (long)t;
// }

// void timerSleep(long miliseconds)
// {
//   struct timeval tv;

//   tv.tv_usec = (miliseconds % 1000) * 1000;
//   tv.tv_sec = miliseconds / 1000;
//   select(0, 0, 0, 0, &tv);
// }

static Uint32 ticktock(Uint32 interval)
{
  ++ticks;

  return(interval);
}

int timer_init(int resolution)
{
  if (SDL_Init(SDL_INIT_TIMER) < 0)
  {
    fprintf(stderr, "Erreur d'initialisation de SDL: %s\n", SDL_GetError());
    exit(1);
  }
  atexit(SDL_Quit);

  SDL_SetTimer(resolution, ticktock);
}
