#ifndef TIMER_HH_
# define TIMER_HH_

typedef struct	timer
{
  int		last_time;
  int		cur_time;
  int		diff_time;
  int		accumulator;
}		s_timer;

// long timerGet(void);
// void timerSleep(long miliseconds);
int  timer_init(int resolution);

#endif /* !TIMER_HH_ */
