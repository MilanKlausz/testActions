#ifndef CORE_Units_hh
#define CORE_Units_hh

////////////////////////////////////////////////////////////////////////////////////////
// File adapted from CLHEP/Geant4 (Geant4 version 4.9.5.p01)                          //
//                                                                                    //
// Apart from change of namespace, addition of milli eV and Aa and a fix of an ugly   //
// "#define pascal hep_pascal" hack, everything is as in the original file,           //
// including the comments below. Also modified for constexpr.                         //
//                                                                                    //
// This codedump seems sensible since units are very useful also for applications not //
// relying on the huge dependencies of either Geant4 or CLHEP.                        //
//                                                                                    //
// Thomas Kittelmann Sep 2012                                                         //
////////////////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------
// HEP coherent system of Units
//
// This file has been provided to CLHEP by Geant4 (simulation toolkit for HEP).
//
// The basic units are :
// millimeter              (millimeter)
// nanosecond              (nanosecond)
// Mega electron Volt      (MeV)
// positron charge         (eplus)
// degree Kelvin           (kelvin)
// the amount of substance (mole)
// luminous intensity      (candela)
// radian                  (radian)
// steradian               (steradian)
//
// Below is a non exhaustive list of derived and pratical units
// (i.e. mostly the SI units).
// You can add your own units.
//
// The SI numerical value of the positron charge is defined here,
// as it is needed for conversion factor : positron charge = e_SI (coulomb)
//
// The others physical constants are defined in the header file :
//PhysicalConstants.h
//
// Authors: M.Maire, S.Giani
//
// History:
//
// 06.02.96   Created.
// 28.03.96   Added miscellaneous constants.
// 05.12.97   E.Tcherniaev: Redefined pascal (to avoid warnings on WinNT)
// 20.05.98   names: meter, second, gram, radian, degree
//            (from Brian.Lasiuk@yale.edu (STAR)). Added luminous units.
// 05.08.98   angstrom, picobarn, microsecond, picosecond, petaelectronvolt
// 01.03.01   parsec
// 31.01.06   kilogray, milligray, microgray
// 29.04.08   use PDG 2006 value of e_SI
// 03.11.08   use PDG 2008 value of e_SI

#include "Core/_tempdetectbadcxx11.hh"
#ifdef NEED_CXX11_WORKAROUNDS
#  define _constexpr_ const
#else
#  define _constexpr_ constexpr
#endif

namespace Units {

  //
  // Length [L]
  //
  static _constexpr_ double millimeter  = 1.;
  static _constexpr_ double millimeter2 = millimeter*millimeter;
  static _constexpr_ double millimeter3 = millimeter*millimeter*millimeter;

  static _constexpr_ double centimeter  = 10.*millimeter;
  static _constexpr_ double centimeter2 = centimeter*centimeter;
  static _constexpr_ double centimeter3 = centimeter*centimeter*centimeter;

  static _constexpr_ double meter  = 1000.*millimeter;
  static _constexpr_ double meter2 = meter*meter;
  static _constexpr_ double meter3 = meter*meter*meter;

  static _constexpr_ double kilometer = 1000.*meter;
  static _constexpr_ double kilometer2 = kilometer*kilometer;
  static _constexpr_ double kilometer3 = kilometer*kilometer*kilometer;

  static _constexpr_ double parsec = 3.0856775807e+16*meter;

  static _constexpr_ double micrometer = 1.e-6 *meter;
  static _constexpr_ double  nanometer = 1.e-9 *meter;
  static _constexpr_ double  angstrom  = 1.e-10*meter;
  static _constexpr_ double  Aa  = angstrom;
  static _constexpr_ double  fermi     = 1.e-15*meter;

  static _constexpr_ double      barn = 1.e-28*meter2;
  static _constexpr_ double millibarn = 1.e-3 *barn;
  static _constexpr_ double microbarn = 1.e-6 *barn;
  static _constexpr_ double  nanobarn = 1.e-9 *barn;
  static _constexpr_ double  picobarn = 1.e-12*barn;

