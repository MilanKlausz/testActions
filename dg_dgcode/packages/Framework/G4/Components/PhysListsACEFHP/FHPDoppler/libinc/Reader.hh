//A class to read Geant4 HP model format data file
//const G4String ElementNames is copied from Geant4.9.6
//method SplitString is copied from http://stackoverflow.com/questions/10909955/error-c2893-failed-to-specialize-function-template-c
//x.x. cai, 13-Feb-2014
#ifndef Reader_h
#define Reader_h 1


#include <vector>
#include <sstream>
#include <string>


class Reader
{
public:

    Reader(std::string fileName);
    virtual ~Reader();

public:
    std::vector<double> * GetEnergyGrid();
    std::vector<double> * GetXsGrid();

    template <typename Container>
    Container& SplitString(
            Container&                                 result,
            const typename Container::value_type&      s,
            typename Container::value_type::value_type delimiter);

private:
    std::vector<double> energy;
    std::vector<double> crossSection;

    std::string fullPath;

    std::string int2string(int a)
    {
        std::stringstream ss;
        ss << a;
        return ss.str();
    }



};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif








