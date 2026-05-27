/*
 *Student Name: Dalton Goodwin
 *File Name: Week11_Discussion.cpp
 *Date: 5/26/2026
 */

#include <iostream>
#include <string>
#include <utility>
#include <conio.h>

using namespace std;

//this template class can store one item of any data type
template <class T>
class EquipmentBox {
private:
    //this stores the equipment item inside the object
    T item;

public:
    //this constructor receives an item and saves it
    EquipmentBox(T newItem) {
        item = std::move(newItem);
    }

    //this function returns the stored item
    T getItem() {
        return item;
    }
};

//this class represents a network device on a small base network
class NetworkDevice {
private:
    //this stores the name of the device
    string deviceName;

    //this stores how many ports are active on the device
    int activePorts;

public:
    //this constructor sets up a network device with a name and port count
    NetworkDevice(const string& name, const int ports) {
        deviceName = name;
        activePorts = ports;
    }

    //this overloaded plus operator adds the active ports from two devices
    NetworkDevice operator+(const NetworkDevice& otherDevice) const {
        //this creates a new combined device using both port counts
        NetworkDevice combinedDevice("Combined Device", activePorts + otherDevice.activePorts);

        //this sends the combined device back to main
        return combinedDevice;
    }

    //this member function checks if the device has a valid port count
    void checkPorts() {
        //this starts the exception handling block
        try {
            //this checks for a port count that should not be allowed
            if (activePorts < 0) {
                //this throws an error message when the port count is negative
                throw runtime_error("active ports cannot be negative");
            }

            //this prints the device name and active port count if no problem is found
            cout << deviceName << " has " << activePorts << " active ports." << endl;
        }
        catch (const runtime_error& message) {
            //this prints the error message from the throw statement
            cout << "Port check failed: " << message.what() << endl;
        }
    }

    //this member function prints the stored device information
    void printDevice() {
        //this prints the network device name
        cout << "Device Name: " << deviceName << endl;

        //this prints the number of active ports
        cout << "Active Ports: " << activePorts << endl;
    }
};

int main() {
    //this creates the first network device object
    NetworkDevice router("Main Router", 12);

    //this creates the second network device object
    NetworkDevice switchOne("Access Switch", 24);

    //this uses the overloaded plus operator to combine the port counts
    NetworkDevice totalDevice = router + switchOne;

    //this creates a class template object that stores a string value
    EquipmentBox<string> sparePart("fiber module");

    //this prints the item stored inside the template object
    cout << "Stored Equipment: " << sparePart.getItem() << endl;

    //this checks the combined device for a valid port count
    totalDevice.checkPorts();

    //this prints the combined device information
    totalDevice.printDevice();

    //this creates a network device with an invalid active port count
    NetworkDevice brokenDevice("Bad Switch", -5);

    //this calls the member function that uses exception handling
    brokenDevice.checkPorts();

    getch();

    //this ends the program
    return 0;
}
