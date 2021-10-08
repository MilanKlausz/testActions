#ifndef CORE_Constants_hh
#define CORE_Constants_hh

////////////////////////////////////////////////////////////////////////////////////////
// File adapted from CLHEP/Geant4 (Geant4 version 4.9.5.p01)                          //
//                                                                                    //
// Apart from change of namespace and addition of nuclearMagneton, everything is as   //
// in the original file, including the comments below.                                //
//                                                                                    //
// This codedump seems sensible since units are very useful also for applications not //
// relying on the huge dependencies of either Geant4 or CLHEP.                        //
//                                                                                    //
// Thomas Kittelmann Sep 2012                                                         //
////////////////////////////////////////////////////////////////////////////////////////

#include "Core/Units.hh"

#include "Core/_tempdetectbadcxx11.hh"
#ifdef NEED_CXX11_WORKAROUNDS
#  define _constexpr_ const
#else
#  define _constexpr_ constexpr
#endif

namespace Constants {

// -*- C++ -*-
// $Id:$
// ----------------------------------------------------------------------
// HEP coherent Physical Constants
//
// This file has been provided by Geant4 (simulation toolkit for HEP).
//
// The basic units are :
//  		millimeter
// 		nanosecond
// 		Mega electron Volt
// 		positon charge
// 		degree Kelvin
//              amount of substance (mole)
//              luminous intensity (candela)
// 		radian
//              steradian
//
// Below is a non exhaustive list of Physical CONSTANTS,
// computed in the Internal HEP System Of Units.
//
// Most of them are extracted from the Particle Data Book :
//        Phys. Rev. D  volume 50 3-1 (1994) page 1233
//
//        ...with a meaningful (?) name ...
//
// You can add your own constants.
//
// Author: M.Maire
//
// History:
//
// 23.02.96 Created
// 26.03.96 Added constants for standard conditions of temperature
//          and pressure; also added Gas threshold.
// 29.04.08   use PDG 2006 values
// 03.11.08   use PDG 2008 values

//
//
//
static _constexpr_ double     pi  = 3.14159265358979323846;
static _constexpr_ double  twopi  = 2*pi;
static _constexpr_ double halfpi  = pi/2;
static _constexpr_ double     pi2 = pi*pi;

//
//
//
static _constexpr_ double Avogadro = 6.02214179e+23/Units::mole;

//
// c   = 299.792458 mm/ns
// c^2 = 898.7404 (mm/ns)^2
//
static _constexpr_ double c_light   = 2.99792458e+8 * Units::m/Units::s;
static _constexpr_ double c_squared = c_light * c_light;

//
// h     = 4.13566e-12 MeV*ns
// hbar  = 6.58212e-13 MeV*ns
// hbarc = 197.32705e-12 MeV*mm
//
static _constexpr_ double h_Planck      = 6.62606896e-34 * Units::joule*Units::s;
static _constexpr_ double hbar_Planck   = h_Planck/twopi;
static _constexpr_ double hbarc         = hbar_Planck * c_light;
static _constexpr_ double hbarc_squared = hbarc * hbarc;

//
//
//
static _constexpr_ double electron_charge = - Units::eplus; // see SystemOfUnits.h
static _constexpr_ double e_squared = Units::eplus * Units::eplus;

//
// amu_c2 - atomic equivalent mass unit
//        - AKA, unified atomic mass unit (u)
// amu    - atomic mass unit
//
static _constexpr_ double electron_mass_c2 = 0.510998910 * Units::MeV;
static _constexpr_ double   proton_mass_c2 = 938.272013 * Units::MeV;
static _constexpr_ double  neutron_mass_c2 = 939.56536 * Units::MeV;
static _constexpr_ double           amu_c2 = 931.494028 * Units::MeV;
static _constexpr_ double              amu = amu_c2/c_squared;

//
// permeability of free space mu0    = 2.01334e-16 Mev*(ns*eplus)^2/mm
// permittivity of free space epsil0 = 5.52636e+10 eplus^2/(MeV*mm)
//
static _constexpr_ double mu0      = 4*pi*1.e-7 * Units::henry/Units::m;
static _constexpr_ double epsilon0 = 1./(c_squared*mu0);

//
// electromagnetic coupling = 1.43996e-12 MeV*mm/(eplus^2)
//
static _constexpr_ double elm_coupling           = e_squared/(4*pi*epsilon0);
static _constexpr_ double fine_structure_const   = elm_coupling/hbarc;
static _constexpr_ double classic_electr_radius  = elm_coupling/electron_mass_c2;
static _constexpr_ double electron_Compton_length = hbarc/electron_mass_c2;
static _constexpr_ double Bohr_radius = electron_Compton_length/fine_structure_const;

static _constexpr_ double alpha_rcl2 = fine_structure_const
                                   *classic_electr_radius
                                   *classic_electr_radius;

static _constexpr_ double twopi_mc2_rcl2 = twopi*electron_mass_c2
                                             *classic_electr_radius
                                             *classic_electr_radius;
//
//
//
static _constexpr_ double k_Boltzmann = 8.617343e-11 * Units::MeV/Units::kelvin;

//
//
//
static _constexpr_ double STP_Temperature = 273.15*Units::kelvin;
static _constexpr_ double STP_Pressure    = 1.*Units::atmosphere;
static _constexpr_ double kGasThreshold   = 10.*Units::mg/Units::cm3;

//
//
//
static _constexpr_ double universe_mean_density = 1.e-25*Units::g/Units::cm3;

static _constexpr_ double nuclearMagneton = Units::eplus*hbar_Planck/2./(proton_mass_c2 /c_squared);

}  // namespace Constants

#undef _constexpr_

#endif
