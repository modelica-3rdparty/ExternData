# ExternData
Free Modelica library for data I/O of INI, JSON, MATLAB MAT, Excel XLS/XLSX and XML files.

## Build status
[![Build Status](https://drone.io/github.com/tbeu/ExternData/status.png)](https://drone.io/github.com/tbeu/ExternData/latest)
[![Build Status](https://travis-ci.org/tbeu/ExternData.svg?branch=master)](https://travis-ci.org/tbeu/ExternData)
[![Build Status](https://ci.appveyor.com/api/projects/status/k77hnpxp99djcong/branch/master?svg=true)](https://ci.appveyor.com/project/tbeu/externdata/branch/master)

## Library description
ExternData is a utility library to access data stored in INI, JSON, MATLAB MAT, Excel XLS/XLSX or XML files.
The aim of this library is to provide access from Modelica simulation tools to data sets
for convenient model initialization and parametrization.

### Main features
* Read support of file formats
  * [INI](https://en.wikipedia.org/wiki/INI_file)
  * [JSON](https://en.wikipedia.org/wiki/JSON)
  * [MATLAB](https://en.wikipedia.org/wiki/MATLAB) MAT of version v4, v6, v7 and v7.3
  * [Excel](https://en.wikipedia.org/wiki/Microsoft_Excel) [XLS](https://en.wikipedia.org/wiki/Microsoft_Excel#Binary) and [XLSX](https://en.wikipedia.org/wiki/Microsoft_Excel#XML_Spreadsheet)
  * [XML](https://en.wikipedia.org/wiki/XML)
* Pure C (and not C++) code for external functions and objects
* Cross-platform (Windows and Linux)
* Tested in Dymola and SimulationX, with dependency on the Modelica Standard Library 3.2.1.

All data I/O access is implemented using external Modelica functions. Furthermore, high level interfaces
on these functions are provided via Modelica models.

### Known issue with OpenModelica
The example models of the library are known to fail with OpenModelica. However, as a workaround the functions can be rewritten to make them working in OpenModelica by replacing their short class definitions and their appropriate function calls. For example, the short class definition `ExternData.XMLFile.getReal`
```mo
final function getReal = Functions.XML.getReal(xml=xml)
  "Get scalar Real value from XML file";
```
can be rewritten as function
```mo
function __OpenModelica_getReal "Get scalar Real value from XML file"
  extends Modelica.Icons.Function;
  input String fileName="" "File where external data is stored";
  input String varName "Key";
  output Real y "Real value";
  protected
    ExternData.Types.ExternXMLFile xml = ExternData.Types.ExternXMLFile(fileName) "External XML file object";
  algorithm
    y := ExternData.Functions.XML.Internal.getReal(xml=xml, varName=varName);
  annotation(Inline=true);
end __OpenModelica_getReal;
```
taking the file name as explicit argument for the external object.

## License
ExternData is released under the terms of the Simplified BSD License.

## Acknowledgement
ExternData is based on the following third-party C projects
* [bsxml-json](https://github.com/bsapundzhiev/bsxml-json) -
Borislav Sapundzhiev's fairly simple XML DOM and JSON implementation
* [expat](http://sourceforge.net/projects/expat) -
James Clark's Expat XML parser library
* [hdf5](https://support.hdfgroup.org/HDF5) -
The HDF Group's data model, library and file format for storing and managing data
* [libxls](http://sourceforge.net/projects/libxls) -
David Hoerl's C library for parsing Excel files
* [matio](http://sourceforge.net/projects/matio) -
Christopher Hulbert's C library for reading and writing MATLAB MAT-files
* [minIni](https://github.com/compuphase/minIni) -
Thiadmer Riemersma's small and portable INI file library with read/write support
* [minizip](http://www.winimage.com/zLibDll/minizip.html) -
Gilles Vollant's Zip and UnZip library
* [uthash](https://github.com/troydhanson/uthash) -
Troy D. Hanson's C macros for hash tables and more
* [zlib](https://github.com/madler/zlib) -
Jean-loup Gailly's and Mark Adler's massively spiffy yet delicately unobtrusive compression library

## Development and contribution
* [tbeu](https://github.com/tbeu)

You may report any issues with using the [Issues](../../issues) button.

Contributions in shape of [Pull Requests](../../pulls) are always welcome.
