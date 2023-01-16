#include <iostream>
#include <cstdio>
#include <thread>

using namespace std;

void motor1() {
    // code pour contrôler le moteur 1
}

void motor2() {
    // code pour contrôler le moteur 2
}

void motor3() {
    // code pour contrôler le moteur 3
}

void motor4() {
    // code pour contrôler le moteur 4
}

void motor5() {
    // code pour contrôler le moteur 5
}

int main()
{
    int position;
    while (true) {
        cout << "Entrez une instruction (1 à 5) : ";
        cin >> position;

        switch (position) {
            case 1:
                thread t1(motor1);
                t1.join();
                break;
            case 2:
                thread t2(motor2);
                t2.join();
                break;
            case 3:
                thread t3(motor3);
                t3.join();
                break;
            case 4:
                thread t4(motor4);
                t4.join();
                break;
            case 5:
                thread t5(motor5);
                t5.join();
                break;
            default:
                cout << "Instruction non valide." << endl;
                break;
        }
    }
    return 0;
}
