#include "System/System.h"
#include "Menu/Menu.h"
#include "Utilities/InvalidInput.h"
#include "Utilities/utils.h"

using namespace std;

int main() {
    try {
        InitMenu m;
    } catch (exception &error) {
        cout << "Error in Menu" << endl;
        Util::pause();
        return 2;
    }
    return 0;
}
