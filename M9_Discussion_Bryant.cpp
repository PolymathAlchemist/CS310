#include <iostream>
#include <conio.h>

using namespace std;


class Animal {
protected:
    string name;

public:
    
    Animal(string n) {
        name = n;
    }

    void speak() {
        cout << name << " makes a sound." << endl;
    }
};


class Dog : public Animal {
public:
    Dog(string n): Animal(n) {}

    void speak() {
        cout << name << " barks." << endl;
    }
};


class Cat : public Animal {
public:
    Cat(string n) : Animal(n) {}

    void speak() {
        cout << name << " meows." << endl;
    }
};

int main() {
    Animal a("Generic Animal");
    Dog d("Buddy");
    Cat c("Whiskers");

    a.speak();   
    d.speak();   
    c.speak();   

    getch();

    return 0;
}