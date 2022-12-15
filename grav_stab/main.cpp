#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <fstream>

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

  delete[] position;
  delete[] velocity;

  return result;
}

float* calc_one_step(float* y, float delta_t) {
  float* result = new float[7];
  float* k1 = right_part(y);

  float* temp2 = new float[7];
  for (int i = 0; i < 7; i++) {
    temp2[i] = y[i] + delta_t / 2 * k1[i];
  }
  float* k2 = right_part(temp2);

  float* temp3 = new float[7];
  for (int i = 0; i < 7; i++) {
    temp3[i] = y[i] + delta_t / 2 * k2[i];
  }
  float* k3 = right_part(temp3);

  float* temp4 = new float[7];
  for (int i = 0; i < 7; i++) {
    temp4[i] = y[i] + delta_t * k3[i];
  }
  float* k4 = right_part(temp4);

  for (int i = 0; i < 6; i++) {
    result[i] = y[i] + delta_t / 6 * (k1[i] + 2 * (k2[i] + k3[i]) + k4[i]);
  }

  delete[] k1;
  delete[] k2;
  delete[] k3;
  delete[] k4;

  result[6] = y[6];
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

std::string time(int sec) {
  int M = sec / 60; //Общее число минут
  int S = sec - (M * 60); //Число секунд
  int H = M / 60; //Число часов
  M = M - (H * 60); //Число минут
  return (std::__cxx11::to_string(H) + ':' + std::__cxx11::to_string(M) + ':' + std::__cxx11::to_string(S));
}

int main()
{
    int X = 700, Y = 700;
    sf::RenderWindow window(sf::VideoMode(X, Y), "SFML works!");
    window.setFramerateLimit(6000);

    float l = 60.0, d = 3.0, r = 4.0;
    sf::Color orange(255, 150, 0);
    sf::RectangleShape rec(sf::Vector2f(l, d));
    sf::CircleShape circle_1(r);
    sf::CircleShape circle_2(r);
    rec.setOrigin(l / 2, d / 2);
    circle_1.setOrigin(- l / 2, r);
    circle_2.setOrigin(l / 2 + r, r);
    rec.setFillColor(orange);
    circle_1.setFillColor(orange);
    circle_2.setFillColor(orange);

    sf::Text deg_text, r_text, v_text, t_text;
    sf::Font font;
    font.loadFromFile("Arial.ttf");
    deg_text.setFont(font);
    r_text.setFont(font);
    v_text.setFont(font);
    t_text.setFont(font);
    deg_text.setCharacterSize(24);
    r_text.setCharacterSize(24);
    v_text.setCharacterSize(24);
    t_text.setCharacterSize(24);

    int X_earth = 350, Y_earth = 350;
    sf::Image earthimage;
	  earthimage.loadFromFile("earth3d350.png");
  	sf::Texture earthtexture;
  	earthtexture.loadFromImage(earthimage);
  	sf::Sprite earthsprite;
  	earthsprite.setTexture(earthtexture);
    earthsprite.setOrigin(X_earth / 2, Y_earth / 2);
  	earthsprite.setPosition(X / 2, Y / 2);


    float y_0[7] = {0};
    std::string line;
    std::ifstream init("init.csv"); // окрываем файл для чтения
    if (init.is_open()) {
        int i = 0;
        while (getline(init, line))
        {
            y_0[i] = std::stof(line);
            i += 1;
        }
    }
    init.close();

    int i = 0;
    float delta_t = 0.1;
    float final_time = 3600 * 6;
    int N_steps = int(final_time / delta_t);
    float** solutions = solve(y_0, delta_t, final_time);
    float* x =  new float[N_steps];
    float* y =  new float[N_steps];
    float* v_x =  new float[N_steps];
    float* v_y =  new float[N_steps];
    float* phi =  new float[N_steps];
    float* t = new float[N_steps];
    for (int i = 0; i < N_steps; i++) {
      x[i] = solutions[i][0];
      y[i] = solutions[i][1];
      v_x[i] = solutions[i][2];
      v_y[i] = solutions[i][3];
      phi[i] = solutions[i][4];
      t[i] = solutions[i][6];
    }

    while (window.isOpen())
    {
      float coef = X_earth / 11e6; //нормировочный коэффициент

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        if (i < N_steps) {
          rec.setPosition(X / 2 + coef * x[i], Y / 2 + coef * y[i]);
          rec.setRotation((180 / M_PI) * (atan(y[i] / x[i]) + phi[i]));
          circle_1.setPosition(X / 2 + coef * x[i], Y / 2 + coef * y[i]);
          circle_2.setPosition(X / 2 + coef * x[i], Y / 2 + coef * y[i]);
          circle_1.setRotation((180 / M_PI) * (atan(y[i] / x[i]) + phi[i]));
          circle_2.setRotation((180 / M_PI) * (atan(y[i] / x[i]) + phi[i]));

          std::string deg = std::__cxx11::to_string(180 / M_PI * phi[i]);
          deg_text.setString(deg.substr(0, 5) + " deg");
          std::string r = std::__cxx11::to_string(sqrt(x[i] * x[i] + y[i] * y[i]) / 1e3 - 6378.1);
          r_text.setString(r.substr(0, 5) + " km");
          std::string v = std::__cxx11::to_string(sqrt(v_x[i] * v_x[i] + v_y[i] * v_y[i]) / 1e3);
          v_text.setString(v.substr(0, 5) + " km/s");
          t_text.setString(time(int(t[i])));
          deg_text.setPosition(295, 630);
          r_text.setPosition(50, 630);
          v_text.setPosition(550, 630);
          t_text.setPosition(305, 30);
        }
        else
          i = -1;
        i++;
        window.draw(earthsprite);
        window.draw(rec);
        window.draw(circle_2);
        window.draw(circle_1);
        window.draw(deg_text);
        window.draw(r_text);
        window.draw(v_text);
        window.draw(t_text);
        window.display();
    }

    for (i = 0; i < N_steps; i++) {
      delete[] solutions[i];
    }
    delete[] solutions;
    delete[] x;
    delete[] y;
    delete[] v_x;
    delete[] v_y;
    delete[] phi;
    delete[] t;

    return 0;
}
