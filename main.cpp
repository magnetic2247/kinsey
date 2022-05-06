#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;
namespace fs = boost::filesystem;

int main() {

  // Read the last saved epoch 
  // Stop if already answered within the last 24 hours
  // Basically if current_epoch - last_epoch < 86400
  ifstream epoch_file;
  epoch_file.open("/home/minus/.config/kinseyscale/last_epoch");
  string last_epoch_str;
  getline(epoch_file, last_epoch_str);
  int last_epoch = stoi(last_epoch_str);
  if (time(0)-last_epoch < 86400) return 0;

  // THIS CODE IS AWFUL! I'm gonna rewrite it all.

  // Kinsey Scale Rating
  int kinsey = -1;
  
  // Repeat the question until input is valid
  // The Kinsey Scale ranges from 0 to 6
  do {
    cout << "Daily Kinsey Scale Rating: ";
    cin >> kinsey;
  } while (kinsey < 0 || kinsey > 7);

  // Find Home Directory
  string home = getenv("HOME");
  // Create XDG config if needed
  if (!fs::is_directory(home+"/.config/kinseyscale")) fs::create_directory(home+"/.config/kinseyscale");

  // Open CSV File
  fstream file;
  file.open(home+"/.config/kinseyscale/data.csv", ios::app | ios::ate);

  // Write CSV headers if file is empty
  if (file.tellg() == 0) file << "epoch,kinsey" << endl;
  // Write CSV Data
  file << time(0) << "," << kinsey << endl;

  // Write epoch to date file
  file.close();
  file.open(home+"/.config/kinseyscale/last_epoch", ios::out |  ios::trunc);
  file << time(0);
  file.close();
  

  return 0;
}

