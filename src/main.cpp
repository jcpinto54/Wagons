#include "System/System.h"
#include "Menu/Menu.h"
#include "Utilities/InvalidInput.h"
#include "Utilities/utils.h"

using namespace std;

int main() {
    System *sys;
    try {
        sys = new System("../../data/files.txt");
    } catch (InvalidInput &error) {
        cout << error.getMsg() << endl;
        Util::pause();
        return 1;
    }
    try {
        MainMenu m(sys);
    } catch (exception &error) {
        cout << "Error in Menu" << endl;
        Util::pause();
        return 2;
    }
    return 0;
}
