#include "System/System.h"
#include "Menu/Menu.h"

using namespace std;

int main() {
    System sys = System("../data/files.txt");

    MainMenu menu = MainMenu(&sys);

    return 0;
}
