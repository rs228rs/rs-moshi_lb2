#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <SFML/Graphics.hpp>
using namespace std;

double test_f(double x) {
    return sin(x);
}

double main_f(double x) {
    return exp(x) / x;
}

double integral(double a, double b, int n, bool is_test_f) {
    double h = (b - a) / n;
    double sum;
    if (is_test_f) {
        sum = 0.5 * (test_f(a) + test_f(b));
    }
    else {
        sum = 0.5 * (main_f(a) + main_f(b));
    }
    for (int i = 1; i < n; ++i) {
        double x = a + i * h;
        if (is_test_f) {
            sum += test_f(x);
        }
        else {
            sum += main_f(x);
        }
    }
    return h * sum;
}

double random_p(double a, double b) {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_real_distribution<> dis(a, b);
    double p = dis(gen);
    return p;
}

double func_value(double (*func)(double), double x, bool is_test_f) {
    if (is_test_f) {
        return func(x);
    }
    else {
        return func(exp(x));
    }
}

double monte_carlo( double a, double b, int n, bool is_test_f) {
    double sum = 0;
    double f;
    vector<double> z(n);
    vector<double> y(n);

    for (int i = 0; i < n; ++i) {
        double p = random_p(a, b);
        if (is_test_f) {
            f = test_f(a + (b - a) * p);
            z[i] = a + (b - a) * p;
            y[i] = f;
        }
        else {
            f = main_f(a + (b - a) * p);
            z[i] = a + (b - a) * p;
            y[i] = f;
        }
        sum += f;
    }

    sf::RenderWindow window(sf::VideoMode(800, 600), "graf");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        float osx = 0;
        float osy = osx;


        window.clear();
        for (int i = 0; i < n; i++)
        {
            float posX = z[i];
            float posY = 600 - y[i];
            sf::CircleShape point(3);
            point.setFillColor(sf::Color::Red);
            point.setPosition(posX, posY);
            window.draw(point);
        }
        window.display();
    }

    return (b - a) / n * sum ;
}



int main() {
    int const n = 20;

    double test_a = 0.0;
    double test_b = 1.0;
    double test_integral = integral(test_a, test_b, n, true);
    double test_monte_carlo = monte_carlo(test_a, test_b, n, true);
    double test_error = abs(test_integral - test_monte_carlo);
    cout << " test_f: sin(x) - [" << test_a << ";" << test_b << "]" << endl;
    cout << " test_integral: " << test_integral << endl;
    cout << " test_monte_carlo: " << test_monte_carlo << endl;
    cout << " error: " << test_error << endl;

    double main_a = 1.0;
    double main_b = 2.0;
    double main_integral = integral(main_a, main_b, n, false);
    double main_monte_carlo = monte_carlo(main_a, main_b, n, false);
    double main_error = abs(main_integral - main_monte_carlo);
    cout << " main_f: exp(x)/x - [" << main_a << ";" << main_b << "]" << endl;
    cout << " main_integral: " << main_integral << endl;
    cout << " main_monte_carlo: " << main_monte_carlo << endl;
    cout << " error: " << main_error << endl;

    return 0;
}

