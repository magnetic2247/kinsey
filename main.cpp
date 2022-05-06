#include <boost/filesystem.hpp>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;
namespace fs = boost::filesystem;

/*
Name: Kinsey
Author: Polis Minus
License: GPLv3
Description: Logs a user's Kinsey Scale rating daily
*/

int main() {
    // Get XDG Config directory
    string config;
    if (getenv("XDG_CONFIG_HOME") == NULL) {
        config = getenv("HOME");
        config = config+"/.config";
    }
    else config = getenv("XDG_CONFIG_HOME");

    // Create config directory if needed
    // Create epoch file if empty
    if (!fs::is_directory(config+"/kinseyscale/")) fs::create_directory(config+"/kinseyscale/");
    fstream epoch_file;
    epoch_file.open(config+"/kinseyscale/last_epoch", ios::in | ios::out);
    epoch_file.seekg(0, epoch_file.end);
    if (epoch_file.tellg() == 0) epoch_file << "0" << endl;

    // Read the last saved epoch 
    // Stop if already answered within the last 24 hours
    // Basically if current_epoch - last_epoch < 86400
    string last_epoch_str;
    epoch_file.seekg(0, ios::beg);
    getline(epoch_file, last_epoch_str);
    int last_epoch = stoi(last_epoch_str);
    if (time(0)-last_epoch < 86400) { // Less than 24 hours since last epoch, quit
        epoch_file.close();
        return 0;
    }

    // Open CSV File
    // Write CSV headers if file is empty
    fstream csv_file;
    csv_file.open(config+"/kinseyscale/data.csv", ios::app | ios::ate);
    if (csv_file.tellg() == 0) csv_file << "epoch,kinsey" << endl;

    // User Input
    // Repeat the question until input is valid
    // The Kinsey Scale ranges from 0 to 6
    int kinsey = -1;
    do {
        cout << "Daily Kinsey Scale Rating: ";
        cin >> kinsey;
    } while (kinsey < 0 || kinsey > 7);

    // Write data to csv file + last epoch
    csv_file << time(0) << "," << kinsey << endl;
    epoch_file.seekg(0, ios::beg);
    epoch_file << time(0);

    // Close Files
    epoch_file.close();
    csv_file.close();

    return 0;
}
