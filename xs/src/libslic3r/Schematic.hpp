#ifndef slic3r_Schematic_hpp_
#define slic3r_Schematic_hpp_

#include "libslic3r.h"
#include "ElectronicPart.hpp"
#include "ElectronicNet.hpp"
#include "Point.hpp"
#include <vector>


namespace Slic3r {

class Schematic
{
    public:
    Schematic();
    ~Schematic();
    void addElectronicPart(ElectronicPart* part);
    ElectronicPart* addElectronicPart(std::string name, std::string library, std::string deviceset, std::string device, std::string package);
    void addElectronicNet(ElectronicNet* net);
    ElectronicParts* getPartlist();
    void setRootOffset(Pointf3 offset);
    Pointf3 getRootOffset();
    void setFilename(std::string filename);
	private:
    std::vector<ElectronicNet*> netlist;
    ElectronicParts partlist;
    std::string filename;
    Pointf3* rootOffset;

};

}

#endif