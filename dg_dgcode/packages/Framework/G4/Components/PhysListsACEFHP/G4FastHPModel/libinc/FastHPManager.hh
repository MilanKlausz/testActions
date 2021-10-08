
#ifndef FastHPManager_HH
#define FastHPManager_HH

#include <map>
#include "G4Element.hh"
#include "G4PhysicsVector.hh"
#include "G4PhysicsOrderedFreeVector.hh"

class DopplerEffect;
class G4PhysicsTable;
class FastHPManager
{
public:
    static FastHPManager* GetInstance();
    static void dispose();
    virtual ~FastHPManager();


    void initElastic(G4PhysicsTable* );
    void initCapture(G4PhysicsTable* );
    void initInelastic(G4PhysicsTable* );


    G4double GetElasticCrossSection(const G4Element *ele, G4double en, G4double aT);
    G4double GetCaptureCrossSection(const G4Element *ele, G4double en, G4double aT);
    G4double GetInelasticCrossSection(const G4Element *ele, G4double en, G4double aT);

    //G4double GetFissionCrossSection(const G4Element *ele, G4double en, G4double aT);

    bool getElasticDataInitialized() {return m_ela_initialized;};



private:
    int CreateElasticCrossEntry(const G4Element* ele, G4double m_mass_ratio, G4PhysicsVector *vec);
    int CreateCaptureCrossEntry(const G4Element* ele, G4double m_mass_ratio, G4PhysicsVector *vec);
    int CreateInelasticCrossEntry(const G4Element* ele, G4double m_mass_ratio, G4PhysicsVector *vec);
    //int CreateFissionCrossEntry(const G4Element* ele, G4PhysicsVector *vec);

    bool m_ela_initialized;
    bool m_cap_initialized;
    bool m_inl_initialized;
    //bool m_fis_initialized;

    G4PhysicsTable* m_ela_table;
    G4PhysicsTable* m_cap_table;
    G4PhysicsTable* m_inl_table;


    FastHPManager();
    static FastHPManager* m_instance;

    std::map<const G4Element* , DopplerEffect* > *m_ela_xs;
    std::map<const G4Element* , DopplerEffect* > *m_cap_xs;
    std::map<const G4Element* , DopplerEffect* > *m_inl_xs;




};

#endif // FastHPManager_HH
