#pragma once
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct Sample {
  float x;
  float y;
  bool A, B, C;
};

class Dataset {
  public:
  vector<Sample> data;

  Dataset(const char* sourceFilePath)
  {
    ifstream sourceFileStream(sourceFilePath);
    string line;
    while (getline(sourceFileStream, line)) {
      istringstream fin(line);
      Sample s;
      fin >> s.x;
      fin >> s.y;
      fin >> s.A;
      fin >> s.B;
      fin >> s.C;
      data.push_back(s);
    }

    sourceFileStream.close();
  }
};
