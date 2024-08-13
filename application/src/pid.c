#include "pid.h"

void pid_controller_init(pid_controller_t *pid, float setpoint, float k_p, float k_i, float k_d, int pid_max) {
  // Clear controller variables
  pid->intergrator = 0.0f;
  pid->prevMesurement = 0.0f;
  pid->intergrator = 0.0f;
  pid->out = 0.0f;
  // Set from parameters
  pid->setpoint = setpoint;
  pid->k_p = k_p;
  pid->k_i = k_i;
  pid->k_d = k_d;
  // Set from defines
  pid->lim_max = pid_max;
  pid->lim_min = PID_LIM_MIN;
}

void pid_controller_compute(pid_controller_t *pid, float measurement) {
  // Error Signal
  float error = pid->setpoint - measurement;
  // Proportional
  float proportial = pid->k_p * error;
  // Compute Integral
  pid->intergrator += (pid->k_i * error);
  if (pid->intergrator > pid->lim_max) {
    pid->intergrator = pid->lim_max;
  } else if (pid->intergrator < pid->lim_min) {
    pid->intergrator = pid->lim_min;
  }
  // Compute Differential on Input
  float d_input = measurement - pid->prevMesurement;
  // Compute PID Output
  float out = pid->k_p * error + pid->intergrator - pid->k_d * d_input;
  if (out > pid->lim_max) {
    out = pid->lim_max;
  } else if (out < pid->lim_min) {
    out = pid->lim_min;
  }
  // Set output
  pid->out = out;
  // Keep Track of for Next Execution
  pid->prevMesurement = measurement;
}