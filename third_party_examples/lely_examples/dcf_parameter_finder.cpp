#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

using namespace std;

struct ParameterInfo {
  int index;
  int subindex;
};
int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s <dcf file>", argv[0]);
    return EXIT_FAILURE;
  }

  // Read the DCF file into an unordered_map where parameter names are keys and ParameterInfo
  // objects are values

  // Read the DCF file into an unordered_map where parameter names are keys and ParameterInfo
  // objects are values
  ifstream dcfFile(argv[1]);  // Replace "example.dcf" with your DCF file name
  unordered_map<string, ParameterInfo> parameterMap;

  if (dcfFile.is_open()) {
    string line;
    string currentParameter;
    int currentIndex = 0;
    int currentSubindex = 0;
    regex pattern("\\[(\\d+)\\]");
    smatch matches;

    while (getline(dcfFile, line)) {
      if (regex_match(line, matches, pattern)) {
        currentIndex = stoi(matches[1]);
        currentSubindex = 0;
      } else if (line.find("ParameterName=") != string::npos) {
        // Extract the parameter name
        string paramName = line.substr(line.find("=") + 1);
        parameterMap[paramName] = {currentIndex, currentSubindex};
      } else if (line.find("ParameterName=") == string::npos &&
                 line.find("[" + to_string(currentIndex) + "sub") != string::npos) {
        // Extract subindex from [indexsubX] format
        currentSubindex = stoi(line.substr(line.find("sub") + 3));
      }
    }

    dcfFile.close();
  } else {
    cout << "Unable to open the DCF file." << endl;
    return 1;
  }

  // Get list of parameter names from the user separated by commas
  string inputNames;
  cout << "Enter parameter names separated by commas: ";
  getline(cin, inputNames);

  // Split the input into individual parameter names
  istringstream iss(inputNames);
  string paramName;
  while (getline(iss, paramName, ',')) {
    // Search for the parameter in the unordered_map
    auto it = parameterMap.find(paramName);
    if (it != parameterMap.end()) {
      cout << "Parameter: " << paramName << ", Index: " << it->second.index
           << ", Subindex: " << it->second.subindex << endl;
    } else {
      cout << "Parameter \"" << paramName << "\" not found in the DCF file." << endl;
    }
  }
  return 0;
}
