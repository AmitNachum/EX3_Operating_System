#include "improved.hpp"
#include <limits>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::pair;

void print_points(const vector<Point*>& points) {
    cout << "Current Points:" << endl;
    for (const auto* p : points) {
        if (p)
            cout << "(" << p->_point.first << ", " << p->_point.second << ")" << endl;
    }
}

int main() {
    cout << "==== Convex Hull ====" << endl << endl;

    size_t n;
    while (true) {
        cout << "Enter the amount of points:" << endl;
        cin >> n;

        if (cin.fail()) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (n < 1) {
            cout << "Number must be positive." << endl;
        } else {
            cin.ignore(); // flush newline
            break;
        }
    }

    vector<Point*> convex_hull_points;

    while (true) {
        size_t choice = 0;

        while (true) {
            cout << "\n=== Make your Choice ===" << endl;
            cout << "1) Add Point" << endl;
            cout << "2) Remove Point" << endl;
            cout << "3) Quit" << endl;
            cout << "4) Calculate and Print CH's Area" << endl;
            cin >> choice;

            if (cin.fail() || choice < 1 || choice > 4) {
                cout << "Invalid choice. Please enter 1, 2, 3, or 4." << endl;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else {
                cin.ignore(); // flush newline
                break;
            }
        }

        if (choice == 3) {
            cout << "Exiting. Cleaning up..." << endl;
            delete_pointers(convex_hull_points);
            return 0;
        }

        if (choice == 4) {
            float area = calculate_area(convex_hull_points);
            cout << "CH's Area: " << area << endl;
            continue;
        }

        if (choice == 1 && convex_hull_points.size() >= n) {
            cout << "Maximum number of points (" << n << ") reached." << endl;
            continue;
        }

        if (choice == 1) {
            cout << "Enter point to add (i,j):" << endl;
        } else if (choice == 2) {
            cout << "Enter point to remove (i,j):" << endl;
        }

        string line;
        std::getline(cin, line);

        size_t comma = line.rfind(',');
        if (comma == string::npos) {
            cout << "Invalid format. Use: i,j" << endl;
            continue;
        }

        string i_str = line.substr(0, comma);
        string j_str = line.substr(comma + 1);

        try {
            float x = std::stof(i_str);
            float y = std::stof(j_str);

            if (choice == 1) {
                add_point(convex_hull_points, x, y);
            } else if (choice == 2) {
                if (delete_ith_pointer(convex_hull_points, x, y)) {
                    cout << "Point removed." << endl;
                } else {
                    cout << "Point not found." << endl;
                }
            }

            print_points(convex_hull_points);

        } catch (...) {
            cout << "Invalid coordinates. Please enter valid numbers." << endl;
        }
    }
}
