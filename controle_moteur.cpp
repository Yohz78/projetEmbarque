#include <iostream>
#include <cstdio>

using namespace std;

int main()
{
    int position;
    while (true) {
        cout << "Entrez une instruction (1 à 5) : ";
        cin >> position;

        switch (position) {
            case 1:
                // code pour contrôler le moteur 1
                break;
            case 2:
                // code pour contrôler le moteur 2
                break;
            case 3:
                // code pour contrôler le moteur 3
                break;
            case 4:
                // code pour contrôler le moteur 4
                break;
            case 5:
                // code pour contrôler le moteur 5
                break;
            default:
                cout << "Instruction non valide." << endl;
                break;
        }
    }
    return 0;
}
