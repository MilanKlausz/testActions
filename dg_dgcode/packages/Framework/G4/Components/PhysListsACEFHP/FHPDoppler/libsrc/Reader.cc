#include "FHPDoppler/Reader.hh"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <algorithm>    // std::lower_bound, std::upper_bound, std::sort
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Reader::Reader(std::string fileName)
:fullPath(fileName)
{
  ifstream infile;
  std::string line;

  infile.open (fullPath.c_str());
  getline(infile,line);
  getline(infile,line);
  getline(infile,line);

  while (std::getline(infile, line)) {
      vector <string> strVec;
      SplitString( strVec, line, ' ' );

      for(unsigned i=0 ; i<strVec.size()/2 ;i++)  {
          energy.push_back(atof(strVec.at(i*2).c_str()));
          crossSection.push_back(atof(strVec.at(i*2+1).c_str()));
      }
  }

  infile.close();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Reader::~Reader()
{
    energy.clear();
    crossSection.clear();
}




template <typename Container>
Container& Reader::SplitString(
        Container&                                 result,
        const typename Container::value_type&      thestring,
        typename Container::value_type::value_type delimiter
        )
{
    result.clear();
    std::istringstream ss( thestring );
    while (!ss.eof())
    {
        typename Container::value_type field;
        getline( ss, field, delimiter );
        if (field.empty()) continue;
        result.push_back( field );
    }
    return result;
}

std::vector<double> *Reader::GetEnergyGrid()
{
   return &energy;
}

std::vector<double> *Reader::GetXsGrid()
{
    return &crossSection;
}