  // symbols
  static _constexpr_ double nm  = nanometer;
  static _constexpr_ double um  = micrometer;

  static _constexpr_ double mm  = millimeter;
  static _constexpr_ double mm2 = millimeter2;
  static _constexpr_ double mm3 = millimeter3;

  static _constexpr_ double cm  = centimeter;
  static _constexpr_ double cm2 = centimeter2;
  static _constexpr_ double cm3 = centimeter3;

  static _constexpr_ double m  = meter;
  static _constexpr_ double m2 = meter2;
  static _constexpr_ double m3 = meter3;

  static _constexpr_ double km  = kilometer;
  static _constexpr_ double km2 = kilometer2;
  static _constexpr_ double km3 = kilometer3;

  static _constexpr_ double pc = parsec;

  //
  // Angle
  //
  static _constexpr_ double radian      = 1.;
  static _constexpr_ double milliradian = 1.e-3*radian;
  static _constexpr_ double degree = (3.14159265358979323846/180.0)*radian;

  static _constexpr_ double   steradian = 1.;

  // symbols
  static _constexpr_ double rad  = radian;
  static _constexpr_ double mrad = milliradian;
  static _constexpr_ double sr   = steradian;
  static _constexpr_ double deg  = degree;

  //
  // Time [T]
  //
  static _constexpr_ double nanosecond  = 1.;
  static _constexpr_ double second      = 1.e+9 *nanosecond;
  static _constexpr_ double millisecond = 1.e-3 *second;
  static _constexpr_ double microsecond = 1.e-6 *second;
  static _constexpr_ double  picosecond = 1.e-12*second;

  static _constexpr_ double hertz = 1./second;
  static _constexpr_ double kilohertz = 1.e+3*hertz;
  static _constexpr_ double megahertz = 1.e+6*hertz;

  // symbols
  static _constexpr_ double ns = nanosecond;
  static _constexpr_ double  s = second;
  static _constexpr_ double ms = millisecond;

  //
  // Electric charge [Q]
  //
  static _constexpr_ double eplus = 1. ;// positron charge
  static _constexpr_ double e_SI  = 1.602176487e-19;// positron charge in coulomb
  static _constexpr_ double coulomb = eplus/e_SI;// coulomb = 6.24150 e+18 * eplus

  //
  // Energy [E]
  //
  static _constexpr_ double megaelectronvolt = 1. ;
  static _constexpr_ double millielectronvolt = 1.e-9*megaelectronvolt;
  static _constexpr_ double     electronvolt = 1.e-6*megaelectronvolt;
  static _constexpr_ double kiloelectronvolt = 1.e-3*megaelectronvolt;
  static _constexpr_ double gigaelectronvolt = 1.e+3*megaelectronvolt;
  static _constexpr_ double teraelectronvolt = 1.e+6*megaelectronvolt;
  static _constexpr_ double petaelectronvolt = 1.e+9*megaelectronvolt;

  static _constexpr_ double joule = electronvolt/e_SI;// joule = 6.24150 e+12 * MeV

  // symbols
  static _constexpr_ double MeV = megaelectronvolt;
  static _constexpr_ double meV = millielectronvolt;
  static _constexpr_ double  eV = electronvolt;
  static _constexpr_ double keV = kiloelectronvolt;
  static _constexpr_ double GeV = gigaelectronvolt;
  static _constexpr_ double TeV = teraelectronvolt;
  static _constexpr_ double PeV = petaelectronvolt;

  //
  // Mass [E][T^2][L^-2]
  //
  static _constexpr_ double  kilogram = joule*second*second/(meter*meter);
  static _constexpr_ double      gram = 1.e-3*kilogram;
  static _constexpr_ double milligram = 1.e-3*gram;

  // symbols
  static _constexpr_ double  kg = kilogram;
  static _constexpr_ double   g = gram;
  static _constexpr_ double  mg = milligram;

