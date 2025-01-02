// CP: 65001
/* package.mo - Modelica library for data I/O of CSV, INI, JSON, MATLAB MAT, SSV, TIR, Excel XLS/XLSX or XML files
 *
 * Copyright (C) 2015-2025, Thomas Beutlich
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

within;
package ExternData "Library for data I/O of CSV, INI, JSON, MATLAB MAT, SSV, TIR, Excel XLS/XLSX or XML files"
  extends Modelica.Icons.Package;
  package UsersGuide "User's Guide"
    extends Modelica.Icons.Information;
    class Overview "Overview"
      extends Modelica.Icons.Information;
      annotation(Documentation(info="<html><p>An overview of the library is provided in</p><p>Thomas Beutlich and Dietmar Winkler. Efficient Parameterization of Modelica Models. In: <em>Proceedings of the 14th International Modelica Conference</em>. Ed. by Martin Sj&ouml;lund, Lena Buffoni, Adrian Pop, and Lennart Ochel. Link&ouml;ping, Sweden, September 2021. DOI: <a href=\"https://doi.org/10.3384/ecp21181141\">10.3384/ecp21181141</a>.</p></html>"));
    end Overview;
    class References "References"
      extends Modelica.Icons.References;
      annotation(Documentation(info="<html><p>The ExternData Modelica library is based on the following third-party C projects</p><ul><li><a href=\"https://github.com/bsapundzhiev/bsxml-json\">bsxml-json</a>&nbsp;-&nbsp;Borislav Sapundzhiev&#039;s fairly simple XML DOM and JSON implementation</li><li><a href=\"https://github.com/libexpat/libexpat\">expat</a>&nbsp;-&nbsp;James Clark&#039;s Expat XML parser library</li><li><a href=\"https://support.hdfgroup.org/HDF5\">hdf5</a>&nbsp;-&nbsp;The HDF Group&#039;s data model, library and file format for storing and managing data</li><li><a href=\"https://github.com/benhoyt/inih\">inih</a>&nbsp;-&nbsp;Ben Hoyt&#039;s simple INI file parser in C</li><li><a href=\"https://github.com/libxls/libxls\">libxls</a>&nbsp;-&nbsp;David Hoerl&#039;s C library for parsing Excel files</li><li><a href=\"https://gitlab.gnome.org/GNOME/libxml2\">libxml2</a>&nbsp;-&nbsp;Daniel Veillard&#039;s XML C parser and toolkit of Gnome</li><li><a href=\"https://sourceforge.net/projects/matio/\">matio</a>&nbsp;-&nbsp;Christopher Hulbert&#039;s C library for reading and writing MATLAB MAT-files</li><li><a href=\"http://www.winimage.com/zLibDll/minizip.html\">minizip</a>&nbsp;-&nbsp;Gilles Vollant&#039;s Zip and UnZip library</li><li><a href=\"https://github.com/kgabis/parson\">parson</a>&nbsp;-&nbsp;Krzysztof Gabis&#039; lightweight JSON library written in C</li><li><a href=\"https://github.com/troydhanson/uthash\">uthash</a>&nbsp;-&nbsp;Troy D. Hanson&#039;s C macros for hash tables and more</li><li><a href=\"https://github.com/madler/zlib\">zlib</a>&nbsp;-&nbsp;Jean-loup Gailly&#039;s and Mark Adler&#039;s massively spiffy yet delicately unobtrusive compression library</li><li><a href=\"https://github.com/fnoyanisi/zString\">zstring</a>&nbsp;-&nbsp;Fehmi Noyan ISI&#039;s string processing library for C</li></ul></html>"));
    end References;
    class License "Simplified BSD License"
      extends Modelica.Icons.Information;
      annotation(Documentation(info="<html><p>Copyright (C) 2015-2025, Thomas Beutlich<br>All rights reserved.</p><p>Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:</p><p>1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.</p><p>2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.</p><p>THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.</p></html>"));
    end License;
    class Contact "Contact"
      extends Modelica.Icons.Contact;
      annotation(Documentation(info="<html><p>The ExternData Modelica library is developed at <a href=\"https://github.com/modelica-3rdparty/ExternData\">GitHub</a>.</p></html>"));
    end Contact;
    annotation(DocumentationClass=true,
      Documentation(info="<html><p>Library <strong>ExternData</strong> is a <a href=\"https://en.wikipedia.org/wiki/Modelica\">Modelica</a> utility library to access data stored in <a href=\"https://en.wikipedia.org/wiki/Comma-separated_values\">CSV</a>, <a href=\"https://en.wikipedia.org/wiki/INI_file\">INI</a>, <a href=\"https://en.wikipedia.org/wiki/JSON\">JSON</a>, <a href=\"https://en.wikipedia.org/wiki/MATLAB\">MATLAB</a> MAT, <a href=\"https://ssp-standard.org/\">SSV</a> (System Structure Parameter Values), TIR, <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel\">Excel</a> <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel#Binary\">XLS</a>/<a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel#XML_Spreadsheet\">XLSX</a> and <a href=\"https://en.wikipedia.org/wiki/XML\">XML</a> files.</p></html>"));
  end UsersGuide;

  record CSVFile "Read data values from CSV file"
    parameter String fileName = "" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="Comma-separated values files (*.csv);;Text files (*.txt)",
        caption="Open file")));
    parameter String delimiter = "," "Column delimiter character" annotation(choices(choice=" " "Blank", choice="," "Comma", choice="\t" "Horizontal tabulator", choice=";" "Semicolon"));
    parameter String quotation = "\"" "Quotation character" annotation(choices(choice="\"" "Double quotation mark", choice="'" "Single quotation mark"));
    parameter Integer nHeaderLines = 0 "Number of header lines to ignore";
    parameter Boolean verboseRead = true "= true, if info message that file is loading is to be printed"
      annotation(Dialog(group="Diagnostics"));
    final parameter Types.ExternCSVFile csv = Types.ExternCSVFile(fileName, delimiter, quotation, nHeaderLines, verboseRead) "External CSV file object";
    extends Interfaces.CSV.Base(
      redeclare final function getRealArray2D = Functions.CSV.getRealArray2D(csv=csv) "Get 2D Real values from CSV file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArraySize2D = Functions.CSV.getArraySize2D(csv=csv) "Get the size of a 2D array in a CSV file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArrayRows2D = Functions.CSV.getArrayRows2D(csv=csv) "Get first dimension of 2D array in CSV file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArrayColumns2D = Functions.CSV.getArrayColumns2D(csv=csv) "Get second dimension of 2D array in CSV file" annotation(Documentation(info="<html></html>")));
    annotation(
      Documentation(info="<html><p>Record that wraps the external object <a href=\"modelica://ExternData.Types.ExternCSVFile\">ExternCSVFile</a> and the <a href=\"modelica://ExternData.Functions.CSV\">CSV</a> read functions for data access of <a href=\"https://en.wikipedia.org/wiki/Comma-separated_values\">CSV</a> files.</p><p>See <a href=\"modelica://ExternData.Examples.CSVTest\">Examples.CSVTest</a> for an example.</p></html>"),
      defaultComponentName="dataSource",
      defaultComponentPrefixes="inner parameter",
      missingInnerMessage="No \"dataSource\" component is defined, please drag ExternData.CSVFile to the model top level",
      Icon(graphics={
        Text(textColor={0,0,255},extent={{-85,-10},{85,-55}},textString=DynamicSelect("csv", if delimiter == " " then "c s v" elseif delimiter == "," then "c,s,v" elseif delimiter == "\t" then "c\\ts\\tv" elseif delimiter == ";" then "c;s;v" else "csv")),
        Text(textColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end CSVFile;

  record INIFile "Read data values from INI file"
    parameter String fileName = "" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="INI files (*.ini);;Configuration files (*.cfg;*.conf;config.txt);;Text files (*.txt)",
        caption="Open file")));
    parameter Boolean verboseRead = true "= true, if info message that file is loading is to be printed"
      annotation(Dialog(group="Diagnostics"));
    parameter Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data"
      annotation(Dialog(group="Diagnostics"));
    final parameter Types.ExternINIFile ini = Types.ExternINIFile(fileName, verboseRead, detectMissingData) "External INI file object";
    extends Interfaces.INI.Base(
      redeclare final function getReal = Functions.INI.getReal(ini=ini, strict=true) "Get scalar Real value from INI file" annotation(Documentation(info="<html></html>")),
      redeclare final function getInteger = Functions.INI.getInteger(ini=ini, strict=true) "Get scalar Integer value from INI file" annotation(Documentation(info="<html></html>")),
      redeclare final function getBoolean = Functions.INI.getBoolean(ini=ini, strict=true) "Get scalar Boolean value from INI file" annotation(Documentation(info="<html></html>")),
      redeclare final function getString = Functions.INI.getString(ini=ini) "Get scalar String value from INI file" annotation(Documentation(info="<html></html>")));
    annotation(
      Documentation(info="<html><p>Record that wraps the external object <a href=\"modelica://ExternData.Types.ExternINIFile\">ExternINIFile</a> and the <a href=\"modelica://ExternData.Functions.INI\">INI</a> read functions for data access of <a href=\"https://en.wikipedia.org/wiki/INI_file\">INI</a> files.</p><p>See <a href=\"modelica://ExternData.Examples.INITest\">Examples.INITest</a> for an example.</p></html>"),
      defaultComponentName="dataSource",
      defaultComponentPrefixes="inner parameter",
      missingInnerMessage="No \"dataSource\" component is defined, please drag ExternData.INIFile to the model top level",
      Icon(graphics={
        Text(textColor={0,0,255},extent={{-85,-10},{85,-55}},textString="[ini]"),
        Text(textColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end INIFile;

  record JSONFile "Read data values from JSON file"
    parameter String fileName = "" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="JSON files (*.json)",
        caption="Open file")));
    parameter Boolean verboseRead = true "= true, if info message that file is loading is to be printed"
      annotation(Dialog(group="Diagnostics"));
    parameter Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data"
      annotation(Dialog(group="Diagnostics"));
    final parameter Types.ExternJSONFile json = Types.ExternJSONFile(fileName, verboseRead, detectMissingData) "External JSON file object";
    extends Interfaces.JSON.Base(
      redeclare final function getReal = Functions.JSON.getReal(json=json) "Get scalar Real value from JSON file" annotation(Documentation(info="<html></html>")),
      redeclare final function getRealArray1D = Functions.JSON.getRealArray1D(json=json) "Get 1D array Real value from JSON file" annotation(Documentation(info="<html></html>")),
      redeclare final function getRealArray2D = Functions.JSON.getRealArray2D(json=json) "Get 2D array Real value from JSON file" annotation(Documentation(info="<html></html>")),
      redeclare final function getInteger = Functions.JSON.getInteger(json=json) "Get scalar Integer value from JSON file" annotation(Documentation(info="<html></html>")),
      redeclare final function getIntegerArray1D = Functions.JSON.getIntegerArray1D(json=json) "Get 1D array Integer value from JSON file" annotation(Documentation(info="<html></html>")),
      redeclare final function getIntegerArray2D = Functions.JSON.getIntegerArray2D(json=json) "Get 2D array Integer value from JSON file" annotation(Documentation(info="<html></html>")),
      redeclare final function getBoolean = Functions.JSON.getBoolean(json=json) "Get scalar Boolean value from JSON file" annotation(Documentation(info="<html></html>")),
      redeclare final function getBooleanArray1D = Functions.JSON.getBooleanArray1D(json=json) "Get 1D array Boolean value from JSON file" annotation(Documentation(info="<html></html>")),
      redeclare final function getBooleanArray2D = Functions.JSON.getBooleanArray2D(json=json) "Get 2D array Boolean value from JSON file" annotation(Documentation(info="<html></html>")),
      redeclare final function getString = Functions.JSON.getString(json=json) "Get scalar String value from JSON file" annotation(Documentation(info="<html></html>")),
      redeclare final function getStringArray1D = Functions.JSON.getStringArray1D(json=json) "Get 1D array String value from JSON file" annotation(Documentation(info="<html></html>")),
      redeclare final function getStringArray2D = Functions.JSON.getStringArray2D(json=json) "Get 2D array String value from JSON file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArraySize1D = Functions.JSON.getArraySize1D(json=json) "Get the size of a 1D array in a JSON file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArraySize2D = Functions.JSON.getArraySize2D(json=json) "Get the size of a 2D array in a JSON file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArrayRows2D = Functions.JSON.getArrayRows2D(json=json) "Get first dimension of 2D array in JSON file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArrayColumns2D = Functions.JSON.getArrayColumns2D(json=json) "Get second dimension of 2D array in JSON file" annotation(Documentation(info="<html></html>")));
    annotation(
      Documentation(info="<html><p>Record that wraps the external object <a href=\"modelica://ExternData.Types.ExternJSONFile\">ExternJSONFile</a> and the <a href=\"modelica://ExternData.Functions.JSON\">JSON</a> read functions for data access of <a href=\"https://en.wikipedia.org/wiki/JSON\">JSON</a> files.</p><p>See <a href=\"modelica://ExternData.Examples.JSONTest\">Examples.JSONTest</a> for an example.</p></html>"),
      defaultComponentName="dataSource",
      defaultComponentPrefixes="inner parameter",
      missingInnerMessage="No \"dataSource\" component is defined, please drag ExternData.JSONFile to the model top level",
      Icon(graphics={
        Text(textColor={0,0,255},extent={{-85,-10},{85,-55}},textString="{\"json\"}"),
        Text(textColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end JSONFile;

  record MATFile "Read data values from MATLAB MAT file"
    parameter String fileName = "" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="MATLAB MAT files (*.mat)",
        caption="Open file")));
    parameter Boolean verboseRead = true "= true, if info message that file is loading is to be printed"
      annotation(Dialog(group="Diagnostics"));
    final parameter Types.ExternMATFile mat = Types.ExternMATFile(fileName, verboseRead) "External MAT file object";
    extends Interfaces.MAT.Base(
      redeclare final function getRealArray2D = Functions.MAT.getRealArray2D(mat=mat) "Get 2D Real values from MAT file" annotation(Documentation(info="<html></html>")),
      redeclare final function getStringArray1D = Functions.MAT.getStringArray1D(mat=mat) "Get 1D String values from MAT file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArraySize2D = Functions.MAT.getArraySize2D(mat=mat) "Get the size of a 2D array in a MAT file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArrayRows2D = Functions.MAT.getArrayRows2D(mat=mat) "Get first dimension of 2D array in MAT file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArrayColumns2D = Functions.MAT.getArrayColumns2D(mat=mat) "Get second dimension of 2D array in MAT file" annotation(Documentation(info="<html></html>")));
    annotation(
      Documentation(info="<html><p>Record that wraps the external object <a href=\"modelica://ExternData.Types.ExternMATFile\">ExternMATFile</a> and the <a href=\"modelica://ExternData.Functions.MAT\">MAT</a> read functions for data access of <a href=\"https://en.wikipedia.org/wiki/MATLAB\">MATLAB</a> MAT files.</p><p>See <a href=\"modelica://ExternData.Examples.MATTest\">Examples.MATTest</a> for an example.</p></html>"),
      defaultComponentName="dataSource",
      defaultComponentPrefixes="inner parameter",
      missingInnerMessage="No \"dataSource\" component is defined, please drag ExternData.MATFile to the model top level",
      Icon(graphics={
        Text(extent={{5,85},{65,40}},textString="mat"),
        Text(textColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end MATFile;

  record SSVFile "Read data values from SSV file"
    parameter String fileName = "" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="SSV files (*.ssv;*.xml)",
        caption="Open file")));
    parameter String nameSpace = "http://ssp-standard.org/SSP1/SystemStructureParameterValues" "SSV name space" annotation(choices(choice="" "No name space", choice="http://ssp-standard.org/SSP1/SystemStructureParameterValues" "SSP 1.0"));
    parameter Boolean verboseRead = true "= true, if info message that file is loading is to be printed"
      annotation(Dialog(group="Diagnostics"));
    parameter Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data"
      annotation(Dialog(group="Diagnostics"));
    final parameter Types.ExternXML2File ssv = Types.ExternXML2File(fileName, if nameSpace == "" then fill("", 0, 2) else {{"ssv", nameSpace}}, verboseRead, detectMissingData) "External SSV file object";
    extends Interfaces.SSV.Base(
      redeclare final function getReal = Functions.SSV.getReal(ssv=ssv, nameSpace=nameSpace) "Get scalar Real value from SSV file" annotation(Documentation(info="<html></html>")),
      redeclare final function getInteger = Functions.SSV.getInteger(ssv=ssv, nameSpace=nameSpace) "Get scalar Integer value from SSV file" annotation(Documentation(info="<html></html>")),
      redeclare final function getBoolean = Functions.SSV.getBoolean(ssv=ssv, nameSpace=nameSpace) "Get scalar Boolean value from SSV file" annotation(Documentation(info="<html></html>")),
      redeclare final function getString = Functions.SSV.getString(ssv=ssv, nameSpace=nameSpace) "Get scalar String value from SSV file" annotation(Documentation(info="<html></html>")));
    annotation(
      Documentation(info="<html><p>Record that wraps the external object <a href=\"modelica://ExternData.Types.ExternXML2File\">ExternXML2File</a> and the <a href=\"modelica://ExternData.Functions.SSV\">SSV</a> read functions for data access of System Structure Parameter Values.</p><p>See <a href=\"modelica://ExternData.Examples.SSVTest\">Examples.SSVTest</a> for an example.</p></html>"),
      defaultComponentName="dataSource",
      defaultComponentPrefixes="inner parameter",
      missingInnerMessage="No \"dataSource\" component is defined, please drag ExternData.SSVFile to the model top level",
      Icon(graphics={
        Text(textColor={255,128,0},extent={{-85,-10},{85,-55}},textString="<ssv>"),
        Text(textColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end SSVFile;

  record XLSFile "Read data values from Excel XLS file"
    parameter String fileName = "" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="Excel files (*.xls)",
        caption="Open file")));
    parameter String encoding = "UTF-8" "Encoding";
    parameter Boolean verboseRead = true "= true, if info message that file is loading is to be printed"
      annotation(Dialog(group="Diagnostics"));
    parameter Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data"
      annotation(Dialog(group="Diagnostics"));
    final parameter Types.ExternXLSFile xls = Types.ExternXLSFile(fileName, encoding, verboseRead, detectMissingData) "External Excel XLS file object";
    extends Interfaces.XLS.Base(
      redeclare final function getReal = Functions.XLS.getReal(xls=xls) "Get scalar Real value from Excel XLS file" annotation(Documentation(info="<html></html>")),
      redeclare final function getRealArray2D = Functions.XLS.getRealArray2D(xls=xls) "Get 2D Real values from Excel XLS file" annotation(Documentation(info="<html></html>")),
      redeclare final function getInteger = Functions.XLS.getInteger(xls=xls) "Get scalar Integer value from Excel XLS file" annotation(Documentation(info="<html></html>")),
      redeclare final function getBoolean = Functions.XLS.getBoolean(xls=xls) "Get scalar Boolean value from Excel XLS file" annotation(Documentation(info="<html></html>")),
      redeclare final function getString = Functions.XLS.getString(xls=xls) "Get scalar String value from Excel XLS file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArraySize2D = Functions.XLS.getArraySize2D(xls=xls) "Get the size of a 2D array in a Excel XLS file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArrayRows2D = Functions.XLS.getArrayRows2D(xls=xls) "Get first dimension of 2D array in Excel XLS file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArrayColumns2D = Functions.XLS.getArrayColumns2D(xls=xls) "Get second dimension of 2D array in Excel XLS file" annotation(Documentation(info="<html></html>")));
    annotation(
      Documentation(info="<html><p>Record that wraps the external object <a href=\"modelica://ExternData.Types.ExternXLSFile\">ExternXLSFile</a> and the <a href=\"modelica://ExternData.Functions.XLS\">XLS</a> read functions for data access of <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel\">Excel</a> <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel#Binary\">XLS</a> files.</p><p>See <a href=\"modelica://ExternData.Examples.XLSTest\">Examples.XLSTest</a> for an example.</p></html>"),
      defaultComponentName="dataSource",
      defaultComponentPrefixes="inner parameter",
      missingInnerMessage="No \"dataSource\" component is defined, please drag ExternData.XLSFile to the model top level",
      Icon(graphics={
        Text(textColor={0,127,0},extent={{-85,-10},{85,-55}},textString="xls"),
        Text(textColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end XLSFile;

  record XLSXFile "Read data values from Excel XLSX file"
    parameter String fileName = "" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="Excel files (*.xlsx)",
        caption="Open file")));
    parameter Boolean verboseRead = true "= true, if info message that file is loading is to be printed"
      annotation(Dialog(group="Diagnostics"));
    parameter Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data"
      annotation(Dialog(group="Diagnostics"));
    final parameter Types.ExternXLSXFile xlsx = Types.ExternXLSXFile(fileName, verboseRead, detectMissingData) "External Excel XLSX file object";
    extends Interfaces.XLSX.Base(
      redeclare final function getReal = Functions.XLSX.getReal(xlsx=xlsx) "Get scalar Real value from Excel XLSX file" annotation(Documentation(info="<html></html>")),
      redeclare final function getRealArray2D = Functions.XLSX.getRealArray2D(xlsx=xlsx) "Get 2D Real values from Excel XLSX file" annotation(Documentation(info="<html></html>")),
      redeclare final function getInteger = Functions.XLSX.getInteger(xlsx=xlsx) "Get scalar Integer value from Excel XLSX file" annotation(Documentation(info="<html></html>")),
      redeclare final function getBoolean = Functions.XLSX.getBoolean(xlsx=xlsx) "Get scalar Boolean value from Excel XLSX file" annotation(Documentation(info="<html></html>")),
      redeclare final function getString = Functions.XLSX.getString(xlsx=xlsx) "Get scalar String value from Excel XLSX file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArraySize2D = Functions.XLSX.getArraySize2D(xlsx=xlsx) "Get the size of a 2D array in a Excel XLSX file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArrayRows2D = Functions.XLSX.getArrayRows2D(xlsx=xlsx) "Get first dimension of 2D array in Excel XLSX file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArrayColumns2D = Functions.XLSX.getArrayColumns2D(xlsx=xlsx) "Get second dimension of 2D array in Excel XLSX file" annotation(Documentation(info="<html></html>")));
    annotation(
      Documentation(info="<html><p>Record that wraps the external object <a href=\"modelica://ExternData.Types.ExternXLSXFile\">ExternXLSXFile</a> and the <a href=\"modelica://ExternData.Functions.XLSX\">XLSX</a> read functions for data access of <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel\">Excel</a> <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel#XML_Spreadsheet\">XLSX</a> files.</p><p>See <a href=\"modelica://ExternData.Examples.XLSXTest\">Examples.XLSXTest</a> for an example.</p></html>"),
      defaultComponentName="dataSource",
      defaultComponentPrefixes="inner parameter",
      missingInnerMessage="No \"dataSource\" component is defined, please drag ExternData.XLSXFile to the model top level",
      Icon(graphics={
        Text(textColor={0,127,255},extent={{-85,-10},{85,-55}},textString="xlsx"),
        Text(textColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end XLSXFile;

  record XMLFile "Read data values from XML file"
    parameter String fileName = "" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="XML files (*.xml)",
        caption="Open file")));
    parameter Boolean verboseRead = true "= true, if info message that file is loading is to be printed"
      annotation(Dialog(group="Diagnostics"));
    parameter Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data"
      annotation(Dialog(group="Diagnostics"));
    final parameter Types.ExternXMLFile xml = Types.ExternXMLFile(fileName, verboseRead, detectMissingData) "External XML file object";
    extends Interfaces.XML.Base(
      redeclare final function getReal = Functions.XML.getReal(xml=xml) "Get scalar Real value from XML file" annotation(Documentation(info="<html></html>")),
      redeclare final function getRealArray1D = Functions.XML.getRealArray1D(xml=xml) "Get 1D Real values from XML file" annotation(Documentation(info="<html></html>")),
      redeclare final function getRealArray2D = Functions.XML.getRealArray2D(xml=xml) "Get 2D Real values from XML file" annotation(Documentation(info="<html></html>")),
      redeclare final function getInteger = Functions.XML.getInteger(xml=xml) "Get scalar Integer value from XML file" annotation(Documentation(info="<html></html>")),
      redeclare final function getBoolean = Functions.XML.getBoolean(xml=xml) "Get scalar Boolean value from XML file" annotation(Documentation(info="<html></html>")),
      redeclare final function getString = Functions.XML.getString(xml=xml) "Get scalar String value from XML file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArraySize1D = Functions.XML.getArraySize1D(xml=xml) "Get the size of a 1D array in a XML file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArraySize2D = Functions.XML.getArraySize2D(xml=xml) "Get the size of a 2D array in a XML file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArrayRows2D = Functions.XML.getArrayRows2D(xml=xml) "Get first dimension of 2D array in XML file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArrayColumns2D = Functions.XML.getArrayColumns2D(xml=xml) "Get second dimension of 2D array in XML file" annotation(Documentation(info="<html></html>")));
    annotation(
      Documentation(info="<html><p>Record that wraps the external object <a href=\"modelica://ExternData.Types.ExternXMLFile\">ExternXMLFile</a> and the <a href=\"modelica://ExternData.Functions.XML\">XML</a> read functions for data access of <a href=\"https://en.wikipedia.org/wiki/XML\">XML</a> files.</p><p>See <a href=\"modelica://ExternData.Examples.XMLTest\">Examples.XMLTest</a> or <a href=\"modelica://ExternData.Examples.XMLTest2\">Examples.XMLTest2</a> for an example.</p></html>"),
      defaultComponentName="dataSource",
      defaultComponentPrefixes="inner parameter",
      missingInnerMessage="No \"dataSource\" component is defined, please drag ExternData.XMLFile to the model top level",
      Icon(graphics={
        Text(textColor={255,128,0},extent={{-85,-10},{85,-55}},textString="<?xml?>"),
        Text(textColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end XMLFile;

  record XML2File "Read data values from XML file (utilizing XPath expressions)"
    parameter String fileName = "" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="XML files (*.xml)",
        caption="Open file")));
    parameter String nameSpace[:,2] = fill("", 0, 2) "XML name spaces (prefix = first column; e.g., nameSpace = [\"prefix\", \"url\"])";
    parameter Boolean verboseRead = true "= true, if info message that file is loading is to be printed"
      annotation(Dialog(group="Diagnostics"));
    parameter Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data"
      annotation(Dialog(group="Diagnostics"));
    final parameter Types.ExternXML2File xml = Types.ExternXML2File(fileName, nameSpace, verboseRead, detectMissingData) "External XML2 file object";
    extends Interfaces.XML2.Base(
      redeclare final function getReal = Functions.XML2.getReal(xml=xml) "Get scalar Real value from XML file" annotation(Documentation(info="<html></html>")),
      redeclare final function getRealArray1D = Functions.XML2.getRealArray1D(xml=xml) "Get 1D Real values from XML file" annotation(Documentation(info="<html></html>")),
      redeclare final function getRealArray2D = Functions.XML2.getRealArray2D(xml=xml) "Get 2D Real values from XML file" annotation(Documentation(info="<html></html>")),
      redeclare final function getInteger = Functions.XML2.getInteger(xml=xml) "Get scalar Integer value from XML file" annotation(Documentation(info="<html></html>")),
      redeclare final function getBoolean = Functions.XML2.getBoolean(xml=xml) "Get scalar Boolean value from XML file" annotation(Documentation(info="<html></html>")),
      redeclare final function getString = Functions.XML2.getString(xml=xml) "Get scalar String value from XML file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArraySize1D = Functions.XML2.getArraySize1D(xml=xml) "Get the size of a 1D array in a XML file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArraySize2D = Functions.XML2.getArraySize2D(xml=xml) "Get the size of a 2D array in a XML file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArrayRows2D = Functions.XML2.getArrayRows2D(xml=xml) "Get first dimension of 2D array in XML file" annotation(Documentation(info="<html></html>")),
      redeclare final function getArrayColumns2D = Functions.XML2.getArrayColumns2D(xml=xml) "Get second dimension of 2D array in XML file" annotation(Documentation(info="<html></html>")));
    annotation(
      Documentation(info="<html><p>Record that wraps the external object <a href=\"modelica://ExternData.Types.ExternXML2File\">ExternXML2File</a> and the <a href=\"modelica://ExternData.Functions.XML2\">XML2</a> read functions for data access of <a href=\"https://en.wikipedia.org/wiki/XML\">XML</a> files.</p><p>See <a href=\"modelica://ExternData.Examples.XML2Test\">Examples.XML2Test</a> for an example.</p></html>"),
      defaultComponentName="dataSource",
      defaultComponentPrefixes="inner parameter",
      missingInnerMessage="No \"dataSource\" component is defined, please drag ExternData.XML2File to the model top level",
      Icon(graphics={
        Text(textColor={255,128,0},extent={{-85,-10},{85,-55}},textString="<?xml?>"),
        Text(textColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end XML2File;

  record TIRFile "Read data values from TIR file"
    parameter String fileName = "" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="TIR files (*.tir)",
        caption="Open file")));
    parameter Boolean verboseRead = true "= true, if info message that file is loading is to be printed"
      annotation(Dialog(group="Diagnostics"));
    parameter Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data"
      annotation(Dialog(group="Diagnostics"));
    final parameter Types.ExternINIFile tir = Types.ExternINIFile(fileName, verboseRead, detectMissingData) "External TIR file object";
    extends Interfaces.INI.Base(
      redeclare final function getReal = Functions.INI.getReal(ini=tir, strict=false) "Get scalar Real value from TIR file" annotation(Documentation(info="<html></html>")),
      redeclare final function getInteger = Functions.INI.getInteger(ini=tir, strict=false) "Get scalar Integer value from TIR file" annotation(Documentation(info="<html></html>")),
      redeclare final function getBoolean = Functions.INI.getBoolean(ini=tir, strict=false) "Get scalar Boolean value from TIR file" annotation(Documentation(info="<html></html>")),
      redeclare final function getString = Functions.INI.getString(ini=tir) "Get scalar String value from TIR file" annotation(Documentation(info="<html></html>")));
    annotation(
      Documentation(info="<html><p>Record that wraps the external object <a href=\"modelica://ExternData.Types.ExternINIFile\">ExternINIFile</a> and the <a href=\"modelica://ExternData.Functions.INI\">INI</a> read functions for data access of tyre data (TIR) files.</p><p>See <a href=\"modelica://ExternData.Examples.TIRTest\">Examples.TIRTest</a> for an example.</p></html>"),
      defaultComponentName="dataSource",
      defaultComponentPrefixes="inner parameter",
      missingInnerMessage="No \"dataSource\" component is defined, please drag ExternData.TIRFile to the model top level",
      Icon(graphics={
        Text(textColor={0,0,255},extent={{-36,84},{88,40}},textString="[tir]"),
        Text(textColor={0,0,255},extent={{-150,150},{150,110}},textString="%name"),
        Ellipse(extent={{-50,20},{50,-80}},fillPattern=FillPattern.Solid),
        Ellipse(extent={{-30,0},{30,-60}},fillColor={255,255,255},fillPattern=FillPattern.Solid)}));
  end TIRFile;

  package Functions "Functions"
    extends Modelica.Icons.FunctionsPackage;

    package CSV "CSV file functions"
      extends Modelica.Icons.FunctionsPackage;
      pure function getRealArray2D "Get 2D Real values from CSV file"
        extends Interfaces.CSV.getRealArray2D;
        external "C" ED_getDoubleArray2DFromCSV(csv, field, y, size(y, 1), size(y, 2)) annotation(
          __iti_dll = "ITI_ED_CSVFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_CSVFile.h\"",
          Library = {"ED_CSVFile", "bsxml-json"});
      end getRealArray2D;

      pure function getArraySize2D "Get dimensions of 2D array in CSV file"
        extends Interfaces.CSV.getArraySize2D;
        external "C" ED_getArray2DDimensionsFromCSV(csv, m, n) annotation(
          __iti_dll = "ITI_ED_CSVFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_CSVFile.h\"",
          Library = {"ED_CSVFile", "bsxml-json"});
      end getArraySize2D;

      pure function getArrayRows2D "Get first dimension of 2D array in CSV file"
        extends Interfaces.CSV.getArrayRows2D;
        external "C" ED_getArray2DDimensionsFromCSV(csv, m, n) annotation(
          __iti_dll = "ITI_ED_CSVFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_CSVFile.h\"",
          Library = {"ED_CSVFile", "bsxml-json"});
      end getArrayRows2D;

      pure function getArrayColumns2D "Get second dimension of 2D array in CSV file"
        extends Interfaces.CSV.getArrayColumns2D;
        external "C" ED_getArray2DDimensionsFromCSV(csv, m, n) annotation(
          __iti_dll = "ITI_ED_CSVFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_CSVFile.h\"",
          Library = {"ED_CSVFile", "bsxml-json"});
      end getArrayColumns2D;

      function readArraySize2D "Read dimensions of 2D array in CSV file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String delimiter = "," "Column delimiter character";
        input String quotation = "\"" "Quotation character";
        input Integer nHeaderLines = 0 "Number of header lines to ignore";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        output Integer m "Number of rows in array";
        output Integer n "Number of columns in array";
        protected
          Types.ExternCSVFile csv = Types.ExternCSVFile(fileName=fileName, delimiter=delimiter, quotation=quotation, nHeaderLines=nHeaderLines, verboseRead=verboseRead) "External CSV file object";
        algorithm
          (m, n) := getArraySize2D(csv=csv);
        annotation(__Dymola_translate=true);
      end readArraySize2D;

      function readArrayRows2D "Read first dimension of 2D array in CSV file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String delimiter = "," "Column delimiter character";
        input String quotation = "\"" "Quotation character";
        input Integer nHeaderLines = 0 "Number of header lines to ignore";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        output Integer m "Number of rows in array";
        protected
          Types.ExternCSVFile csv = Types.ExternCSVFile(fileName=fileName, delimiter=delimiter, quotation=quotation, nHeaderLines=nHeaderLines, verboseRead=verboseRead) "External CSV file object";
        algorithm
          m := getArrayRows2D(csv=csv);
        annotation(__Dymola_translate=true);
      end readArrayRows2D;

      function readArrayColumns2D "Read second dimension of 2D array in CSV file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String delimiter = "," "Column delimiter character";
        input String quotation = "\"" "Quotation character";
        input Integer nHeaderLines = 0 "Number of header lines to ignore";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        output Integer n "Number of columns in array";
        protected
          Types.ExternCSVFile csv = Types.ExternCSVFile(fileName=fileName, delimiter=delimiter, quotation=quotation, nHeaderLines=nHeaderLines, verboseRead=verboseRead) "External CSV file object";
        algorithm
          n := getArrayColumns2D(csv=csv);
        annotation(__Dymola_translate=true);
      end readArrayColumns2D;
    end CSV;

    package INI "INI file functions"
      extends Modelica.Icons.FunctionsPackage;
      pure function getReal "Get scalar Real value from INI file"
        extends Interfaces.INI.getReal;
        external "C" y=ED_getDoubleFromINI(ini, varName, section, strict, exist) annotation(
          __iti_dll = "ITI_ED_INIFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_INIFile.h\"",
          Library = {"ED_INIFile", "bsxml-json"});
      annotation(Documentation(info="<html>
<p>If strict=true and the line being read contains more than a name=value pair this function will return an error. However, when strict=false it will return the value and ignore the rest of the line.</p>
<p>For example</p>
<code>[example_section]<br />name=65.2   ;this is a comment</code>
<p>In this example, if strict=true we will get an error but if strict=false it will return the value 65.2</p>
</html>"));
      end getReal;

      pure function getInteger "Get scalar Integer value from INI file"
        extends Interfaces.INI.getInteger;
        external "C" y=ED_getIntFromINI(ini, varName, section, strict, exist) annotation(
          __iti_dll = "ITI_ED_INIFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_INIFile.h\"",
          Library = {"ED_INIFile", "bsxml-json"});
        annotation(Documentation(info="<html>
<p>If strict=true and the line being read contains more than a name=value pair this function will return an error. However, when strict=false it will return the value and ignore the rest of the line.</p>
<p>For example</p>
<code>[example_section]<br />name=13   ;this is a comment</code>
<p>In this example, if strict=true we will get an error but if strict=false it will return the value 13</p>
</html>"));
      end getInteger;

      pure function getBoolean "Get scalar Boolean value from INI file"
        extends Interfaces.INI.getBoolean;
        algorithm
          (aux, exist) := getReal(ini=ini, varName=varName, section=section, strict=strict);
          y := aux <> 0;
        annotation(Inline=false);
      end getBoolean;

      pure function getString "Get scalar String value from INI file"
        extends Interfaces.INI.getString;
        external "C" str=ED_getStringFromINI(ini, varName, section, exist) annotation(
          __iti_dll = "ITI_ED_INIFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_INIFile.h\"",
          Library = {"ED_INIFile", "bsxml-json"});
      end getString;
    end INI;

    package JSON "JSON file functions"
      extends Modelica.Icons.FunctionsPackage;
      pure function getReal "Get scalar Real value from JSON file"
        extends Interfaces.JSON.getReal;
        external "C" y=ED_getDoubleFromJSON(json, varName, exist) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getReal;

      pure function getRealArray1D "Get 1D Real values from JSON file"
        extends Interfaces.JSON.getRealArray1D;
        external "C" ED_getDoubleArray1DFromJSON(json, varName, y, size(y, 1)) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getRealArray1D;

      pure function getRealArray2D "Get 2D Real values from JSON file"
        extends Interfaces.JSON.getRealArray2D;
        external "C" ED_getDoubleArray2DFromJSON(json, varName, y, size(y, 1), size(y, 2)) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getRealArray2D;

      pure function getInteger "Get scalar Integer value from JSON file"
        extends Interfaces.JSON.getInteger;
        external "C" y=ED_getIntFromJSON(json, varName, exist) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getInteger;

      pure function getIntegerArray1D "Get 1D Integer values from JSON file"
        extends Interfaces.JSON.getIntegerArray1D;
        external "C" ED_getIntArray1DFromJSON(json, varName, y, size(y, 1)) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getIntegerArray1D;

      pure function getIntegerArray2D "Get 2D Integer values from JSON file"
        extends Interfaces.JSON.getIntegerArray2D;
        external "C" ED_getIntArray2DFromJSON(json, varName, y, size(y, 1), size(y, 2)) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getIntegerArray2D;

      pure function getBoolean "Get scalar Boolean value from JSON file"
        extends Interfaces.JSON.getBoolean;
        external "C" y = ED_getBooleanFromJSON(json, varName, exist) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getBoolean;

      pure function getBooleanArray1D "Get 1D Boolean values from JSON file"
        extends Interfaces.JSON.getBooleanArray1D;
        external "C" ED_getBooleanArray1DFromJSON(json, varName, y, size(y, 1)) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getBooleanArray1D;

      pure function getBooleanArray2D "Get 2D Boolean values from JSON file"
        extends Interfaces.JSON.getBooleanArray2D;
        external "C" ED_getBooleanArray2DFromJSON(json, varName, y, size(y, 1), size(y, 2)) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getBooleanArray2D;

      pure function getString "Get scalar String value from JSON file"
        extends Interfaces.JSON.getString;
        external "C" str=ED_getStringFromJSON(json, varName, exist) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getString;

      pure function getStringArray1D "Get 1D String values from JSON file"
        extends Interfaces.JSON.getStringArray1D;
        external "C" ED_getStringArray1DFromJSON(json, varName, y, size(y, 1)) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getStringArray1D;

      pure function getStringArray2D "Get 2D String values from JSON file"
        extends Interfaces.JSON.getStringArray2D;
        external "C" ED_getStringArray2DFromJSON(json, varName, y, size(y, 1), size(y, 2)) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getStringArray2D;

      pure function getArraySize1D "Get length of 1D array in JSON file"
        extends Interfaces.JSON.getArraySize1D;
        external "C" ED_getArray1DDimensionFromJSON(json, varName, n) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getArraySize1D;

      pure function getArraySize2D "Get dimensions of 2D array in JSON file"
        extends Interfaces.JSON.getArraySize2D;
        external "C" ED_getArray2DDimensionsFromJSON(json, varName, m, n) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getArraySize2D;

      pure function getArrayRows2D "Get first dimension of 2D array in JSON file"
        extends Interfaces.JSON.getArrayRows2D;
        external "C" ED_getArray2DDimensionsFromJSON(json, varName, m, n) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getArrayRows2D;

      pure function getArrayColumns2D "Get second dimension of 2D array in JSON file"
        extends Interfaces.JSON.getArrayColumns2D;
        external "C" ED_getArray2DDimensionsFromJSON(json, varName, m, n) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getArrayColumns2D;

      function readArraySize1D "Read length of 1D array in JSON file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String varName "Key";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data";
        output Integer n "Number of elements in array";
        protected
          Types.ExternJSONFile json = Types.ExternJSONFile(fileName=fileName, verboseRead=verboseRead, detectMissingData=detectMissingData) "External JSON file object";
        algorithm
          n := getArraySize1D(json=json, varName=varName);
        annotation(__Dymola_translate=true);
      end readArraySize1D;

      function readArraySize2D "Read dimensions of 2D array in JSON file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String varName "Key";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data";
        output Integer m "Number of rows in array";
        output Integer n "Number of columns in array";
        protected
          Types.ExternJSONFile json = Types.ExternJSONFile(fileName=fileName, verboseRead=verboseRead, detectMissingData=detectMissingData) "External JSON file object";
        algorithm
          (m, n) := getArraySize2D(json=json, varName=varName);
        annotation(__Dymola_translate=true);
      end readArraySize2D;

      function readArrayRows2D "Read first dimension of 2D array in JSON file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String varName "Key";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data";
        output Integer m "Number of rows in array";
        protected
          Types.ExternJSONFile json = Types.ExternJSONFile(fileName=fileName, verboseRead=verboseRead, detectMissingData=detectMissingData) "External JSON file object";
        algorithm
          m := getArrayRows2D(json=json, varName=varName);
        annotation(__Dymola_translate=true);
      end readArrayRows2D;

      function readArrayColumns2D "Read second dimension of 2D array in JSON file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String varName "Key";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data";
        output Integer n "Number of columns in array";
        protected
          Types.ExternJSONFile json = Types.ExternJSONFile(fileName=fileName, verboseRead=verboseRead, detectMissingData=detectMissingData) "External JSON file object";
        algorithm
          n := getArrayColumns2D(json=json, varName=varName);
        annotation(__Dymola_translate=true);
      end readArrayColumns2D;
    end JSON;

    package MAT "MAT file functions"
      extends Modelica.Icons.FunctionsPackage;
      pure function getRealArray2D "Get 2D Real values from MAT file"
        extends Interfaces.MAT.getRealArray2D;
        external "C" ED_getDoubleArray2DFromMAT(mat, varName, y, size(y, 1), size(y, 2)) annotation(
          __iti_dll = "ITI_ED_MATFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_MATFile.h\"",
          Library = {"ED_MATFile", "hdf5", "zlib", "dl"});
      end getRealArray2D;

      pure function getStringArray1D "Get 1D String values from MAT file"
        extends Interfaces.MAT.getStringArray1D;
        external "C" ED_getStringArray1DFromMAT(mat, varName, str, size(str, 1)) annotation(
          __iti_dll = "ITI_ED_MATFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_MATFile.h\"",
          Library = {"ED_MATFile", "hdf5", "zlib", "dl"});
      end getStringArray1D;

      pure function getArraySize2D "Get dimensions of 2D array in MAT file"
        extends Interfaces.MAT.getArraySize2D;
        external "C" ED_getArray2DDimensionsFromMAT(mat, varName, m, n) annotation(
          __iti_dll = "ITI_ED_MATFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_MATFile.h\"",
          Library = {"ED_MATFile", "hdf5", "zlib", "dl"});
      end getArraySize2D;

      pure function getArrayRows2D "Get first dimension of 2D array in MAT file"
        extends Interfaces.MAT.getArrayRows2D;
        external "C" ED_getArray2DDimensionsFromMAT(mat, varName, m, n) annotation(
          __iti_dll = "ITI_ED_MATFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_MATFile.h\"",
          Library = {"ED_MATFile", "hdf5", "zlib", "dl"});
      end getArrayRows2D;

      pure function getArrayColumns2D "Get second dimension of 2D array in MAT file"
        extends Interfaces.MAT.getArrayColumns2D;
        external "C" ED_getArray2DDimensionsFromMAT(mat, varName, m, n) annotation(
          __iti_dll = "ITI_ED_MATFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_MATFile.h\"",
          Library = {"ED_MATFile", "hdf5", "zlib", "dl"});
      end getArrayColumns2D;

      function readArraySize2D "Read dimensions of 2D array in MAT file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String varName "Variable name";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        output Integer m "Number of rows in array";
        output Integer n "Number of columns in array";
        protected
          Types.ExternMATFile mat = Types.ExternMATFile(fileName=fileName, verboseRead=verboseRead) "External MAT file object";
        algorithm
          (m, n) := getArraySize2D(mat=mat, varName=varName);
        annotation(__Dymola_translate=true);
      end readArraySize2D;

      function readArrayRows2D "Read first dimension of 2D array in MAT file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String varName "Variable name";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        output Integer m "Number of rows in array";
        protected
          Types.ExternMATFile mat = Types.ExternMATFile(fileName=fileName, verboseRead=verboseRead) "External MAT file object";
        algorithm
          m := getArrayRows2D(mat=mat, varName=varName);
        annotation(__Dymola_translate=true);
      end readArrayRows2D;

      function readArrayColumns2D "Read second dimension of 2D array in MAT file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String varName "Variable name";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        output Integer n "Number of columns in array";
        protected
          Types.ExternMATFile mat = Types.ExternMATFile(fileName=fileName, verboseRead=verboseRead) "External MAT file object";
        algorithm
          n := getArrayColumns2D(mat=mat, varName=varName);
        annotation(__Dymola_translate=true);
      end readArrayColumns2D;
    end MAT;

    package SSV "SSV file functions"
      extends Modelica.Icons.FunctionsPackage;
      pure function getReal "Get scalar Real value from SSV file"
        extends Interfaces.SSV.getReal;
        algorithm
          (y, exist) := XML2.getReal(xml=ssv, varName=if nameSpace == "" then "//Parameters/Parameter[@name='" + varName + "']/Real/@value" else "//ssv:Parameters/ssv:Parameter[@name='" + varName + "']/ssv:Real/@value");
      end getReal;

      pure function getInteger "Get scalar Integer value from SSV file"
        extends Interfaces.SSV.getInteger;
        algorithm
          (y, exist) := XML2.getInteger(xml=ssv, varName=if nameSpace == "" then "//Parameters/Parameter[@name='" + varName + "']/Integer/@value" else "//ssv:Parameters/ssv:Parameter[@name='" + varName + "']/ssv:Integer/@value");
      end getInteger;

      pure function getBoolean "Get scalar Boolean value from SSV file"
        extends Interfaces.SSV.getBoolean;
        algorithm
          (aux, exist) := XML2.getString(xml=ssv, varName=if nameSpace == "" then "//Parameters/Parameter[@name='" + varName + "']/Boolean/@value" else "//ssv:Parameters/ssv:Parameter[@name='" + varName + "']/ssv:Boolean/@value");
          y := aux == "true" or aux == "1";
      end getBoolean;

      pure function getString "Get scalar String value from SSV file"
        extends Interfaces.SSV.getString;
        algorithm
          (str, exist) := XML2.getString(xml=ssv, varName=if nameSpace == "" then "//Parameters/Parameter[@name='" + varName + "']/String/@value" else "//ssv:Parameters/ssv:Parameter[@name='" + varName + "']/ssv:String/@value");
      end getString;
    end SSV;

    package XLS "Excel XLS file functions"
      extends Modelica.Icons.FunctionsPackage;
      pure function getReal "Get scalar Real value from Excel XLS file"
        extends Interfaces.XLS.getReal;
        external "C" y=ED_getDoubleFromXLS(xls, cellAddress, sheetName, exist) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSFile.h\"",
          Library = {"ED_XLSFile", "xlsreader"});
      end getReal;

      pure function getRealArray2D "Get 2D Real values from Excel XLS file"
        extends Interfaces.XLS.getRealArray2D;
        external "C" ED_getDoubleArray2DFromXLS(xls, cellAddress, sheetName, y, size(y, 1), size(y, 2)) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSFile.h\"",
          Library = {"ED_XLSFile", "xlsreader"});
      end getRealArray2D;

      pure function getInteger "Get scalar Integer value from Excel XLS file"
        extends Interfaces.XLS.getInteger;
        external "C" y=ED_getIntFromXLS(xls, cellAddress, sheetName, exist) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSFile.h\"",
          Library = {"ED_XLSFile", "xlsreader"});
      end getInteger;

      pure function getBoolean "Get scalar Boolean value from Excel XLS file"
        extends Interfaces.XLS.getBoolean;
        algorithm
          (aux, exist) := getReal(xls=xls, cellAddress=cellAddress, sheetName=sheetName);
          y := aux <> 0;
        annotation(Inline=false);
      end getBoolean;

      pure function getString "Get scalar String value from Excel XLS file"
        extends Interfaces.XLS.getString;
        external "C" str=ED_getStringFromXLS(xls, cellAddress, sheetName, exist) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSFile.h\"",
          Library = {"ED_XLSFile", "xlsreader"});
      end getString;

      pure function getArraySize2D "Get dimensions of 2D array in Excel XLS file"
        extends Interfaces.XLS.getArraySize2D;
        external "C" ED_getArray2DDimensionsFromXLS(xls, sheetName, m, n) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSFile.h\"",
          Library = {"ED_XLSFile", "xlsreader"});
      end getArraySize2D;

      pure function getArrayRows2D "Get first dimension of 2D array in Excel XLS file"
        extends Interfaces.XLS.getArrayRows2D;
        external "C" ED_getArray2DDimensionsFromXLS(xls, sheetName, m, n) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSFile.h\"",
          Library = {"ED_XLSFile", "xlsreader"});
      end getArrayRows2D;

      pure function getArrayColumns2D "Get second dimension of 2D array in Excel XLS file"
        extends Interfaces.XLS.getArrayColumns2D;
        external "C" ED_getArray2DDimensionsFromXLS(xls, sheetName, m, n) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSFile.h\"",
          Library = {"ED_XLSFile", "xlsreader"});
      end getArrayColumns2D;

      function readArraySize2D "Read dimensions of 2D array in Excel XLS file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String sheetName = "" "Sheet name";
        input String encoding = "UTF-8" "Encoding";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data";
        output Integer m "Number of rows in array";
        output Integer n "Number of columns in array";
        protected
          Types.ExternXLSFile xls = Types.ExternXLSFile(fileName=fileName, encoding=encoding, verboseRead=verboseRead, detectMissingData=detectMissingData) "External Excel XLS file object";
        algorithm
          (m, n) := getArraySize2D(xls=xls, sheetName=sheetName);
        annotation(__Dymola_translate=true);
      end readArraySize2D;

      function readArrayRows2D "Read first dimension of 2D array in Excel XLS file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String sheetName = "" "Sheet name";
        input String encoding = "UTF-8" "Encoding";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data";
        output Integer m "Number of rows in array";
        protected
          Types.ExternXLSFile xls = Types.ExternXLSFile(fileName=fileName, encoding=encoding, verboseRead=verboseRead, detectMissingData=detectMissingData) "External Excel XLS file object";
        algorithm
          m := getArrayRows2D(xls=xls, sheetName=sheetName);
        annotation(__Dymola_translate=true);
      end readArrayRows2D;

      function readArrayColumns2D "Read second dimension of 2D array in Excel XLS file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String sheetName = "" "Sheet name";
        input String encoding = "UTF-8" "Encoding";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data";
        output Integer n "Number of columns in array";
        protected
          Types.ExternXLSFile xls = Types.ExternXLSFile(fileName=fileName, encoding=encoding, verboseRead=verboseRead, detectMissingData=detectMissingData) "External Excel XLS file object";
        algorithm
          n := getArrayColumns2D(xls=xls, sheetName=sheetName);
        annotation(__Dymola_translate=true);
      end readArrayColumns2D;
    end XLS;

    package XLSX "Excel XLSX file functions"
      extends Modelica.Icons.FunctionsPackage;
      pure function getReal "Get scalar Real value from Excel XLSX file"
        extends Interfaces.XLSX.getReal;
        external "C" y=ED_getDoubleFromXLSX(xlsx, cellAddress, sheetName, exist) annotation(
          __iti_dll = "ITI_ED_XLSXFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSXFile.h\"",
          Library = {"ED_XLSXFile", "bsxml-json", "expat", "minizip", "zlib"});
      end getReal;

      pure function getRealArray2D "Get 2D Real values from Excel XLSX file"
        extends Interfaces.XLSX.getRealArray2D;
        external "C" ED_getDoubleArray2DFromXLSX(xlsx, cellAddress, sheetName, y, size(y, 1), size(y, 2)) annotation(
          __iti_dll = "ITI_ED_XLSXFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSXFile.h\"",
          Library = {"ED_XLSXFile", "bsxml-json", "expat", "minizip", "zlib"});
      end getRealArray2D;

      pure function getInteger "Get scalar Integer value from Excel XLSX file"
        extends Interfaces.XLSX.getInteger;
        external "C" y=ED_getIntFromXLSX(xlsx, cellAddress, sheetName, true) annotation(
          __iti_dll = "ITI_ED_XLSXFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSXFile.h\"",
          Library = {"ED_XLSXFile", "bsxml-json", "expat", "minizip", "zlib"});
      end getInteger;

      pure function getBoolean "Get scalar Boolean value from Excel XLSX file"
        extends Interfaces.XLSX.getBoolean;
        algorithm
          (aux, exist) := getReal(xlsx=xlsx, cellAddress=cellAddress, sheetName=sheetName);
          y := aux <> 0;
        annotation(Inline=false);
      end getBoolean;

      pure function getString "Get scalar String value from Excel XLSX file"
        extends Interfaces.XLSX.getString;
        external "C" str=ED_getStringFromXLSX(xlsx, cellAddress, sheetName, exist) annotation(
          __iti_dll = "ITI_ED_XLSXFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSXFile.h\"",
          Library = {"ED_XLSXFile", "bsxml-json", "expat", "minizip", "zlib"});
      end getString;

      pure function getArraySize2D "Get dimensions of 2D array in Excel XLSX file"
        extends Interfaces.XLSX.getArraySize2D;
        external "C" ED_getArray2DDimensionsFromXLSX(xlsx, sheetName, m, n) annotation(
          __iti_dll = "ITI_ED_XLSXFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSXFile.h\"",
          Library = {"ED_XLSXFile", "bsxml-json", "expat", "minizip", "zlib"});
      end getArraySize2D;

      pure function getArrayRows2D "Get first dimension of 2D array in Excel XLSX file"
        extends Interfaces.XLSX.getArrayRows2D;
        external "C" ED_getArray2DDimensionsFromXLSX(xlsx, sheetName, m, n) annotation(
          __iti_dll = "ITI_ED_XLSXFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSXFile.h\"",
          Library = {"ED_XLSXFile", "bsxml-json", "expat", "minizip", "zlib"});
      end getArrayRows2D;

      pure function getArrayColumns2D "Get second dimension of 2D array in Excel XLSX file"
        extends Interfaces.XLSX.getArrayColumns2D;
        external "C" ED_getArray2DDimensionsFromXLSX(xlsx, sheetName, m, n) annotation(
          __iti_dll = "ITI_ED_XLSXFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSXFile.h\"",
          Library = {"ED_XLSXFile", "bsxml-json", "expat", "minizip", "zlib"});
      end getArrayColumns2D;

      function readArraySize2D "Read dimensions of 2D array in Excel XLSX file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String sheetName = "" "Sheet name";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data";
        output Integer m "Number of rows in array";
        output Integer n "Number of columns in array";
        protected
          Types.ExternXLSXFile xlsx = Types.ExternXLSXFile(fileName=fileName, verboseRead=verboseRead, detectMissingData=detectMissingData) "External Excel XLSX file object";
        algorithm
          (m, n) := getArraySize2D(xlsx=xlsx, sheetName=sheetName);
        annotation(__Dymola_translate=true);
      end readArraySize2D;

      function readArrayRows2D "Read first dimension of 2D array in Excel XLSX file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String sheetName = "" "Sheet name";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data";
        output Integer m "Number of rows in array";
        protected
          Types.ExternXLSXFile xlsx = Types.ExternXLSXFile(fileName=fileName, verboseRead=verboseRead, detectMissingData=detectMissingData) "External Excel XLSX file object";
        algorithm
          m := getArrayRows2D(xlsx=xlsx, sheetName=sheetName);
        annotation(__Dymola_translate=true);
      end readArrayRows2D;

      function readArrayColumns2D "Read second dimension of 2D array in Excel XLSX file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String sheetName = "" "Sheet name";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data";
        output Integer n "Number of columns in array";
        protected
          Types.ExternXLSXFile xlsx = Types.ExternXLSXFile(fileName=fileName, verboseRead=verboseRead, detectMissingData=detectMissingData) "External Excel XLSX file object";
        algorithm
          n := getArrayColumns2D(xlsx=xlsx, sheetName=sheetName);
        annotation(__Dymola_translate=true);
      end readArrayColumns2D;
    end XLSX;

    package XML "XML file functions"
      extends Modelica.Icons.FunctionsPackage;
      pure function getReal "Get scalar Real value from XML file"
        extends Interfaces.XML.getReal;
        external "C" y=ED_getDoubleFromXML(xml, varName, exist) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end getReal;

      pure function getRealArray1D "Get 1D Real values from XML file"
        extends Interfaces.XML.getRealArray1D;
        external "C" ED_getDoubleArray1DFromXML(xml, varName, y, size(y, 1)) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end getRealArray1D;

      pure function getRealArray2D "Get 2D Real values from XML file"
        extends Interfaces.XML.getRealArray2D;
        external "C" ED_getDoubleArray2DFromXML(xml, varName, y, size(y, 1), size(y, 2)) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end getRealArray2D;

      pure function getInteger "Get scalar Integer value from XML file"
        extends Interfaces.XML.getInteger;
        external "C" y=ED_getIntFromXML(xml, varName, exist) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end getInteger;

      pure function getBoolean "Get scalar Boolean value from XML file"
        extends Interfaces.XML.getBoolean;
        algorithm
          (aux, exist) := getReal(xml=xml, varName=varName);
          y := aux <> 0;
        annotation(Inline=false);
      end getBoolean;

      pure function getString "Get scalar String value from XML file"
        extends Interfaces.XML.getString;
        external "C" str=ED_getStringFromXML(xml, varName, exist) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end getString;

      pure function getArraySize1D "Get length of 1D array in XML file"
        extends Interfaces.XML.getArraySize1D;
        external "C" ED_getArray1DDimensionFromXML(xml, varName, n) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end getArraySize1D;

      pure function getArraySize2D "Get dimensions of 2D array in XML file"
        extends Interfaces.XML.getArraySize2D;
        external "C" ED_getArray2DDimensionsFromXML(xml, varName, m, n) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end getArraySize2D;

      pure function getArrayRows2D "Get first dimension of 2D array in XML file"
        extends Interfaces.XML.getArrayRows2D;
        external "C" ED_getArray2DDimensionsFromXML(xml, varName, m, n) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end getArrayRows2D;

      pure function getArrayColumns2D "Get second dimension of 2D array in XML file"
        extends Interfaces.XML.getArrayColumns2D;
        external "C" ED_getArray2DDimensionsFromXML(xml, varName, m, n) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end getArrayColumns2D;

      function readArraySize1D "Read length of 1D array in XML file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String varName "Key";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data";
        output Integer n "Number of elements in array";
        protected
          Types.ExternXMLFile xml = Types.ExternXMLFile(fileName=fileName, verboseRead=verboseRead, detectMissingData=detectMissingData) "External XML file object";
        algorithm
          n := getArraySize1D(xml=xml, varName=varName);
        annotation(__Dymola_translate=true);
      end readArraySize1D;

      function readArraySize2D "Read dimensions of 2D array in XML file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String varName "Key";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data";
        output Integer m "Number of rows in array";
        output Integer n "Number of columns in array";
        protected
          Types.ExternXMLFile xml = Types.ExternXMLFile(fileName=fileName, verboseRead=verboseRead, detectMissingData=detectMissingData) "External XML file object";
        algorithm
          (m, n) := getArraySize2D(xml=xml, varName=varName);
        annotation(__Dymola_translate=true);
      end readArraySize2D;

      function readArrayRows2D "Read first dimension of 2D array in XML file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String varName "Key";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data";
        output Integer m "Number of rows in array";
        protected
          Types.ExternXMLFile xml = Types.ExternXMLFile(fileName=fileName, verboseRead=verboseRead, detectMissingData=detectMissingData) "External XML file object";
        algorithm
          m := getArrayRows2D(xml=xml, varName=varName);
        annotation(__Dymola_translate=true);
      end readArrayRows2D;

      function readArrayColumns2D "Read second dimension of 2D array in XML file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String varName "Key";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data";
        output Integer n "Number of columns in array";
        protected
          Types.ExternXMLFile xml = Types.ExternXMLFile(fileName=fileName, verboseRead=verboseRead, detectMissingData=detectMissingData) "External XML file object";
        algorithm
          n := getArrayColumns2D(xml=xml, varName=varName);
        annotation(__Dymola_translate=true);
      end readArrayColumns2D;
    end XML;

    package XML2 "XML2 file functions"
      extends Modelica.Icons.FunctionsPackage;
      pure function getReal "Get scalar Real value from XML file"
        extends Interfaces.XML2.getReal;
        external "C" y=ED_getDoubleFromXML2(xml, varName, exist) annotation(
          __iti_dll = "ITI_ED_XML2File.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XML2File.h\"",
          Library = {"ED_XML2File", "xml2", "zlib"});
      end getReal;

      pure function getRealArray1D "Get 1D Real values from XML file"
        extends Interfaces.XML2.getRealArray1D;
        external "C" ED_getDoubleArray1DFromXML2(xml, varName, y, size(y, 1)) annotation(
          __iti_dll = "ITI_ED_XML2File.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XML2File.h\"",
          Library = {"ED_XML2File", "xml2", "zlib"});
      end getRealArray1D;

      pure function getRealArray2D "Get 2D Real values from XML file"
        extends Interfaces.XML2.getRealArray2D;
        external "C" ED_getDoubleArray2DFromXML2(xml, varName, y, size(y, 1), size(y, 2)) annotation(
          __iti_dll = "ITI_ED_XML2File.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XML2File.h\"",
          Library = {"ED_XML2File", "xml2", "zlib"});
      end getRealArray2D;

      pure function getInteger "Get scalar Integer value from XML file"
        extends Interfaces.XML2.getInteger;
        external "C" y=ED_getIntFromXML2(xml, varName, exist) annotation(
          __iti_dll = "ITI_ED_XML2File.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XML2File.h\"",
          Library = {"ED_XML2File", "xml2", "zlib"});
      end getInteger;

      pure function getBoolean "Get scalar Boolean value from XML file"
        extends Interfaces.XML2.getBoolean;
        algorithm
          (aux, exist) := getReal(xml=xml, varName=varName);
          y := aux <> 0;
        annotation(Inline=false);
      end getBoolean;

      pure function getString "Get scalar String value from XML file"
        extends Interfaces.XML2.getString;
        external "C" str=ED_getStringFromXML2(xml, varName, exist) annotation(
          __iti_dll = "ITI_ED_XML2File.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XML2File.h\"",
          Library = {"ED_XML2File", "xml2", "zlib"});
      end getString;

      pure function getArraySize1D "Get length of 1D array in XML file"
        extends Interfaces.XML2.getArraySize1D;
        external "C" ED_getArray1DDimensionFromXML2(xml, varName, n) annotation(
          __iti_dll = "ITI_ED_XML2File.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XML2File.h\"",
          Library = {"ED_XML2File", "xml2", "zlib"});
      end getArraySize1D;

      pure function getArraySize2D "Get dimensions of 2D array in XML file"
        extends Interfaces.XML2.getArraySize2D;
        external "C" ED_getArray2DDimensionsFromXML2(xml, varName, m, n) annotation(
          __iti_dll = "ITI_ED_XML2File.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XML2File.h\"",
          Library = {"ED_XML2File", "xml2", "zlib"});
      end getArraySize2D;

      pure function getArrayRows2D "Get first dimension of 2D array in XML file"
        extends Interfaces.XML2.getArrayRows2D;
        external "C" ED_getArray2DDimensionsFromXML2(xml, varName, m, n) annotation(
          __iti_dll = "ITI_ED_XML2File.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XML2File.h\"",
          Library = {"ED_XML2File", "xml2", "zlib"});
      end getArrayRows2D;

      pure function getArrayColumns2D "Get second dimension of 2D array in XML file"
        extends Interfaces.XML2.getArrayColumns2D;
        external "C" ED_getArray2DDimensionsFromXML2(xml, varName, m, n) annotation(
          __iti_dll = "ITI_ED_XML2File.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XML2File.h\"",
          Library = {"ED_XML2File", "xml2", "zlib"});
      end getArrayColumns2D;

      function readArraySize1D "Read length of 1D array in XML file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String varName "Key";
        input String nameSpace[:,2] = fill("", 0, 2) "XML name spaces";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data";
        output Integer n "Number of elements in array";
        protected
          Types.ExternXML2File xml = Types.ExternXML2File(fileName=fileName, nameSpace=nameSpace, verboseRead=verboseRead, detectMissingData=detectMissingData) "External XML2 file object";
        algorithm
          n := getArraySize1D(xml=xml, varName=varName);
        annotation(__Dymola_translate=true);
      end readArraySize1D;

      function readArraySize2D "Read dimensions of 2D array in XML file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String varName "Key";
        input String nameSpace[:,2] = fill("", 0, 2) "XML name spaces";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data";
        output Integer m "Number of rows in array";
        output Integer n "Number of columns in array";
        protected
          Types.ExternXML2File xml = Types.ExternXML2File(fileName=fileName, nameSpace=nameSpace, verboseRead=verboseRead, detectMissingData=detectMissingData) "External XML2 file object";
        algorithm
          (m, n) := getArraySize2D(xml=xml, varName=varName);
        annotation(__Dymola_translate=true);
      end readArraySize2D;

      function readArrayRows2D "Read first dimension of 2D array in XML file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String varName "Key";
        input String nameSpace[:,2] = fill("", 0, 2) "XML name spaces";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data";
        output Integer m "Number of rows in array";
        protected
          Types.ExternXML2File xml = Types.ExternXML2File(fileName=fileName, nameSpace=nameSpace, verboseRead=verboseRead, detectMissingData=detectMissingData) "External XML2 file object";
        algorithm
          m := getArrayRows2D(xml=xml, varName=varName);
        annotation(__Dymola_translate=true);
      end readArrayRows2D;

      function readArrayColumns2D "Read second dimension of 2D array in XML file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String varName "Key";
        input String nameSpace[:,2] = fill("", 0, 2) "XML name spaces";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Types.Diagnostics detectMissingData = Types.Diagnostics.Warning "Print diagnostic message in case of missing data";
        output Integer n "Number of columns in array";
        protected
          Types.ExternXML2File xml = Types.ExternXML2File(fileName=fileName, nameSpace=nameSpace, verboseRead=verboseRead, detectMissingData=detectMissingData) "External XML2 file object";
        algorithm
          n := getArrayColumns2D(xml=xml, varName=varName);
        annotation(__Dymola_translate=true);
      end readArrayColumns2D;
    end XML2;
  end Functions;

  package Interfaces "Interfaces"
    extends Modelica.Icons.InterfacesPackage;

    record DataSource "Place holder for file based data source"
      extends Modelica.Icons.Record;
      annotation(
        Documentation(info="<html><p>Place holder record that can be used as generic outer parameter instance in component models to reference an implemented inner parameter instance of <a href=\"modelica://ExternData.CSVFile\">CSVFile</a>, <a href=\"modelica://ExternData.INIFile\">INIFile</a>, <a href=\"modelica://ExternData.JSONFile\">JSONFile</a>, <a href=\"modelica://ExternData.MATFile\">MATFile</a>, <a href=\"modelica://ExternData.SSVFile\">SSVFile</a>, <a href=\"modelica://ExternData.XLSFile\">XLSFile</a>, <a href=\"modelica://ExternData.XLSXFile\">XLSXFile</a>, <a href=\"modelica://ExternData.XMLFile\">XMLFile</a>, <a href=\"modelica://ExternData.XML2File\">XML2File</a>, or <a href=\"modelica://ExternData.TIRFile\">TIRFile</a>.</p><p>See <a href=\"modelica://ExternData.Examples.XMLTest2.Component\">Examples.XMLTest2.Component</a> for an example.</p></html>"),
        defaultComponentName="dataSource",
        defaultComponentPrefixes="outer parameter",
        missingInnerMessage="No \"dataSource\" component is defined, please drag ExternData.CSVFile, ExternData.INIFile, ExternData.JSONFile, ExternData.MATFile, ExternData.SSVFile, ExternData.XLSFile, ExternData.XLSXFile, ExternData.XMLFile, ExternData.XML2File, or ExternData.TIRFile to the model top level");
    end DataSource;

    partial block DataSourceBlock "Base block containing an outer place holder data source"
      outer parameter ExternData.Interfaces.DataSource dataSource "File based data source" annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      annotation(Documentation(info="<html><p>Partial base block that declares an outer parameter instance of generic record type <a href=\"modelica://ExternData.Interfaces.DataSource\">DataSource</a>.</p></html>"));
    end DataSourceBlock;

    partial model DataSourceModel "Base model containing an outer place holder data source"
      outer parameter ExternData.Interfaces.DataSource dataSource "File based data source" annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      annotation(Documentation(info="<html><p>Partial base model that declares an outer parameter instance of generic record type <a href=\"modelica://ExternData.Interfaces.DataSource\">DataSource</a>.</p></html>"));
    end DataSourceModel;

    package CSV "CSV file interfaces"
      extends Modelica.Icons.InterfacesPackage;
      partial record Base "Interface for CSV file"
        replaceable function getRealArray2D = CSV.getRealArray2D "Get 2D Real values from CSV file" annotation(Documentation(info="<html></html>"));
        replaceable function getArraySize2D = CSV.getArraySize2D "Get the size of a 2D array in a CSV file" annotation(Documentation(info="<html></html>"));
        replaceable function getArrayRows2D = CSV.getArrayRows2D "Get first dimension of 2D array in CSV file" annotation(Documentation(info="<html></html>"));
        replaceable function getArrayColumns2D = CSV.getArrayColumns2D "Get second dimension of 2D array in CSV file" annotation(Documentation(info="<html></html>"));
      annotation(
        Documentation(info="<html><p>Base record that defines the function interfaces for <a href=\"modelica://ExternData.CSVFile\">CSVFile</a>.</p></html>"),
        Icon(graphics={
          Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
          Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillPattern=FillPattern.Solid)}));
      end Base;

      partial function getRealArray2D "Get 2D Real values from CSV file"
        extends Modelica.Icons.Function;
        input Integer m = 1 "Number of rows";
        input Integer n = 1 "Number of columns";
        input Integer field[2](each min=1)={1,1} "Start field {row, col}";
        input Types.ExternCSVFile csv "External CSV file object";
        output Real y[m,n] "2D Real values";
        annotation(Documentation(info="<html></html>"));
      end getRealArray2D;

      partial function getArraySize2D "Get dimensions of 2D array in CSV file"
        extends Modelica.Icons.Function;
        input Types.ExternCSVFile csv "External CSV file object";
        output Integer m "Number of rows in array";
        output Integer n "Number of columns in array";
        annotation(Documentation(info="<html></html>"));
      end getArraySize2D;

      partial function getArrayRows2D "Get first dimension of 2D array in CSV file"
        extends Modelica.Icons.Function;
        input Types.ExternCSVFile csv "External CSV file object";
        output Integer m "Number of rows in array";
        protected
          Integer n[1] "Number of columns in array";
        annotation(Documentation(info="<html></html>"));
      end getArrayRows2D;

      partial function getArrayColumns2D "Get second dimension of 2D array in CSV file"
        extends Modelica.Icons.Function;
        input Types.ExternCSVFile csv "External CSV file object";
        output Integer n "Number of columns in array";
        protected
          Integer m[1] "Number of rows in array";
        annotation(Documentation(info="<html></html>"));
      end getArrayColumns2D;
    end CSV;

    package INI "INI file interfaces"
      extends Modelica.Icons.InterfacesPackage;
      partial record Base "Interface for INI file and TIR file"
        replaceable function getReal = INI.getReal "Get scalar Real value from INI file" annotation(Documentation(info="<html></html>"));
        replaceable function getInteger = INI.getInteger "Get scalar Integer value from INI file" annotation(Documentation(info="<html></html>"));
        replaceable function getBoolean = INI.getBoolean "Get scalar Boolean value from INI file" annotation(Documentation(info="<html></html>"));
        replaceable function getString = INI.getString "Get scalar String value from INI file" annotation(Documentation(info="<html></html>"));
      annotation(
        Documentation(info="<html><p>Base record that defines the function interfaces for <a href=\"modelica://ExternData.INIFile\">INIFile</a> and <a href=\"modelica://ExternData.TIRFile\">TIRFile</a>.</p></html>"),
        Icon(graphics={
          Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
          Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillPattern=FillPattern.Solid)}));
      end Base;

      partial function getReal "Get scalar Real value from INI file"
        extends Interfaces.partialGetReal;
        input String section="" "Section";
        input Types.ExternINIFile ini "External INI file object";
        input Boolean strict=true "Return an error if there are characters on the line that aren't part of the value";
        annotation(Documentation(info="<html>
<p>If strict=true and the line being read contains more than a name=value pair this function will return an error. However, when strict=false it will return the value and ignore the rest of the line.</p>
<p>For example</p>
<code>[example_section]<br />name=65.2   ;this is a comment</code>
<p>In this example, if strict=true we will get an error but if strict=false it will return the value 65.2</p>
</html>"));
      end getReal;

      partial function getInteger "Get scalar Integer value from INI file"
        extends Interfaces.partialGetInteger;
        input String section="" "Section";
        input Types.ExternINIFile ini "External INI file object";
        input Boolean strict=true "Return an error if there are characters on the line that aren't part of the value";
        annotation(Documentation(info="<html>
<p>If strict=true and the line being read contains more than a name=value pair this function will return an error. However, when strict=false it will return the value and ignore the rest of the line.</p>
<p>For example</p>
<code>[example_section]<br />name=13   ;this is a comment</code>
<p>In this example, if strict=true we will get an error but if strict=false it will return the value 13</p>
</html>"));
      end getInteger;

      partial function getBoolean "Get scalar Boolean value from INI file"
        extends Interfaces.partialGetBoolean;
        input String section="" "Section";
        input Types.ExternINIFile ini "External INI file object";
        input Boolean strict=true "Return an error if there are characters on the line that aren't part of the value";
        protected
          Real aux;
        annotation(Documentation(info="<html></html>"));
      end getBoolean;

      partial function getString "Get scalar String value from INI file"
        extends Interfaces.partialGetString;
        input String section="" "Section";
        input Types.ExternINIFile ini "External INI file object";
        annotation(Documentation(info="<html></html>"));
      end getString;
    end INI;

    package JSON "JSON file interfaces"
      extends Modelica.Icons.InterfacesPackage;
      partial record Base "Interface for JSON file"
        replaceable function getReal = JSON.getReal "Get scalar Real value from JSON file" annotation(Documentation(info="<html></html>"));
        replaceable function getRealArray1D = JSON.getRealArray1D "Get 1D array Real value from JSON file" annotation(Documentation(info="<html></html>"));
        replaceable function getRealArray2D = JSON.getRealArray2D "Get 2D array Real value from JSON file" annotation(Documentation(info="<html></html>"));
        replaceable function getInteger = JSON.getInteger "Get scalar Integer value from JSON file" annotation(Documentation(info="<html></html>"));
        replaceable function getIntegerArray1D = JSON.getIntegerArray1D "Get 1D array Integer value from JSON file" annotation(Documentation(info="<html></html>"));
        replaceable function getIntegerArray2D = JSON.getIntegerArray2D "Get 2D array Integer value from JSON file" annotation(Documentation(info="<html></html>"));
        replaceable function getBoolean = JSON.getBoolean "Get scalar Boolean value from JSON file" annotation(Documentation(info="<html></html>"));
        replaceable function getBooleanArray1D = JSON.getBooleanArray1D "Get 1D array Boolean value from JSON file" annotation(Documentation(info="<html></html>"));
        replaceable function getBooleanArray2D = JSON.getBooleanArray2D "Get 2D array Boolean value from JSON file" annotation(Documentation(info="<html></html>"));
        replaceable function getString = JSON.getString "Get scalar String value from JSON file" annotation(Documentation(info="<html></html>"));
        replaceable function getStringArray1D = JSON.getStringArray1D "Get 1D array String value from JSON file" annotation(Documentation(info="<html></html>"));
        replaceable function getStringArray2D = JSON.getStringArray2D "Get 2D array String value from JSON file" annotation(Documentation(info="<html></html>"));
        replaceable function getArraySize1D = JSON.getArraySize1D"Get the size of a 1D array in a JSON file" annotation(Documentation(info="<html></html>"));
        replaceable function getArraySize2D = JSON.getArraySize2D"Get the size of a 2D array in a JSON file" annotation(Documentation(info="<html></html>"));
        replaceable function getArrayRows2D = JSON.getArrayRows2D "Get first dimension of 2D array in JSON file" annotation(Documentation(info="<html></html>"));
        replaceable function getArrayColumns2D = JSON.getArrayColumns2D "Get second dimension of 2D array in JSON file" annotation(Documentation(info="<html></html>"));
      annotation(
        Documentation(info="<html><p>Base record that defines the function interfaces for <a href=\"modelica://ExternData.JSONFile\">JSONFile</a>.</p></html>"),
        Icon(graphics={
          Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
          Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillPattern=FillPattern.Solid)}));
      end Base;

      partial function getReal "Get scalar Real value from JSON file"
        extends Interfaces.partialGetReal;
        input Types.ExternJSONFile json "External JSON file object";
        annotation(Documentation(info="<html></html>"));
      end getReal;

      partial function getRealArray1D "Get 1D Real values from JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer n = 1 "Number of elements";
        input Types.ExternJSONFile json "External JSON file object";
        output Real y[n] "1D Real values";
        annotation(Documentation(info="<html></html>"));
      end getRealArray1D;

      partial function getRealArray2D "Get 2D Real values from JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer m = 1 "Number of rows";
        input Integer n = 1 "Number of columns";
        input Types.ExternJSONFile json "External JSON file object";
        output Real y[m,n] "2D Real values";
        annotation(Documentation(info="<html></html>"));
      end getRealArray2D;

      partial function getInteger "Get scalar Integer value from JSON file"
        extends Interfaces.partialGetInteger;
        input Types.ExternJSONFile json "External JSON file object";
        annotation(Documentation(info="<html></html>"));
      end getInteger;

      partial function getIntegerArray1D "Get 1D Integer values from JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer n = 1 "Number of elements";
        input Types.ExternJSONFile json "External JSON file object";
        output Integer y[n] "1D Real values";
        annotation(Documentation(info="<html></html>"));
      end getIntegerArray1D;

      partial function getIntegerArray2D "Get 2D Integer values from JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer m = 1 "Number of rows";
        input Integer n = 1 "Number of columns";
        input Types.ExternJSONFile json "External JSON file object";
        output Integer y[m,n] "2D Real values";
        annotation(Documentation(info="<html></html>"));
      end getIntegerArray2D;

      partial function getBoolean "Get scalar Boolean value from JSON file"
        extends Interfaces.partialGetBoolean;
        input Types.ExternJSONFile json "External JSON file object";
        annotation(Documentation(info="<html></html>"));
      end getBoolean;

      partial function getBooleanArray1D "Get 1D Boolean values from JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer n = 1 "Number of elements";
        input Types.ExternJSONFile json "External JSON file object";
        output Boolean y[n] "1D Real values";
        annotation(Documentation(info="<html></html>"));
      end getBooleanArray1D;

      partial function getBooleanArray2D "Get 2D Boolean values from JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer m = 1 "Number of rows";
        input Integer n = 1 "Number of columns";
        input Types.ExternJSONFile json "External JSON file object";
        output Boolean y[m,n] "2D Real values";
        annotation(Documentation(info="<html></html>"));
      end getBooleanArray2D;

      partial function getString "Get scalar String value from JSON file"
        extends Interfaces.partialGetString;
        input Types.ExternJSONFile json "External JSON file object";
        annotation(Documentation(info="<html></html>"));
      end getString;

      partial function getStringArray1D "Get 1D String values from JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer n = 1 "Number of elements";
        input Types.ExternJSONFile json "External JSON file object";
        output String y[n] "1D Real values";
        annotation(Documentation(info="<html></html>"));
      end getStringArray1D;

      partial function getStringArray2D "Get 2D String values from JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer m = 1 "Number of rows";
        input Integer n = 1 "Number of columns";
        input Types.ExternJSONFile json "External JSON file object";
        output String y[m,n] "2D Real values";
        annotation(Documentation(info="<html></html>"));
      end getStringArray2D;

      partial function getArraySize1D "Get length of 1D array in JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Types.ExternJSONFile json "External JSON file object";
        output Integer n "Number of elements in array";
        annotation(Documentation(info="<html></html>"));
      end getArraySize1D;

      partial function getArraySize2D "Get dimensions of 2D array in JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Types.ExternJSONFile json "External JSON file object";
        output Integer m "Number of rows in array";
        output Integer n "Number of columns in array";
        annotation(Documentation(info="<html></html>"));
      end getArraySize2D;

      partial function getArrayRows2D "Get first dimension of 2D array in JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Types.ExternJSONFile json "External JSON file object";
        output Integer m "Number of rows in array";
        protected
          Integer n[1] "Number of columns in array";
        annotation(Documentation(info="<html></html>"));
      end getArrayRows2D;

      partial function getArrayColumns2D "Get second dimension of 2D array in JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Types.ExternJSONFile json "External JSON file object";
        output Integer n "Number of columns in array";
        protected
          Integer m[1] "Number of rows in array";
        annotation(Documentation(info="<html></html>"));
      end getArrayColumns2D;
    end JSON;

    package MAT "MAT file interfaces"
      extends Modelica.Icons.InterfacesPackage;
      partial record Base "Interface for MAT file"
        replaceable function getRealArray2D = MAT.getRealArray2D "Get 2D Real values from MAT file" annotation(Documentation(info="<html></html>"));
        replaceable function getStringArray1D = MAT.getStringArray1D "Get 1D String values from MAT file" annotation(Documentation(info="<html></html>"));
        replaceable function getArraySize2D = MAT.getArraySize2D "Get the size of a 2D array in a MAT file" annotation(Documentation(info="<html></html>"));
        replaceable function getArrayRows2D = MAT.getArrayRows2D "Get first dimension of 2D array in MAT file" annotation(Documentation(info="<html></html>"));
        replaceable function getArrayColumns2D = MAT.getArrayColumns2D "Get second dimension of 2D array in MAT file" annotation(Documentation(info="<html></html>"));
      annotation(
        Documentation(info="<html><p>Base record that defines the function interfaces for <a href=\"modelica://ExternData.MATFile\">MATFile</a>.</p></html>"),
        Icon(graphics={
          Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
          Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillColor={241,219,48},fillPattern=FillPattern.Solid),
          Rectangle(extent={{-80,20},{80,-80}},lineColor={248,236,140},fillColor={241,219,48},fillPattern=FillPattern.HorizontalCylinder),
          Rectangle(extent={{-80,20},{80,-80}}),
          Line(points={{-40,20},{-40,-80}}),
          Line(points={{0,20},{0,-80}}),
          Line(points={{40,20},{40,-80}}),
          Line(points={{-80,-30},{80,-30}})}));
      end Base;

      partial function getRealArray2D "Get 2D Real values from MAT file"
        extends Modelica.Icons.Function;
        input String varName "Variable name";
        input Integer m = 1 "Number of rows";
        input Integer n = 1 "Number of columns";
        input Types.ExternMATFile mat "External MATLAB MAT file object";
        output Real y[m,n] "2D Real values";
        annotation(Documentation(info="<html></html>"));
      end getRealArray2D;

      partial function getStringArray1D "Get 1D String values from MAT file"
        extends Modelica.Icons.Function;
        input String varName "Variable name";
        input Integer n = 1 "Number of elements";
        input Types.ExternMATFile mat "External MATLAB MAT file object";
        output String str[n] "1D String values";
        annotation(Documentation(info="<html></html>"));
      end getStringArray1D;

      partial function getArraySize2D "Get dimensions of 2D array in MAT file"
        extends Modelica.Icons.Function;
        input String varName "Variable name";
        input Types.ExternMATFile mat "External MATLAB MAT file object";
        output Integer m "Number of rows in array";
        output Integer n "Number of columns in array";
        annotation(Documentation(info="<html></html>"));
      end getArraySize2D;

      partial function getArrayRows2D "Get first dimension of 2D array in MAT file"
        extends Modelica.Icons.Function;
        input String varName "Variable name";
        input Types.ExternMATFile mat "External MATLAB MAT file object";
        output Integer m "Number of rows in array";
        protected
          Integer n[1] "Number of columns in array";
        annotation(Documentation(info="<html></html>"));
      end getArrayRows2D;

      partial function getArrayColumns2D "Get second dimension of 2D array in MAT file"
        extends Modelica.Icons.Function;
        input String varName "Variable name";
        input Types.ExternMATFile mat "External MATLAB MAT file object";
        output Integer n "Number of columns in array";
        protected
          Integer m[1] "Number of rows in array";
        annotation(Documentation(info="<html></html>"));
      end getArrayColumns2D;
    end MAT;

    package SSV "SSV file interfaces"
      extends Modelica.Icons.InterfacesPackage;
      partial record Base "Interface for SSV file"
        replaceable function getReal = SSV.getReal "Get scalar Real value from SSV file" annotation(Documentation(info="<html></html>"));
        replaceable function getInteger = SSV.getInteger "Get scalar Integer value from SSV file" annotation(Documentation(info="<html></html>"));
        replaceable function getBoolean = SSV.getBoolean "Get scalar Boolean value from SSV file" annotation(Documentation(info="<html></html>"));
        replaceable function getString = SSV.getString "Get scalar String value from SSV file" annotation(Documentation(info="<html></html>"));
      annotation(
        Documentation(info="<html><p>Base record that defines the function interfaces for <a href=\"modelica://ExternData.SSVFile\">SSVFile</a>.</p></html>"),
        Icon(graphics={
          Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
          Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillColor={255,128,0},fillPattern=FillPattern.Solid)}));
      end Base;

      partial function getReal "Get scalar Real value from SSV file"
        extends Interfaces.partialGetReal;
        input Types.ExternXML2File ssv "External XML2 file object";
        input String nameSpace "SSV name space";
        annotation(Documentation(info="<html></html>"));
      end getReal;

      partial function getInteger "Get scalar Integer value from SSV file"
        extends Interfaces.partialGetInteger;
        input Types.ExternXML2File ssv "External XML2 file object";
        input String nameSpace "SSV name space";
        annotation(Documentation(info="<html></html>"));
      end getInteger;

      partial function getBoolean "Get scalar Boolean value from SSV file"
        extends Interfaces.partialGetBoolean;
        input Types.ExternXML2File ssv "External XML2 file object";
        input String nameSpace "SSV name space";
        protected
          String aux;
        annotation(Documentation(info="<html></html>"));
      end getBoolean;

      partial function getString "Get scalar String value from SSV file"
        extends Interfaces.partialGetString;
        input Types.ExternXML2File ssv "External XML2 file object";
        input String nameSpace "SSV name space";
        annotation(Documentation(info="<html></html>"));
      end getString;
    end SSV;

    package XLS "Excel XLS file interfaces"
      extends Modelica.Icons.InterfacesPackage;
      partial record Base "Interface for Excel XLS file"
        replaceable function getReal = XLS.getReal "Get scalar Real value from Excel XLS file" annotation(Documentation(info="<html></html>"));
        replaceable function getRealArray2D = XLS.getRealArray2D "Get 2D Real values from Excel XLS file" annotation(Documentation(info="<html></html>"));
        replaceable function getInteger = XLS.getInteger "Get scalar Integer value from Excel XLS file" annotation(Documentation(info="<html></html>"));
        replaceable function getBoolean = XLS.getBoolean "Get scalar Boolean value from Excel XLS file" annotation(Documentation(info="<html></html>"));
        replaceable function getString = XLS.getString "Get scalar String value from Excel XLS file" annotation(Documentation(info="<html></html>"));
        replaceable function getArraySize2D = XLS.getArraySize2D "Get the size of a 2D array in a Excel XLS file" annotation(Documentation(info="<html></html>"));
        replaceable function getArrayRows2D = XLS.getArrayRows2D "Get first dimension of 2D array in Excel XLS file" annotation(Documentation(info="<html></html>"));
        replaceable function getArrayColumns2D = XLS.getArrayColumns2D "Get second dimension of 2D array in Excel XLS file" annotation(Documentation(info="<html></html>"));
      annotation(
        Documentation(info="<html><p>Base record that defines the function interfaces for <a href=\"modelica://ExternData.XLSFile\">XLSFile</a>.</p></html>"),
        Icon(graphics={
          Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
          Rectangle(extent={{80,70},{40,50}},lineColor={215,215,215},fillColor={215,215,215},fillPattern=FillPattern.Solid),
          Rectangle(extent={{20,70},{-20,50}},lineColor={215,215,215},fillColor={215,215,215},fillPattern=FillPattern.Solid),
          Rectangle(extent={{-40,40},{-80,20}},lineColor={215,215,215},fillColor={215,215,215},fillPattern=FillPattern.Solid),
          Rectangle(extent={{-40,10},{-80,-10}},lineColor={215,215,215},fillColor={215,215,215},fillPattern=FillPattern.Solid),
          Rectangle(extent={{-40,-20},{-80,-40}},lineColor={215,215,215},fillColor={215,215,215},fillPattern=FillPattern.Solid),
          Rectangle(extent={{-40,-50},{-80,-70}},lineColor={215,215,215},fillColor={215,215,215},fillPattern=FillPattern.Solid),
          Rectangle(extent={{40,40},{80,20}},lineColor={148,215,187},fillColor={148,215,187},fillPattern=FillPattern.Solid),
          Rectangle(extent={{40,10},{80,-10}},lineColor={148,215,187},fillColor={148,215,187},fillPattern=FillPattern.Solid),
          Rectangle(extent={{40,-20},{80,-40}},lineColor={148,215,187},fillColor={148,215,187},fillPattern=FillPattern.Solid),
          Rectangle(extent={{40,-50},{80,-70}},lineColor={148,215,187},fillColor={148,215,187},fillPattern=FillPattern.Solid),
          Rectangle(extent={{-20,40},{20,20}},lineColor={148,215,187},fillColor={148,215,187},fillPattern=FillPattern.Solid),
          Rectangle(extent={{-20,10},{20,-10}},lineColor={148,215,187},fillColor={148,215,187},fillPattern=FillPattern.Solid),
          Rectangle(extent={{-20,-20},{20,-40}},lineColor={148,215,187},fillColor={148,215,187},fillPattern=FillPattern.Solid),
          Rectangle(extent={{-20,-50},{20,-70}},lineColor={148,215,187},fillColor={148,215,187},fillPattern=FillPattern.Solid),
          Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillColor={0,127,0},fillPattern=FillPattern.Solid)}));
      end Base;

      partial function getReal "Get scalar Real value from Excel XLS file"
        extends Modelica.Icons.Function;
        input String cellAddress = "A1" "Cell address";
        input String sheetName = "" "Sheet name";
        input Types.ExternXLSFile xls "External Excel XLS file object";
        output Real y "Real value";
        output Boolean exist "= true, if cellAddress exits; = false, if it does not exist and y is set to 0.0";
        annotation(Documentation(info="<html></html>"));
      end getReal;

      partial function getRealArray2D "Get 2D Real values from Excel XLS file"
        extends Modelica.Icons.Function;
        input String cellAddress = "A1" "Start cell address";
        input String sheetName = "" "Sheet name";
        input Integer m = 1 "Number of rows";
        input Integer n = 1 "Number of columns";
        input Types.ExternXLSFile xls "External Excel XLS file object";
        output Real y[m,n] "2D Real values";
        annotation(Documentation(info="<html></html>"));
      end getRealArray2D;

      partial function getInteger "Get scalar Integer value from Excel XLS file"
        extends Modelica.Icons.Function;
        input String cellAddress = "A1" "Cell address";
        input String sheetName = "" "Sheet name";
        input Types.ExternXLSFile xls "External Excel XLS file object";
        output Integer y "Integer value";
        output Boolean exist "= true, if cellAddress exits; = false, if it does not exist and y is set to 0";
        annotation(Documentation(info="<html></html>"));
      end getInteger;

      partial function getBoolean "Get scalar Boolean value from Excel XLS file"
        extends Modelica.Icons.Function;
        input String cellAddress = "A1" "Cell address";
        input String sheetName = "" "Sheet name";
        input Types.ExternXLSFile xls "External Excel XLS file object";
        output Boolean y "Boolean value";
        output Boolean exist "= true, if cellAddress exits; = false, if it does not exist and y is set to false";
        protected
          Real aux;
        annotation(Documentation(info="<html></html>"));
      end getBoolean;

      partial function getString "Get scalar String value from Excel XLS file"
        extends Modelica.Icons.Function;
        input String cellAddress = "A1" "Cell address";
        input String sheetName = "" "Sheet name";
        input Types.ExternXLSFile xls "External Excel XLS file object";
        output String str "String value";
        output Boolean exist "= true, if cellAddress exits; = false, if it does not exist and y is set to an empty string";
        annotation(Documentation(info="<html></html>"));
      end getString;

      partial function getArraySize2D "Get dimensions of 2D array in Excel XLS file"
        extends Modelica.Icons.Function;
        input String sheetName = "" "Sheet name";
        input Types.ExternXLSFile xls "External Excel XLS file object";
        output Integer m "Number of rows in array";
        output Integer n "Number of columns in array";
        annotation(Documentation(info="<html></html>"));
      end getArraySize2D;

      partial function getArrayRows2D "Get first dimension of 2D array in Excel XLS file"
        extends Modelica.Icons.Function;
        input String sheetName = "" "Sheet name";
        input Types.ExternXLSFile xls "External Excel XLS file object";
        output Integer m "Number of rows in array";
        protected
          Integer n[1] "Number of columns in array";
        annotation(Documentation(info="<html></html>"));
      end getArrayRows2D;

      partial function getArrayColumns2D "Get second dimension of 2D array in Excel XLS file"
        extends Modelica.Icons.Function;
        input String sheetName = "" "Sheet name";
        input Types.ExternXLSFile xls "External Excel XLS file object";
        output Integer n "Number of columns in array";
        protected
          Integer m[1] "Number of rows in array";
        annotation(Documentation(info="<html></html>"));
      end getArrayColumns2D;
    end XLS;

    package XLSX "Excel XLSX file interfaces"
      extends Modelica.Icons.InterfacesPackage;
      partial record Base "Interface for Excel XLSX file"
        replaceable function getReal = XLSX.getReal "Get scalar Real value from Excel XLSX file" annotation(Documentation(info="<html></html>"));
        replaceable function getRealArray2D = XLSX.getRealArray2D "Get 2D Real values from Excel XLSX file" annotation(Documentation(info="<html></html>"));
        replaceable function getInteger = XLSX.getInteger "Get scalar Integer value from Excel XLSX file" annotation(Documentation(info="<html></html>"));
        replaceable function getBoolean = XLSX.getBoolean"Get scalar Boolean value from Excel XLSX file" annotation(Documentation(info="<html></html>"));
        replaceable function getString = XLSX.getString "Get scalar String value from Excel XLSX file" annotation(Documentation(info="<html></html>"));
        replaceable function getArraySize2D = XLSX.getArraySize2D "Get the size of a 2D array in a Excel XLSX file" annotation(Documentation(info="<html></html>"));
        replaceable function getArrayRows2D = XLSX.getArrayRows2D "Get first dimension of 2D array in Excel XLSX file" annotation(Documentation(info="<html></html>"));
        replaceable function getArrayColumns2D = XLSX.getArrayColumns2D "Get second dimension of 2D array in Excel XLSX file" annotation(Documentation(info="<html></html>"));
      annotation(
        Documentation(info="<html><p>Base record that defines the function interfaces for <a href=\"modelica://ExternData.XLSXFile\">XLSXFile</a>.</p></html>"),
        Icon(graphics={
          Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
          Rectangle(extent={{80,70},{40,50}},lineColor={215,215,215},fillColor={215,215,215},fillPattern=FillPattern.Solid),
          Rectangle(extent={{20,70},{-20,50}},lineColor={215,215,215},fillColor={215,215,215},fillPattern=FillPattern.Solid),
          Rectangle(extent={{-40,40},{-80,20}},lineColor={215,215,215},fillColor={215,215,215},fillPattern=FillPattern.Solid),
          Rectangle(extent={{-40,10},{-80,-10}},lineColor={215,215,215},fillColor={215,215,215},fillPattern=FillPattern.Solid),
          Rectangle(extent={{-40,-20},{-80,-40}},lineColor={215,215,215},fillColor={215,215,215},fillPattern=FillPattern.Solid),
          Rectangle(extent={{-40,-50},{-80,-70}},lineColor={215,215,215},fillColor={215,215,215},fillPattern=FillPattern.Solid),
          Rectangle(extent={{40,40},{80,20}},lineColor={148,215,187},fillColor={148,215,187},fillPattern=FillPattern.Solid),
          Rectangle(extent={{40,10},{80,-10}},lineColor={148,215,187},fillColor={148,215,187},fillPattern=FillPattern.Solid),
          Rectangle(extent={{40,-20},{80,-40}},lineColor={148,215,187},fillColor={148,215,187},fillPattern=FillPattern.Solid),
          Rectangle(extent={{40,-50},{80,-70}},lineColor={148,215,187},fillColor={148,215,187},fillPattern=FillPattern.Solid),
          Rectangle(extent={{-20,40},{20,20}},lineColor={148,215,187},fillColor={148,215,187},fillPattern=FillPattern.Solid),
          Rectangle(extent={{-20,10},{20,-10}},lineColor={148,215,187},fillColor={148,215,187},fillPattern=FillPattern.Solid),
          Rectangle(extent={{-20,-20},{20,-40}},lineColor={148,215,187},fillColor={148,215,187},fillPattern=FillPattern.Solid),
          Rectangle(extent={{-20,-50},{20,-70}},lineColor={148,215,187},fillColor={148,215,187},fillPattern=FillPattern.Solid),
          Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillColor={160,255,255},fillPattern=FillPattern.Solid)}));
      end Base;

      partial function getReal "Get scalar Real value from Excel XLSX file"
        extends Modelica.Icons.Function;
        input String cellAddress = "A1" "Cell address";
        input String sheetName = "" "Sheet name";
        input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
        output Real y "Real value";
        output Boolean exist "= true, if cellAddress exits; = false, if it does not exist and y is set to 0.0";
        annotation(Documentation(info="<html></html>"));
      end getReal;

      partial function getRealArray2D "Get 2D Real values from Excel XLSX file"
        extends Modelica.Icons.Function;
        input String cellAddress = "A1" "Start cell address";
        input String sheetName = "" "Sheet name";
        input Integer m = 1 "Number of rows";
        input Integer n = 1 "Number of columns";
        input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
        output Real y[m,n] "2D Real values";
        annotation(Documentation(info="<html></html>"));
      end getRealArray2D;

      partial function getInteger "Get scalar Integer value from Excel XLSX file"
        extends Modelica.Icons.Function;
        input String cellAddress = "A1" "Cell address";
        input String sheetName = "" "Sheet name";
        input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
        output Integer y "Integer value";
        output Boolean exist "= true, if cellAddress exits; = false, if it does not exist and y is set 0";
        annotation(Documentation(info="<html></html>"));
      end getInteger;

      partial function getBoolean "Get scalar Boolean value from Excel XLSX file"
        extends Modelica.Icons.Function;
        input String cellAddress = "A1" "Cell address";
        input String sheetName = "" "Sheet name";
        input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
        output Boolean y "Boolean value";
        output Boolean exist "= true, if cellAddress exits; = false, if it does not exist and y is set to false";
        protected
          Real aux;
        annotation(Documentation(info="<html></html>"));
      end getBoolean;

      partial function getString "Get scalar String value from Excel XLSX file"
        extends Modelica.Icons.Function;
        input String cellAddress = "A1" "Cell address";
        input String sheetName = "" "Sheet name";
        input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
        output String str "String value";
        output Boolean exist "= true, if cellAddress exits; = false, if it does not exist and y is set to an empty string";
        annotation(Documentation(info="<html></html>"));
      end getString;

      partial function getArraySize2D "Get dimensions of 2D array in Excel XLSX file"
        extends Modelica.Icons.Function;
        input String sheetName = "" "Sheet name";
        input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
        output Integer m "Number of rows in array";
        output Integer n "Number of columns in array";
        annotation(Documentation(info="<html></html>"));
      end getArraySize2D;

      partial function getArrayRows2D "Get first dimension of 2D array in Excel XLSX file"
        extends Modelica.Icons.Function;
        input String sheetName="" "Sheet name";
        input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
        output Integer m "Number of rows in array";
        protected
          Integer n[1] "Number of columns in array";
        annotation(Documentation(info="<html></html>"));
      end getArrayRows2D;

      partial function getArrayColumns2D "Get second dimension of 2D array in Excel XLSX file"
        extends Modelica.Icons.Function;
        input String sheetName = "" "Sheet name";
        input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
        output Integer n "Number of columns in array";
        protected
          Integer m[1] "Number of rows in array";
        annotation(Documentation(info="<html></html>"));
      end getArrayColumns2D;
    end XLSX;

    package XML "XML file interfaces"
      extends Modelica.Icons.InterfacesPackage;
      partial record Base "Interface for XML file"
        replaceable function getReal = XML.getReal "Get scalar Real value from XML file" annotation(Documentation(info="<html></html>"));
        replaceable function getRealArray1D = XML.getRealArray1D "Get 1D Real values from XML file" annotation(Documentation(info="<html></html>"));
        replaceable function getRealArray2D = XML.getRealArray2D "Get 2D Real values from XML file" annotation(Documentation(info="<html></html>"));
        replaceable function getInteger = XML.getInteger "Get scalar Integer value from XML file" annotation(Documentation(info="<html></html>"));
        replaceable function getBoolean = XML.getBoolean "Get scalar Boolean value from XML file" annotation(Documentation(info="<html></html>"));
        replaceable function getString = XML.getString "Get scalar String value from XML file" annotation(Documentation(info="<html></html>"));
        replaceable function getArraySize1D = XML.getArraySize1D "Get the size of a 1D array in a XML file" annotation(Documentation(info="<html></html>"));
        replaceable function getArraySize2D = XML.getArraySize2D "Get the size of a 2D array in a XML file" annotation(Documentation(info="<html></html>"));
        replaceable function getArrayRows2D = XML.getArrayRows2D "Get first dimension of 2D array in XML file" annotation(Documentation(info="<html></html>"));
        replaceable function getArrayColumns2D = XML.getArrayColumns2D "Get second dimension of 2D array in XML file" annotation(Documentation(info="<html></html>"));
      annotation(
        Documentation(info="<html><p>Base record that defines the function interfaces for <a href=\"modelica://ExternData.XMLFile\">XMLFile</a>.</p></html>"),
        Icon(graphics={
          Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
          Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillColor={255,128,0},fillPattern=FillPattern.Solid)}));
      end Base;

      partial function getReal "Get scalar Real value from XML file"
        extends Interfaces.partialGetReal;
        input Types.ExternXMLFile xml "External XML file object";
        annotation(Documentation(info="<html></html>"));
      end getReal;

      partial function getRealArray1D "Get 1D Real values from XML file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer n = 1 "Number of elements";
        input Types.ExternXMLFile xml "External XML file object";
        output Real y[n] "1D Real values";
        annotation(Documentation(info="<html></html>"));
      end getRealArray1D;

      partial function getRealArray2D "Get 2D Real values from XML file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer m = 1 "Number of rows";
        input Integer n = 1 "Number of columns";
        input Types.ExternXMLFile xml "External XML file object";
        output Real y[m,n] "2D Real values";
        annotation(Documentation(info="<html></html>"));
      end getRealArray2D;

      partial function getInteger "Get scalar Integer value from XML file"
        extends Interfaces.partialGetInteger;
        input Types.ExternXMLFile xml "External XML file object";
        annotation(Documentation(info="<html></html>"));
      end getInteger;

      partial function getBoolean "Get scalar Boolean value from XML file"
        extends Interfaces.partialGetBoolean;
        input Types.ExternXMLFile xml "External XML file object";
        protected
          Real aux;
        annotation(Documentation(info="<html></html>"));
      end getBoolean;

      partial function getString "Get scalar String value from XML file"
        extends Interfaces.partialGetString;
        input Types.ExternXMLFile xml "External XML file object";
        annotation(Documentation(info="<html></html>"));
      end getString;

      partial function getArraySize1D "Get length of 1D array in XML file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Types.ExternXMLFile xml "External XML file object";
        output Integer n "Number of elements in array";
        annotation(Documentation(info="<html></html>"));
      end getArraySize1D;

      partial function getArraySize2D "Get dimensions of 2D array in XML file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Types.ExternXMLFile xml "External XML file object";
        output Integer m "Number of rows in array";
        output Integer n "Number of columns in array";
        annotation(Documentation(info="<html></html>"));
      end getArraySize2D;

      partial function getArrayRows2D "Get first dimension of 2D array in XML file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Types.ExternXMLFile xml "External XML file object";
        output Integer m "Number of rows in array";
        protected
          Integer n[1] "Number of columns in array";
        annotation(Documentation(info="<html></html>"));
      end getArrayRows2D;

      partial function getArrayColumns2D "Get second dimension of 2D array in XML file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Types.ExternXMLFile xml "External XML file object";
        output Integer n "Number of columns in array";
        protected
          Integer m[1] "Number of rows in array";
        annotation(Documentation(info="<html></html>"));
      end getArrayColumns2D;
    end XML;

    package XML2 "XML2 file interfaces"
      extends Modelica.Icons.InterfacesPackage;
      partial record Base "Interface for XML2 file"
        replaceable function getReal = XML2.getReal "Get scalar Real value from XML file" annotation(Documentation(info="<html></html>"));
        replaceable function getRealArray1D = XML2.getRealArray1D "Get 1D Real values from XML file" annotation(Documentation(info="<html></html>"));
        replaceable function getRealArray2D = XML2.getRealArray2D "Get 2D Real values from XML file" annotation(Documentation(info="<html></html>"));
        replaceable function getInteger = XML2.getInteger "Get scalar Integer value from XML file" annotation(Documentation(info="<html></html>"));
        replaceable function getBoolean = XML2.getBoolean "Get scalar Boolean value from XML file" annotation(Documentation(info="<html></html>"));
        replaceable function getString = XML2.getString "Get scalar String value from XML file" annotation(Documentation(info="<html></html>"));
        replaceable function getArraySize1D = XML2.getArraySize1D "Get the size of a 1D array in a XML file" annotation(Documentation(info="<html></html>"));
        replaceable function getArraySize2D = XML2.getArraySize2D "Get the size of a 2D array in a XML file" annotation(Documentation(info="<html></html>"));
        replaceable function getArrayRows2D = XML2.getArrayRows2D "Get first dimension of 2D array in XML file" annotation(Documentation(info="<html></html>"));
        replaceable function getArrayColumns2D = XML2.getArrayColumns2D "Get second dimension of 2D array in XML file" annotation(Documentation(info="<html></html>"));
      annotation(
        Documentation(info="<html><p>Base record that defines the function interfaces for <a href=\"modelica://ExternData.XML2File\">XML2File</a>.</p></html>"),
        Icon(graphics={
          Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
          Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillColor={255,128,0},fillPattern=FillPattern.Solid)}));
      end Base;

      partial function getReal "Get scalar Real value from XML file"
        extends Interfaces.partialGetReal;
        input Types.ExternXML2File xml "External XML2 file object";
        annotation(Documentation(info="<html></html>"));
      end getReal;

      partial function getRealArray1D "Get 1D Real values from XML file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer n = 1 "Number of elements";
        input Types.ExternXML2File xml "External XML2 file object";
        output Real y[n] "1D Real values";
        annotation(Documentation(info="<html></html>"));
      end getRealArray1D;

      partial function getRealArray2D "Get 2D Real values from XML file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer m = 1 "Number of rows";
        input Integer n = 1 "Number of columns";
        input Types.ExternXML2File xml "External XML2 file object";
        output Real y[m,n] "2D Real values";
        annotation(Documentation(info="<html></html>"));
      end getRealArray2D;

      partial function getInteger "Get scalar Integer value from XML file"
        extends Interfaces.partialGetInteger;
        input Types.ExternXML2File xml "External XML2 file object";
        annotation(Documentation(info="<html></html>"));
      end getInteger;

      partial function getBoolean "Get scalar Boolean value from XML file"
        extends Interfaces.partialGetBoolean;
        input Types.ExternXML2File xml "External XML2 file object";
        protected
          Real aux;
        annotation(Documentation(info="<html></html>"));
      end getBoolean;

      partial function getString "Get scalar String value from XML file"
        extends Interfaces.partialGetString;
        input Types.ExternXML2File xml "External XML2 file object";
        annotation(Documentation(info="<html></html>"));
      end getString;

      partial function getArraySize1D "Get length of 1D array in XML file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Types.ExternXML2File xml "External XML2 file object";
        output Integer n "Number of elements in array";
        annotation(Documentation(info="<html></html>"));
      end getArraySize1D;

      partial function getArraySize2D "Get dimensions of 2D array in XML file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Types.ExternXML2File xml "External XML2 file object";
        output Integer m "Number of rows in array";
        output Integer n "Number of columns in array";
        annotation(Documentation(info="<html></html>"));
      end getArraySize2D;

      partial function getArrayRows2D "Get first dimension of 2D array in XML file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Types.ExternXML2File xml "External XML2 file object";
        output Integer m "Number of rows in array";
        protected
          Integer n[1] "Number of columns in array";
        annotation(Documentation(info="<html></html>"));
      end getArrayRows2D;

      partial function getArrayColumns2D "Get second dimension of 2D array in XML file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Types.ExternXML2File xml "External XML2 file object";
        output Integer n "Number of columns in array";
        protected
          Integer m[1] "Number of rows in array";
        annotation(Documentation(info="<html></html>"));
      end getArrayColumns2D;
    end XML2;

    partial function partialGetReal
      extends Modelica.Icons.Function;
      input String varName "Key";
      output Real y "Real value";
      output Boolean exist "= true, if varName exits; = false, if it does not exist and y is set to 0.0";
    end partialGetReal;

    partial function partialGetInteger
      extends Modelica.Icons.Function;
      input String varName "Key";
      output Integer y "Integer value";
      output Boolean exist "= true, if varName exits; = false, if it does not exist and y is set to 0";
    end partialGetInteger;

    partial function partialGetBoolean
      extends Modelica.Icons.Function;
      input String varName "Key";
      output Boolean y "Boolean value";
      output Boolean exist "= true, if varName exits; = false, if it does not exist and y is set to false";
    end partialGetBoolean;

    partial function partialGetString
      extends Modelica.Icons.Function;
      input String varName "Key";
      output String str "String value";
      output Boolean exist "= true, if varName exits; = false, if it does not exist and str is set to a an empty string";
    end partialGetString;
  end Interfaces;

  package Types "Types"
    extends Modelica.Icons.TypesPackage;

    type Diagnostics = enumeration(
      None "No diagnostic messages",
      Debug "Print diagnostic messages as information",
      Warning "Print diagnostic messages as warning",
      Error "Print diagnostic messages as error")
      "Enumeration defining the diagnostics";

    class ExternCSVFile "External CSV file object"
      extends ExternalObject;
      function constructor "Parse CSV file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String delimiter = "," "Column delimiter character";
        input String quotation = "\"" "Quotation character";
        input Integer nHeaderLines = 0 "Number of header lines to ignore";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        output ExternCSVFile csv "External CSV file object";
        external "C" csv=ED_createCSV(fileName, delimiter, quotation, nHeaderLines, verboseRead) annotation(
          __iti_dll = "ITI_ED_CSVFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_CSVFile.h\"",
          Library = {"ED_CSVFile", "bsxml-json"});
      end constructor;

      function destructor "Clean up"
        extends Modelica.Icons.Function;
        input ExternCSVFile csv "External CSV file object";
        external "C" ED_destroyCSV(csv) annotation(
          __iti_dll = "ITI_ED_CSVFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_CSVFile.h\"",
          Library = {"ED_CSVFile", "bsxml-json"});
      end destructor;
    end ExternCSVFile;

    class ExternINIFile "External INI file object"
      extends ExternalObject;
      function constructor "Parse INI file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Diagnostics detectMissingData = Diagnostics.Warning "Print diagnostic message in case of missing data";
        output ExternINIFile ini "External INI file object";
        external "C" ini=ED_createINI(fileName, verboseRead, detectMissingData) annotation(
          __iti_dll = "ITI_ED_INIFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_INIFile.h\"",
          Library = {"ED_INIFile", "bsxml-json"});
      end constructor;

      function destructor "Clean up"
        extends Modelica.Icons.Function;
        input ExternINIFile ini "External INI file object";
        external "C" ED_destroyINI(ini) annotation(
          __iti_dll = "ITI_ED_INIFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_INIFile.h\"",
          Library = {"ED_INIFile", "bsxml-json"});
      end destructor;
    end ExternINIFile;

    class ExternJSONFile "External JSON file object"
      extends ExternalObject;
      function constructor "Parse JSON file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Diagnostics detectMissingData = Diagnostics.Warning "Print diagnostic message in case of missing data";
        output ExternJSONFile json "External JSON file object";
        external "C" json=ED_createJSON(fileName, verboseRead, detectMissingData) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end constructor;

      function destructor "Clean up"
        extends Modelica.Icons.Function;
        input ExternJSONFile json "External JSON file object";
        external "C" ED_destroyJSON(json) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end destructor;
    end ExternJSONFile;

    class ExternMATFile "External MAT file object"
      extends ExternalObject;
      function constructor "Only copy MAT file name (File not yet opened)"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        output ExternMATFile mat "External MATLAB MAT file object";
        external "C" mat=ED_createMAT(fileName, verboseRead) annotation(
          __iti_dll = "ITI_ED_MATFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_MATFile.h\"",
          Library = {"ED_MATFile", "hdf5", "zlib", "dl"});
      end constructor;

      function destructor "Clean up"
        extends Modelica.Icons.Function;
        input ExternMATFile mat "External MATLAB MAT file object";
        external "C" ED_destroyMAT(mat) annotation(
          __iti_dll = "ITI_ED_MATFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_MATFile.h\"",
          Library = {"ED_MATFile", "hdf5", "zlib", "dl"});
      end destructor;
    end ExternMATFile;

    class ExternXLSFile "External XLS file object"
      extends ExternalObject;
      function constructor "Open Excel XLS file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String encoding = "UTF-8" "Encoding";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Diagnostics detectMissingData = Diagnostics.Warning "Print diagnostic message in case of missing data";
        output ExternXLSFile xls "External Excel XLS file object";
        external "C" xls=ED_createXLS(fileName, encoding, verboseRead, detectMissingData) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSFile.h\"",
          Library = {"ED_XLSFile", "xlsreader"});
      end constructor;

      function destructor "Clean up"
        extends Modelica.Icons.Function;
        input ExternXLSFile xls "External Excel XLS file object";
        external "C" ED_destroyXLS(xls) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSFile.h\"",
          Library = {"ED_XLSFile", "xlsreader"});
      end destructor;
    end ExternXLSFile;

    class ExternXLSXFile "External XLSX file object"
      extends ExternalObject;
      function constructor "Parse Excel XLSX file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Diagnostics detectMissingData = Diagnostics.Warning "Print diagnostic message in case of missing data";
        output ExternXLSXFile xlsx "External Excel XLSX file object";
        external "C" xlsx=ED_createXLSX(fileName, verboseRead, detectMissingData) annotation(
          __iti_dll = "ITI_ED_XLSXFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSXFile.h\"",
          Library = {"ED_XLSXFile", "bsxml-json", "expat", "minizip", "zlib"});
      end constructor;

      function destructor "Clean up"
        extends Modelica.Icons.Function;
        input ExternXLSXFile xlsx "External Excel XLSX file object";
        external "C" ED_destroyXLSX(xlsx) annotation(
          __iti_dll = "ITI_ED_XLSXFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSXFile.h\"",
          Library = {"ED_XLSXFile", "bsxml-json", "expat", "minizip", "zlib"});
      end destructor;
    end ExternXLSXFile;

    class ExternXMLFile "External XML file object"
      extends ExternalObject;
      function constructor "Parse XML file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Diagnostics detectMissingData = Diagnostics.Warning "Print diagnostic message in case of missing data";
        output ExternXMLFile xml "External XML file object";
        external "C" xml=ED_createXML(fileName, verboseRead, detectMissingData) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end constructor;

      function destructor "Clean up"
        extends Modelica.Icons.Function;
        input ExternXMLFile xml "External XML file object";
        external "C" ED_destroyXML(xml) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end destructor;
    end ExternXMLFile;

    class ExternXML2File "External XML2 file object"
      extends ExternalObject;
      function constructor "Parse XML file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String nameSpace[:,2] = fill("", 0, 2) "XML name spaces";
        input Boolean verboseRead = true "= true, if info message that file is loading is to be printed";
        input Diagnostics detectMissingData = Diagnostics.Warning "Print diagnostic message in case of missing data";
        output ExternXML2File xml "External XML2 file object";
        external "C" xml=ED_createXML2(fileName, nameSpace, size(nameSpace, 1), verboseRead, detectMissingData) annotation(
          __iti_dll = "ITI_ED_XML2File.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XML2File.h\"",
          Library = {"ED_XML2File", "xml2", "zlib"});
      end constructor;

      function destructor "Clean up"
        extends Modelica.Icons.Function;
        input ExternXML2File xml "External XML2 file object";
        external "C" ED_destroyXML2(xml) annotation(
          __iti_dll = "ITI_ED_XML2File.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XML2File.h\"",
          Library = {"ED_XML2File", "xml2", "zlib"});
      end destructor;
    end ExternXML2File;
  end Types;

  annotation(uses(Modelica(version="4.0.0")), version="3.0.5",
    Documentation(info="<html><p>Library <strong>ExternData</strong> is a Modelica utility library for data access of <a href=\"https://en.wikipedia.org/wiki/Comma-separated_values\">CSV</a>, <a href=\"https://en.wikipedia.org/wiki/INI_file\">INI</a>, <a href=\"https://en.wikipedia.org/wiki/JSON\">JSON</a>, <a href=\"https://en.wikipedia.org/wiki/MATLAB\">MATLAB</a> MAT, <a href=\"https://ssp-standard.org/\">SSV</a> (System Structure Parameter Values), TIR, <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel\">Excel</a> <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel#Binary\">XLS</a>/<a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel#XML_Spreadsheet\">XLSX</a> and <a href=\"https://en.wikipedia.org/wiki/XML\">XML</a> files.</p></html>"));
end ExternData;
