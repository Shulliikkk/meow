#include <cmath>
#include <iostream>
#include "grav_stab_solver.h"

float* slice(float* mass, int n, int m) {
  float* slice_mass = new float[m - n];
  for (int i = n, j = 0; i < m; i++, j++) {
    slice_mass[j] = mass[i];
  }
  return slice_mass;
}

void rec_in_slice(float* mass_1, float* mass_2, int n, int m) {
  for (int i = n, j = 0; i < m; i++, j++) {
    mass_1[i] = mass_2[j];
  }
}

float norm(float* mass, int n) {
  float sq_sum = 0;
  for (int i = 0; i < n; i++) {
    sq_sum += mass[i] * mass[i];
  }
  return sqrt(sq_sum);
}

float* right_part(float* y) {
  int dim = 2;
  float mu = 3.986e14;

  float* position = slice(y, 0, 2);
  float* velocity = slice(y, 2, 4);
  float angle = y[4];
  float angular_velocity = y[5];
  float time = y[6];
  float radius_vector = norm(position, dim);

  float* result = new float[7];
  float acceleration[dim];
  float angular_acceleration;
  for (int i = 0; i < dim; i++) {
    acceleration[i] = - mu * position[i] / (radius_vector *  radius_vector *  radius_vector);
  }
  angular_acceleration = - 1.5 * (mu / (norm(position, dim) * norm(position, dim) * norm(position, dim))) * sin(2 * angle);

  rec_in_slice(result, velocity, 0, 2);
  rec_in_slice(result, acceleration, 2, 4);
  result[4] = angular_velocity;
  result[5] = angular_acceleration;
  result[6] = time;

  return result;
}

float* calc_one_step(float* y, float delta_t) {
  float* result = new float[7];
  for (int i = 0; i < 6; i++) {
    result[i] = y[i] + right_part(y)[i] * delta_t;
  }
  return result;
}

float** solve(float* y_0, float delta_t, float final_t) {
  int N_steps = int(final_t / delta_t);
  float* current_y = y_0;
  float** solutions = new float*[N_steps];
  solutions[0] = current_y;
  int counter = 0;
  for (int i = 0; i < N_steps; i++) {
    current_y = calc_one_step(current_y, delta_t);
    current_y[6] += delta_t;
    solutions[i] = current_y;
  }
  return solutions;
}

/*
int main() {
  float y_0[] = {6800e3, 0, 0, 8.5e3, 0.001, 0, 0};
  float delta_t = 0.1;
  float final_time = 3600 * 2;
  int N_steps = int(final_time / delta_t);
  float** solutions = solve(y_0, delta_t, final_time);
}
*/