  //
  // Power [E][T^-1]
  //
  static _constexpr_ double watt = joule/second;// watt = 6.24150 e+3 * MeV/ns

  //
  // Force [E][L^-1]
  //
  static _constexpr_ double newton = joule/meter;// newton = 6.24150 e+9 * MeV/mm

  //
  // Pressure [E][L^-3]
  //
  //[Note from Thomas Kittelmann: For whatever historical reason pascal was
  // named hep_pascal in CLHEP and mapped to pascal with a #define]
  static _constexpr_ double pascal = newton/m2;   // pascal = 6.24150 e+3 * MeV/mm3
  static _constexpr_ double bar        = 100000*pascal; // bar    = 6.24150 e+8 * MeV/mm3
  static _constexpr_ double atmosphere = 101325*pascal; // atm    = 6.32420 e+8 * MeV/mm3

  //
  // Electric current [Q][T^-1]
  //
  static _constexpr_ double      ampere = coulomb/second; // ampere = 6.24150 e+9 * eplus/ns
  static _constexpr_ double milliampere = 1.e-3*ampere;
  static _constexpr_ double microampere = 1.e-6*ampere;
  static _constexpr_ double  nanoampere = 1.e-9*ampere;

  //
  // Electric potential [E][Q^-1]
  //
  static _constexpr_ double megavolt = megaelectronvolt/eplus;
  static _constexpr_ double kilovolt = 1.e-3*megavolt;
  static _constexpr_ double     volt = 1.e-6*megavolt;

  //
  // Electric resistance [E][T][Q^-2]
  //
  static _constexpr_ double ohm = volt/ampere;// ohm = 1.60217e-16*(MeV/eplus)/(eplus/ns)

  //
  // Electric capacitance [Q^2][E^-1]
  //
  static _constexpr_ double farad = coulomb/volt;// farad = 6.24150e+24 * eplus/Megavolt
  static _constexpr_ double millifarad = 1.e-3*farad;
  static _constexpr_ double microfarad = 1.e-6*farad;
  static _constexpr_ double  nanofarad = 1.e-9*farad;
  static _constexpr_ double  picofarad = 1.e-12*farad;

  //
  // Magnetic Flux [T][E][Q^-1]
  //
  static _constexpr_ double weber = volt*second;// weber = 1000*megavolt*ns

  //
  // Magnetic Field [T][E][Q^-1][L^-2]
  //
  static _constexpr_ double tesla     = volt*second/meter2;// tesla =0.001*megavolt*ns/mm2

  static _constexpr_ double gauss     = 1.e-4*tesla;
  static _constexpr_ double kilogauss = 1.e-1*tesla;

  //
  // Inductance [T^2][E][Q^-2]
  //
  static _constexpr_ double henry = weber/ampere;// henry = 1.60217e-7*MeV*(ns/eplus)**2

  //
  // Temperature
  //
  static _constexpr_ double kelvin = 1.;

  //
  // Amount of substance
  //
  static _constexpr_ double mole = 1.;

  //
  // Activity [T^-1]
  //
  static _constexpr_ double becquerel = 1./second ;
  static _constexpr_ double curie = 3.7e+10 * becquerel;

  //
  // Absorbed dose [L^2][T^-2]
  //
  static _constexpr_ double      gray = joule/kilogram ;
  static _constexpr_ double  kilogray = 1.e+3*gray;
  static _constexpr_ double milligray = 1.e-3*gray;
  static _constexpr_ double microgray = 1.e-6*gray;

  //
  // Luminous intensity [I]
  //
  static _constexpr_ double candela = 1.;

  //
  // Luminous flux [I]
  //
  static _constexpr_ double lumen = candela*steradian;

  //
  // Illuminance [I][L^-2]
  //
  static _constexpr_ double lux = lumen/meter2;

  //
  // Miscellaneous
  //
  static _constexpr_ double perCent     = 0.01 ;
  static _constexpr_ double perThousand = 0.001;
  static _constexpr_ double perMillion  = 0.000001;

}  // namespace Units

#undef _constexpr_

#endif
