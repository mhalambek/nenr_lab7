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
  private:
  vector<Sample> data;

  public:
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
  };

  auto begin() const { return data.begin(); };
  auto end() const { return data.end(); };
  auto size() const { return data.size(); };
};
