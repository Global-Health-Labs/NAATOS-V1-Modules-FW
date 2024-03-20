#ifndef  PID_H
#define  PID_H

/* Based on PID Conroller:
   https://github.com/geekfactory/PID
*/

/* PID Controller Parameters */
#define PID_LIM_MIN  0.0f   // TODO: Change these to acutal min maxes
#define PID_LIM_MAX  150.0f

typedef struct {
  /* Controller Gains */
  float k_p;
  float k_i;
  float k_d;
  /* Output Limits */
  float lim_max;
  float lim_min;
  /* Controller */
  float intergrator;
  float prevMesurement;
  /* Controller Output */
  float out;
  /* Controller Setpoint */
  float setpoint;
} pid_controller_t;

void pid_controller_create(pid_controller_t * pid);
void pid_controller_compute(pid_controller_t * pid, float measurement);

#endif