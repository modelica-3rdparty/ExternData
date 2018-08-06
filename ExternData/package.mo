// CP: 65001
/* package.mo - Modelica library for data I/O of CSV, INI, JSON, MATLAB MAT, TIR, Excel XLS/XLSX or XML files
 *
 * Copyright (C) 2015-2018, tbeu
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

within;
package ExternData "Library for data I/O of CSV, INI, JSON, MATLAB MAT, TIR, Excel XLS/XLSX or XML files"
  extends Modelica.Icons.Package;
  package UsersGuide "User's Guide"
    extends Modelica.Icons.Information;
    class References "References"
      extends Modelica.Icons.References;
      annotation(Documentation(info="<html><p>The ExternData Modelica library is based on the following third-party C projects</p><ul><li><a href=\"https://github.com/bsapundzhiev/bsxml-json\">bsxml-json</a>&nbsp;-&nbsp;Borislav Sapundzhiev&#039;s fairly simple XML DOM and JSON implementation</li><li><a href=\"https://github.com/libexpat/libexpat\">expat</a>&nbsp;-&nbsp;James Clark&#039;s Expat XML parser library</li><li><a href=\"https://support.hdfgroup.org/HDF5\">hdf5</a>&nbsp;-&nbsp;The HDF Group&#039;s data model, library and file format for storing and managing data</li><li><a href=\"https://github.com/benhoyt/inih\">inih</a>&nbsp;-&nbsp;Ben Hoyt&#039;s simple INI file parser in C</li><li><a href=\"http://sourceforge.net/projects/libxls\">libxls</a>&nbsp;-&nbsp;David Hoerl&#039;s C library for parsing Excel files</li><li><a href=\"http://sourceforge.net/projects/matio\">matio</a>&nbsp;-&nbsp;Christopher Hulbert&#039;s C library for reading and writing MATLAB MAT-files</li><li><a href=\"http://www.winimage.com/zLibDll/minizip.html\">minizip</a>&nbsp;-&nbsp;Gilles Vollant&#039;s Zip and UnZip library</li><li><a href=\"https://github.com/kgabis/parson\">parson</a>&nbsp;-&nbsp;Krzysztof Gabis&#039; lightweight JSON library written in C</li><li><a href=\"https://github.com/troydhanson/uthash\">uthash</a>&nbsp;-&nbsp;Troy D. Hanson&#039;s C macros for hash tables and more</li><li><a href=\"https://github.com/madler/zlib\">zlib</a>&nbsp;-&nbsp;Jean-loup Gailly&#039;s and Mark Adler&#039;s massively spiffy yet delicately unobtrusive compression library</li><li><a href=\"https://github.com/fnoyanisi/zString\">zstring</a>&nbsp;-&nbsp;Fehmi Noyan ISI&#039;s string processing library for C</li></ul></html>"));
    end References;
    class License "Simplified BSD License"
      extends Modelica.Icons.Information;
      annotation(Documentation(info="<html><p>Copyright (C) 2015-2018, tbeu<br>All rights reserved.</p><p>Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:</p><p>1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.</p><p>2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.</p><p>THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.</p></html>"));
    end License;
    class Contact "Contact"
      extends Modelica.Icons.Contact;
      annotation(Documentation(info="<html><p>The ExternData Modelica library is developed by <a href=\"https://github.com/tbeu\">tbeu</a> at <a href=\"https://github.com/tbeu/ExternData\">GitHub</a>.</p></html>"));
    end Contact;
    annotation(DocumentationClass=true,
      Documentation(info="<html><p>Library <strong>ExternData</strong> is a <a href=\"https://en.wikipedia.org/wiki/Modelica\">Modelica</a> utility library to access data stored in <a href=\"https://en.wikipedia.org/wiki/Comma-separated_values\">CSV</a>, <a href=\"https://en.wikipedia.org/wiki/INI_file\">INI</a>, <a href=\"https://en.wikipedia.org/wiki/JSON\">JSON</a>, <a href=\"https://en.wikipedia.org/wiki/MATLAB\">MATLAB</a> MAT, <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel\">Excel</a> <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel#Binary\">XLS</a>/<a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel#XML_Spreadsheet\">XLSX</a> and <a href=\"https://en.wikipedia.org/wiki/XML\">XML</a> files.</p></html>"));
  end UsersGuide;

  record CSVFile "Read data values from CSV file"
    parameter String fileName="" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="Comma-separated values files (*.csv);;Text files (*.txt)",
        caption="Open file")));
    parameter String delimiter="," "Column delimiter character" annotation(choices(choice=" " "Blank", choice="," "Comma", choice="\t" "Horizontal tabulator", choice=";" "Semicolon"));
    parameter String quotation="\"" "Quotation character" annotation(choices(choice="\"" "Double quotation mark", choice="'" "Single quotation mark"));
    parameter Boolean verboseRead=true "= true, if info message that file is loading is to be printed";
    final parameter Types.ExternCSVFile csv=Types.ExternCSVFile(fileName, delimiter, quotation, verboseRead) "External INI file object";
    final function getRealArray2D = Functions.CSV.getRealArray2D(final csv=csv) "Get 2D Real values from CSV file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getArraySize2D = Functions.CSV.getArraySize2D(final csv=csv) "Get the size of a 2D array in a CSV file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getArrayRows2D = Functions.CSV.getArrayRows2D(final csv=csv) "Get first dimension of 2D array in CSV file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getArrayColumns2D = Functions.CSV.getArrayColumns2D(final csv=csv) "Get second dimension of 2D array in CSV file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    annotation(
      Documentation(info="<html><p>Record that wraps the external object <a href=\"modelica://ExternData.Types.ExternCSVFile\">ExternCSVFile</a> and the <a href=\"modelica://ExternData.Functions.CSV\">CSV</a> read function for data access of <a href=\"https://en.wikipedia.org/wiki/Comma-separated_values\">CSV</a> files.</p><p>See <a href=\"modelica://ExternData.Examples.CSVTest\">Examples.CSVTest</a> for an example.</p></html>"),
      defaultComponentName="csvfile",
      defaultComponentPrefixes="inner parameter",
      missingInnerMessage="No \"csvfile\" component is defined, please drag ExternData.CSVFile to the model top level",
      Icon(graphics={
        Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
        Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillPattern=FillPattern.Solid),
        Text(lineColor={0,0,255},extent={{-85,-10},{85,-55}},textString=DynamicSelect("csv", if delimiter == " " then "c s v" elseif delimiter == "," then "c,s,v" elseif delimiter == "\t" then "c\\ts\\tv" elseif delimiter == ";" then "c;s;v" else "csv")),
        Text(lineColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end CSVFile;

  record INIFile "Read data values from INI file"
    parameter String fileName="" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="INI files (*.ini);;Configuration files (*.cfg;*.conf;config.txt);;Text files (*.txt)",
        caption="Open file")));
    parameter Boolean verboseRead=true "= true, if info message that file is loading is to be printed";
    final parameter Types.ExternINIFile ini=Types.ExternINIFile(fileName, verboseRead) "External INI file object";
    final function getReal = Functions.INI.getReal(final ini=ini, final strict=true) "Get scalar Real value from INI file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getInteger = Functions.INI.getInteger(final ini=ini, final strict=true) "Get scalar Integer value from INI file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getBoolean = Functions.INI.getBoolean(final ini=ini, final strict=true) "Get scalar Boolean value from INI file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getString = Functions.INI.getString(final ini=ini) "Get scalar String value from INI file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    annotation(
      Documentation(info="<html><p>Record that wraps the external object <a href=\"modelica://ExternData.Types.ExternINIFile\">ExternINIFile</a> and the <a href=\"modelica://ExternData.Functions.INI\">INI</a> read functions for data access of <a href=\"https://en.wikipedia.org/wiki/INI_file\">INI</a> files.</p><p>See <a href=\"modelica://ExternData.Examples.INITest\">Examples.INITest</a> for an example.</p></html>"),
      defaultComponentName="inifile",
      defaultComponentPrefixes="inner parameter",
      missingInnerMessage="No \"inifile\" component is defined, please drag ExternData.INIFile to the model top level",
      Icon(graphics={
        Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
        Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillPattern=FillPattern.Solid),
        Text(lineColor={0,0,255},extent={{-85,-10},{85,-55}},textString="[ini]"),
        Text(lineColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end INIFile;

  record JSONFile "Read data values from JSON file"
    parameter String fileName="" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="JSON files (*.json)",
        caption="Open file")));
    parameter Boolean verboseRead=true "= true, if info message that file is loading is to be printed";
    final parameter Types.ExternJSONFile json=Types.ExternJSONFile(fileName, verboseRead) "External JSON file object";
    final function getReal = Functions.JSON.getReal(final json=json) "Get scalar Real value from JSON file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getRealArray1D = Functions.JSON.getRealArray1D(final json=json) "Get 1D array Real value from JSON file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getRealArray2D = Functions.JSON.getRealArray2D(final json=json) "Get 2D array Real value from JSON file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getInteger = Functions.JSON.getInteger(final json=json) "Get scalar Integer value from JSON file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getIntegerArray1D = Functions.JSON.getIntegerArray1D(final json=json) "Get 1D array Integer value from JSON file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getIntegerArray2D = Functions.JSON.getIntegerArray2D(final json=json) "Get 2D array Integer value from JSON file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getBoolean = Functions.JSON.getBoolean(final json=json) "Get scalar Boolean value from JSON file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getBooleanArray1D = Functions.JSON.getBooleanArray1D(final json=json) "Get 1D array Boolean value from JSON file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getBooleanArray2D = Functions.JSON.getBooleanArray2D(final json=json) "Get 2D array Boolean value from JSON file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getString = Functions.JSON.getString(final json=json) "Get scalar String value from JSON file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getStringArray1D = Functions.JSON.getStringArray1D(final json=json) "Get 1D array String value from JSON file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getStringArray2D = Functions.JSON.getStringArray2D(final json=json) "Get 2D array String value from JSON file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getArraySize1D = Functions.JSON.getArraySize1D(final json=json) "Get the size of a 1D array in a JSON file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getArraySize2D = Functions.JSON.getArraySize2D(final json=json) "Get the size of a 2D array in a JSON file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getArrayRows2D = Functions.JSON.getArrayRows2D(final json=json) "Get first dimension of 2D array in JSON file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getArrayColumns2D = Functions.JSON.getArrayColumns2D(final json=json) "Get second dimension of 2D array in JSON file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    annotation(
      Documentation(info="<html><p>Record that wraps the external object <a href=\"modelica://ExternData.Types.ExternJSONFile\">ExternJSONFile</a> and the <a href=\"modelica://ExternData.Functions.JSON\">JSON</a> read functions for data access of <a href=\"https://en.wikipedia.org/wiki/JSON\">JSON</a> files.</p><p>See <a href=\"modelica://ExternData.Examples.JSONTest\">Examples.JSONTest</a> for an example.</p></html>"),
      defaultComponentName="jsonfile",
      defaultComponentPrefixes="inner parameter",
      missingInnerMessage="No \"jsonfile\" component is defined, please drag ExternData.JSONFile to the model top level",
      Icon(graphics={
        Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
        Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillPattern=FillPattern.Solid),
        Text(lineColor={0,0,255},extent={{-85,-10},{85,-55}},textString="{\"json\"}"),
        Text(lineColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end JSONFile;

  record MATFile "Read data values from MATLAB MAT-file"
    parameter String fileName="" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="MATLAB MAT-files (*.mat)",
        caption="Open file")));
    parameter Boolean verboseRead=true "= true, if info message that file is loading is to be printed";
    final parameter Types.ExternMATFile mat=Types.ExternMATFile(fileName, verboseRead) "External MAT file object";
    final function getRealArray2D = Functions.MAT.getRealArray2D(final mat=mat) "Get 2D Real values from MAT-file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getStringArray1D = Functions.MAT.getStringArray1D(final mat=mat) "Get 1D String values from MAT-file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getArraySize2D = Functions.MAT.getArraySize2D(final mat=mat) "Get the size of a 2D array in a MAT file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getArrayRows2D = Functions.MAT.getArrayRows2D(final mat=mat) "Get first dimension of 2D array in MAT file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getArrayColumns2D = Functions.MAT.getArrayColumns2D(final mat=mat) "Get second dimension of 2D array in MAT file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    annotation(
      Documentation(info="<html><p>Record that wraps the external object <a href=\"modelica://ExternData.Types.ExternMATFile\">ExternMATFile</a> and the <a href=\"modelica://ExternData.Functions.MAT\">MAT</a> read functions for data access of <a href=\"https://en.wikipedia.org/wiki/MATLAB\">MATLAB</a> MAT-files.</p><p>See <a href=\"modelica://ExternData.Examples.MATTest\">Examples.MATTest</a> for an example.</p></html>"),
      defaultComponentName="matfile",
      defaultComponentPrefixes="inner parameter",
      missingInnerMessage="No \"matfile\" component is defined, please drag ExternData.MATFile to the model top level",
      Icon(graphics={
        Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
        Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillColor={241,219,48},fillPattern=FillPattern.Solid),
        Rectangle(extent={{-80,20},{80,-80}},lineColor={248,236,140},fillColor={241,219,48},fillPattern=FillPattern.HorizontalCylinder),
        Rectangle(extent={{-80,20},{80,-80}}),
        Line(points={{-40,20},{-40,-80}}),
        Line(points={{0,20},{0,-80}}),
        Line(points={{40,20},{40,-80}}),
        Line(points={{-80,-30},{80,-30}}),
        Text(extent={{5,85},{65,40}},textString="mat"),
        Text(lineColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end MATFile;

  record XLSFile "Read data values from Excel XLS file"
    parameter String fileName="" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="Excel files (*.xls)",
        caption="Open file")));
    parameter String encoding="UTF-8" "Encoding";
    parameter Boolean verboseRead=true "= true, if info message that file is loading is to be printed";
    final parameter Types.ExternXLSFile xls=Types.ExternXLSFile(fileName, encoding, verboseRead) "External Excel XLS file object";
    final function getReal = Functions.XLS.getReal(final xls=xls) "Get scalar Real value from Excel XLS file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getRealArray2D = Functions.XLS.getRealArray2D(final xls=xls) "Get 2D Real values from Excel XLS file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getInteger = Functions.XLS.getInteger(final xls=xls) "Get scalar Integer value from Excel XLS file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getBoolean = Functions.XLS.getBoolean(final xls=xls) "Get scalar Boolean value from Excel XLS file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getString = Functions.XLS.getString(final xls=xls) "Get scalar String value from Excel XLS file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getArraySize2D = Functions.XLS.getArraySize2D(final xls=xls) "Get the size of a 2D array in a Excel XLS file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getArrayRows2D = Functions.XLS.getArrayRows2D(final xls=xls) "Get first dimension of 2D array in Excel XLS file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getArrayColumns2D = Functions.XLS.getArrayColumns2D(final xls=xls) "Get second dimension of 2D array in Excel XLS file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    annotation(
      Documentation(info="<html><p>Record that wraps the external object <a href=\"modelica://ExternData.Types.ExternXLSFile\">ExternXLSFile</a> and the <a href=\"modelica://ExternData.Functions.XLS\">XLS</a> read functions for data access of <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel\">Excel</a> <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel#Binary\">XLS</a> files.</p><p>See <a href=\"modelica://ExternData.Examples.XLSTest\">Examples.XLSTest</a> for an example.</p></html>"),
      defaultComponentName="xlsfile",
      defaultComponentPrefixes="inner parameter",
      missingInnerMessage="No \"xlsfile\" component is defined, please drag ExternData.XLSFile to the model top level",
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
        Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillColor={0,127,0},fillPattern=FillPattern.Solid),
        Text(lineColor={0,127,0},extent={{-85,-10},{85,-55}},textString="xls"),
        Text(lineColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end XLSFile;

  record XLSXFile "Read data values from Excel XLSX file"
    parameter String fileName="" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="Excel files (*.xlsx)",
        caption="Open file")));
    parameter Boolean verboseRead=true "= true, if info message that file is loading is to be printed";
    final parameter Types.ExternXLSXFile xlsx=Types.ExternXLSXFile(fileName, verboseRead)  "External Excel XLSX file object";
    final function getReal = Functions.XLSX.getReal(final xlsx=xlsx) "Get scalar Real value from Excel XLSX file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getRealArray2D = Functions.XLSX.getRealArray2D(final xlsx=xlsx) "Get 2D Real values from Excel XLSX file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getInteger = Functions.XLSX.getInteger(final xlsx=xlsx) "Get scalar Integer value from Excel XLSX file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getBoolean = Functions.XLSX.getBoolean(final xlsx=xlsx) "Get scalar Boolean value from Excel XLSX file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getString = Functions.XLSX.getString(final xlsx=xlsx) "Get scalar String value from Excel XLSX file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getArraySize2D = Functions.XLSX.getArraySize2D(final xlsx=xlsx) "Get the size of a 2D array in a Excel XLSX file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getArrayRows2D = Functions.XLSX.getArrayRows2D(final xlsx=xlsx) "Get first dimension of 2D array in Excel XLSX file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getArrayColumns2D = Functions.XLSX.getArrayColumns2D(final xlsx=xlsx) "Get second dimension of 2D array in Excel XLSX file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    annotation(
      Documentation(info="<html><p>Record that wraps the external object <a href=\"modelica://ExternData.Types.ExternXLSXFile\">ExternXLSXFile</a> and the <a href=\"modelica://ExternData.Functions.XLSX\">XLSX</a> read functions for data access of <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel\">Excel</a> <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel#XML_Spreadsheet\">XLSX</a> files.</p><p>See <a href=\"modelica://ExternData.Examples.XLSXTest\">Examples.XLSXTest</a> for an example.</p></html>"),
      defaultComponentName="xlsxfile",
      defaultComponentPrefixes="inner parameter",
      missingInnerMessage="No \"xlsxfile\" component is defined, please drag ExternData.XLSXFile to the model top level",
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
        Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillColor={160,255,255},fillPattern=FillPattern.Solid),
        Text(lineColor={0,127,255},extent={{-85,-10},{85,-55}},textString="xlsx"),
        Text(lineColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end XLSXFile;

  record XMLFile "Read data values from XML file"
    parameter String fileName="" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="XML files (*.xml)",
        caption="Open file")));
    parameter Boolean verboseRead=true "= true, if info message that file is loading is to be printed";
    final parameter Types.ExternXMLFile xml=Types.ExternXMLFile(fileName, verboseRead) "External XML file object";
    final function getReal = Functions.XML.getReal(final xml=xml) "Get scalar Real value from XML file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getRealArray1D = Functions.XML.getRealArray1D(final xml=xml) "Get 1D Real values from XML file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getRealArray2D = Functions.XML.getRealArray2D(final xml=xml) "Get 2D Real values from XML file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getInteger = Functions.XML.getInteger(final xml=xml) "Get scalar Integer value from XML file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getBoolean = Functions.XML.getBoolean(final xml=xml) "Get scalar Boolean value from XML file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getString = Functions.XML.getString(final xml=xml) "Get scalar String value from XML file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getArraySize1D = Functions.XML.getArraySize1D(final xml=xml) "Get the size of a 1D array in a XML file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getArraySize2D = Functions.XML.getArraySize2D(final xml=xml) "Get the size of a 2D array in a XML file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getArrayRows2D = Functions.XML.getArrayRows2D(final xml=xml) "Get first dimension of 2D array in XML file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getArrayColumns2D = Functions.XML.getArrayColumns2D(final xml=xml) "Get second dimension of 2D array in XML file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    annotation(
      Documentation(info="<html><p>Record that wraps the external object <a href=\"modelica://ExternData.Types.ExternXMLFile\">ExternXMLFile</a> and the <a href=\"modelica://ExternData.Functions.XML\">XML</a> read functions for data access of <a href=\"https://en.wikipedia.org/wiki/XML\">XML</a> files.</p><p>See <a href=\"modelica://ExternData.Examples.XMLTest\">Examples.XMLTest</a> for an example.</p></html>"),
      defaultComponentName="xmlfile",
      defaultComponentPrefixes="inner parameter",
      missingInnerMessage="No \"xmlfile\" component is defined, please drag ExternData.XMLFile to the model top level",
      Icon(graphics={
        Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
        Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillColor={255,128,0},fillPattern=FillPattern.Solid),
        Text(lineColor={255,128,0},extent={{-85,-10},{85,-55}},textString="<?xml?>"),
        Text(lineColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end XMLFile;

  record TIRFile "Read data values from TIR file"
    parameter String fileName="" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="TIR files (*.tir)",
        caption="Open file")));
    parameter Boolean verboseRead=true "= true, if info message that file is loading is to be printed";
    final parameter Types.ExternINIFile tir=Types.ExternINIFile(fileName, verboseRead) "External TIR file object";
    final function getReal = Functions.INI.getReal(final ini=tir, final strict=false) "Get scalar Real value from TIR file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getInteger = Functions.INI.getInteger(final ini=tir, final strict=false) "Get scalar Integer value from TIR file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getBoolean = Functions.INI.getBoolean(final ini=tir, final strict=false) "Get scalar Boolean value from TIR file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    final function getString = Functions.INI.getString(final ini=tir) "Get scalar String value from TIR file" annotation(__Dymola_interactive=true, Documentation(info="<html></html>"));
    annotation(
      Documentation(info="<html><p>Record that wraps the external object <a href=\"modelica://ExternData.Types.ExternINIFile\">ExternINIFile</a> and the <a href=\"modelica://ExternData.Functions.INI\">INI</a> read functions for data access of tyre data (TIR) files.</p><p>See <a href=\"modelica://ExternData.Examples.TIRTest\">Examples.TIRTest</a> for an example.</p></html>"),
      defaultComponentName="tirfile",
      defaultComponentPrefixes="inner parameter",
      missingInnerMessage="No \"tirfile\" component is defined, please drag ExternData.TIRFile to the model top level",
      Icon(graphics={
        Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
        Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillPattern=FillPattern.Solid),
        Text(lineColor={0,0,255},extent={{-36,84},{88,40}},textString="[tir]"),
        Text(lineColor={0,0,255},extent={{-150,150},{150,110}},textString="%name"),
        Ellipse(extent={{-50,20},{50,-80}},fillPattern=FillPattern.Solid),
        Ellipse(extent={{-30,0},{30,-60}},fillColor={255,255,255},fillPattern=FillPattern.Solid)}));
  end TIRFile;

  package Functions "Functions"
    extends Modelica.Icons.Package;

    package CSV "CSV file functions"
      extends Modelica.Icons.Package;
      function getRealArray2D "Get 2D Real values from CSV file"
        extends Modelica.Icons.Function;
        input Integer m=1 "Number of rows";
        input Integer n=1 "Number of columns";
        input Integer field[2](each min=1)={1,1} "Start field {row, col}";
        input Types.ExternCSVFile csv "External CSV file object";
        output Real y[m,n] "2D Real values";
        external "C" ED_getDoubleArray2DFromCSV(csv, field, y, size(y, 1), size(y, 2)) annotation(
          __iti_dll = "ITI_ED_CSVFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_CSVFile.h\"",
          Library = {"ED_CSVFile", "bsxml-json"});
      end getRealArray2D;

      function getArraySize2D "Get dimensions of 2D array in CSV file"
        extends Modelica.Icons.Function;
        input Types.ExternCSVFile csv "External CSV file object";
        output Integer m "Number of rows in array";
        output Integer n "Number of columns in array";
        external "C" ED_getArray2DDimensionsFromCSV(csv, m, n) annotation(
          __iti_dll = "ITI_ED_CSVFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_CSVFile.h\"",
          Library = {"ED_CSVFile", "bsxml-json"});
      end getArraySize2D;

      function getArrayRows2D "Get first dimension of 2D array in CSV file"
        extends Modelica.Icons.Function;
        input Types.ExternCSVFile csv "External CSV file object";
        output Integer m "Number of rows in array";
        protected
          Integer n[1] "Number of columns in array";
        external "C" ED_getArray2DDimensionsFromCSV(csv, m, n) annotation(
          __iti_dll = "ITI_ED_CSVFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_CSVFile.h\"",
          Library = {"ED_CSVFile", "bsxml-json"});
      end getArrayRows2D;

      function getArrayColumns2D "Get second dimension of 2D array in CSV file"
        extends Modelica.Icons.Function;
        input Types.ExternCSVFile csv "External CSV file object";
        output Integer n "Number of columns in array";
        protected
          Integer m[1] "Number of rows in array";
        external "C" ED_getArray2DDimensionsFromCSV(csv, m, n) annotation(
          __iti_dll = "ITI_ED_CSVFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_CSVFile.h\"",
          Library = {"ED_CSVFile", "bsxml-json"});
      end getArrayColumns2D;
      annotation(Icon(coordinateSystem(preserveAspectRatio=false, extent={{-100,-100},{100,100}}), graphics={Text(lineColor={128,128,128},extent={{-90,-90},{90,90}},textString="f")}));
    end CSV;

    package INI "INI file functions"
      extends Modelica.Icons.Package;
      function getReal "Get scalar Real value from INI file"
        extends Interfaces.partialGetReal;
        input String section="" "Section";
        input Types.ExternINIFile ini "External INI file object";
        input Boolean strict=true "Return an error if there are characters on the line that aren't part of the value";
        external "C" y=ED_getDoubleFromINI(ini, varName, section, strict, exist) annotation(
          __iti_dll = "ITI_ED_INIFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_INIFile.h\"",
          Library = {"ED_INIFile", "bsxml-json"});
      annotation(Documentation(info="<html>
<p>If strict=true and the line being read contains more than a name=value pair this function will return an error.  However, when strict=false it will return the value and ignore the rest of the line.</p>
<p>For example</p>
<code>[example_section]<br />name=65.2   ;this is a comment</code>
<p>In this example, if strict=true we will get an error but if strict=false it will return the value 65.2</p>
</html>"));
      end getReal;

      function getInteger "Get scalar Integer value from INI file"
        extends Interfaces.partialGetInteger;
        input String section="" "Section";
        input Types.ExternINIFile ini "External INI file object";
        input Boolean strict=true "Return an error if there are characters on the line that aren't part of the value";
        external "C" y=ED_getIntFromINI(ini, varName, section, strict, exist) annotation(
          __iti_dll = "ITI_ED_INIFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_INIFile.h\"",
          Library = {"ED_INIFile", "bsxml-json"});
        annotation(Documentation(info="<html>
<p>If strict=true and the line being read contains more than a name=value pair this function will return an error.  However, when strict=false it will return the value and ignore the rest of the line.</p>
<p>For example</p>
<code>[example_section]<br />name=13   ;this is a comment</code>
<p>In this example, if strict=true we will get an error but if strict=false it will return the value 13</p>
</html>"));
      end getInteger;

      function getBoolean "Get scalar Boolean value from INI file"
        extends Interfaces.partialGetBoolean;
        input String section="" "Section";
        input Types.ExternINIFile ini "External INI file object";
        input Boolean strict=true "Return an error if there are characters on the line that aren't part of the value";
        protected
          Real aux;
        algorithm
          (aux, exist) := getReal(ini=ini, varName=varName, section=section, strict=strict);
          y := aux <> 0;
        annotation(Inline=false);
      end getBoolean;

      function getString "Get scalar String value from INI file"
        extends Interfaces.partialGetString;
        input String section="" "Section";
        input Types.ExternINIFile ini "External INI file object";
        external "C" str=ED_getStringFromINI(ini, varName, section, exist) annotation(
          __iti_dll = "ITI_ED_INIFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_INIFile.h\"",
          Library = {"ED_INIFile", "bsxml-json"});
      end getString;
      annotation(Icon(coordinateSystem(preserveAspectRatio=false, extent={{-100,-100},{100,100}}), graphics={Text(lineColor={128,128,128},extent={{-90,-90},{90,90}},textString="f")}));
    end INI;

    package JSON "JSON file functions"
      extends Modelica.Icons.Package;
      function getReal "Get scalar Real value from JSON file"
        extends Interfaces.partialGetReal;
        input Types.ExternJSONFile json "External JSON file object";
        external "C" y=ED_getDoubleFromJSON(json, varName, exist) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getReal;

      function getRealArray1D "Get 1D Real values from JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer n=1 "Number of elements";
        input Types.ExternJSONFile json "External JSON file object";
        output Real y[n] "1D Real values";
        external "C" ED_getDoubleArray1DFromJSON(json, varName, y, size(y, 1)) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getRealArray1D;

      function getRealArray2D "Get 2D Real values from JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer m=1 "Number of rows";
        input Integer n=1 "Number of columns";
        input Types.ExternJSONFile json "External JSON file object";
        output Real y[m,n] "2D Real values";
        external "C" ED_getDoubleArray2DFromJSON(json, varName, y, size(y, 1), size(y, 2)) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getRealArray2D;

      function getInteger "Get scalar Integer value from JSON file"
        extends Interfaces.partialGetInteger;
        input Types.ExternJSONFile json "External JSON file object";
        external "C" y=ED_getIntFromJSON(json, varName, exist) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getInteger;

      function getIntegerArray1D "Get 1D Integer values from JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer n=1 "Number of elements";
        input Types.ExternJSONFile json "External JSON file object";
        output Integer y[n] "1D Real values";
        external "C" ED_getIntArray1DFromJSON(json, varName, y, size(y, 1)) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getIntegerArray1D;

      function getIntegerArray2D "Get 2D Integer values from JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer m=1 "Number of rows";
        input Integer n=1 "Number of columns";
        input Types.ExternJSONFile json "External JSON file object";
        output Integer y[m,n] "2D Real values";
        external "C" ED_getIntArray2DFromJSON(json, varName, y, size(y, 1), size(y, 2)) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getIntegerArray2D;

      function getBoolean "Get scalar Boolean value from JSON file"
        extends Interfaces.partialGetBoolean;
        input Types.ExternJSONFile json "External JSON file object";
        external "C" y = ED_getBooleanFromJSON(json, varName, exist) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getBoolean;

      function getBooleanArray1D "Get 1D Boolean values from JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer n=1 "Number of elements";
        input Types.ExternJSONFile json "External JSON file object";
        output Boolean y[n] "1D Real values";
        external "C" ED_getBooleanArray1DFromJSON(json, varName, y, size(y, 1)) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getBooleanArray1D;

      function getBooleanArray2D "Get 2D Boolean values from JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer m=1 "Number of rows";
        input Integer n=1 "Number of columns";
        input Types.ExternJSONFile json "External JSON file object";
        output Boolean y[m,n] "2D Real values";
        external "C" ED_getBooleanArray2DFromJSON(json, varName, y, size(y, 1), size(y, 2)) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getBooleanArray2D;

      function getString "Get scalar String value from JSON file"
        extends Interfaces.partialGetString;
        input Types.ExternJSONFile json "External JSON file object";
        external "C" str=ED_getStringFromJSON(json, varName, exist) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getString;

      function getStringArray1D "Get 1D String values from JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer n=1 "Number of elements";
        input Types.ExternJSONFile json "External JSON file object";
        output String y[n] "1D Real values";
        external "C" ED_getStringArray1DFromJSON(json, varName, y, size(y, 1)) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getStringArray1D;

      function getStringArray2D "Get 2D String values from JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer m=1 "Number of rows";
        input Integer n=1 "Number of columns";
        input Types.ExternJSONFile json "External JSON file object";
        output String y[m,n] "2D Real values";
        external "C" ED_getStringArray2DFromJSON(json, varName, y, size(y, 1), size(y, 2)) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getStringArray2D;

      function getArraySize1D "Get length of 1D array in JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Types.ExternJSONFile json "External JSON file object";
        output Integer n "Number of elements in array";
        external "C" ED_getArray1DDimensionFromJSON(json, varName, n) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getArraySize1D;

      function getArraySize2D "Get dimensions of 2D array in JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Types.ExternJSONFile json "External JSON file object";
        output Integer m "Number of rows in array";
        output Integer n "Number of columns in array";
        external "C" ED_getArray2DDimensionsFromJSON(json, varName, m, n) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getArraySize2D;

      function getArrayRows2D "Get first dimension of 2D array in JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Types.ExternJSONFile json "External JSON file object";
        output Integer m "Number of rows in array";
        protected
          Integer n[1] "Number of columns in array";
        external "C" ED_getArray2DDimensionsFromJSON(json, varName, m, n) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getArrayRows2D;

      function getArrayColumns2D "Get second dimension of 2D array in JSON file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Types.ExternJSONFile json "External JSON file object";
        output Integer n "Number of columns in array";
        protected
          Integer m[1] "Number of rows in array";
        external "C" ED_getArray2DDimensionsFromJSON(json, varName, m, n) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "parson"});
      end getArrayColumns2D;
      annotation(Icon(coordinateSystem(preserveAspectRatio=false, extent={{-100,-100},{100,100}}), graphics={Text(lineColor={128,128,128},extent={{-90,-90},{90,90}},textString="f")}));
    end JSON;

    package MAT "MAT-file functions"
      extends Modelica.Icons.Package;
      function getRealArray2D "Get 2D Real values from MAT-file"
        extends Modelica.Icons.Function;
        input String varName "Variable name";
        input Integer m=1 "Number of rows";
        input Integer n=1 "Number of columns";
        input Types.ExternMATFile mat "External MATLAB MAT-file object";
        output Real y[m,n] "2D Real values";
        external "C" ED_getDoubleArray2DFromMAT(mat, varName, y, size(y, 1), size(y, 2)) annotation(
          __iti_dll = "ITI_ED_MATFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_MATFile.h\"",
          Library = {"ED_MATFile", "hdf5", "zlib", "dl"});
      end getRealArray2D;

      function getStringArray1D "Get 1D String values from MAT-file"
        extends Modelica.Icons.Function;
        input String varName "Variable name";
        input Integer n=1 "Number of elements";
        input Types.ExternMATFile mat "External MATLAB MAT-file object";
        output String str[n] "1D String values";
        external "C" ED_getStringArray1DFromMAT(mat, varName, str, size(str, 1)) annotation(
          __iti_dll = "ITI_ED_MATFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_MATFile.h\"",
          Library = {"ED_MATFile", "hdf5", "zlib", "dl"});
      end getStringArray1D;

      function getArraySize2D "Get dimensions of 2D array in MAT file"
        extends Modelica.Icons.Function;
        input String varName "Variable name";
        input Types.ExternMATFile mat "External MATLAB MAT-file object";
        output Integer m "Number of rows in array";
        output Integer n "Number of columns in array";
        external "C" ED_getArray2DDimensionsFromMAT(mat, varName, m, n) annotation(
          __iti_dll = "ITI_ED_MATFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_MATFile.h\"",
          Library = {"ED_MATFile", "hdf5", "zlib", "dl"});
      end getArraySize2D;

      function getArrayRows2D "Get first dimension of 2D array in MAT file"
        extends Modelica.Icons.Function;
        input String varName "Variable name";
        input Types.ExternMATFile mat "External MATLAB MAT-file object";
        output Integer m "Number of rows in array";
        protected
          Integer n[1] "Number of columns in array";
        external "C" ED_getArray2DDimensionsFromMAT(mat, varName, m, n) annotation(
          __iti_dll = "ITI_ED_MATFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_MATFile.h\"",
          Library = {"ED_MATFile", "hdf5", "zlib", "dl"});
      end getArrayRows2D;

      function getArrayColumns2D "Get second dimension of 2D array in MAT file"
        extends Modelica.Icons.Function;
        input String varName "Variable name";
        input Types.ExternMATFile mat "External MATLAB MAT-file object";
        output Integer n "Number of columns in array";
        protected
          Integer m[1] "Number of rows in array";
        external "C" ED_getArray2DDimensionsFromMAT(mat, varName, m, n) annotation(
          __iti_dll = "ITI_ED_MATFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_MATFile.h\"",
          Library = {"ED_MATFile", "hdf5", "zlib", "dl"});
      end getArrayColumns2D;
      annotation(Icon(coordinateSystem(preserveAspectRatio=false, extent={{-100,-100},{100,100}}), graphics={Text(lineColor={128,128,128},extent={{-90,-90},{90,90}},textString="f")}));
    end MAT;

    package XLS "Excel XLS file functions"
      extends Modelica.Icons.Package;
      function getReal "Get scalar Real value from Excel XLS file"
        extends Modelica.Icons.Function;
        input String cellAddress="A1" "Cell address";
        input String sheetName="" "Sheet name";
        input Types.ExternXLSFile xls "External Excel XLS file object";
        output Real y "Real value";
        output Boolean exist "= true, if cellAddress exits; = false, if it does not exist and y is set to 0.0";
        external "C" y=ED_getDoubleFromXLS(xls, cellAddress, sheetName, exist) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSFile.h\"",
          Library = "ED_XLSFile");
      end getReal;

      function getRealArray2D "Get 2D Real values from Excel XLS file"
        extends Modelica.Icons.Function;
        input String cellAddress="A1" "Start cell address";
        input String sheetName="" "Sheet name";
        input Integer m=1 "Number of rows";
        input Integer n=1 "Number of columns";
        input Types.ExternXLSFile xls "External Excel XLS file object";
        output Real y[m,n] "2D Real values";
        external "C" ED_getDoubleArray2DFromXLS(xls, cellAddress, sheetName, y, size(y, 1), size(y, 2)) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSFile.h\"",
          Library = "ED_XLSFile");
      end getRealArray2D;

      function getInteger "Get scalar Integer value from Excel XLS file"
        extends Modelica.Icons.Function;
        input String cellAddress="A1" "Cell address";
        input String sheetName="" "Sheet name";
        input Types.ExternXLSFile xls "External Excel XLS file object";
        output Integer y "Integer value";
        output Boolean exist "= true, if cellAddress exits; = false, if it does not exist and y is set to 0";
        external "C" y=ED_getIntFromXLS(xls, cellAddress, sheetName, exist) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSFile.h\"",
          Library = "ED_XLSFile");
      end getInteger;

      function getBoolean "Get scalar Boolean value from Excel XLS file"
        extends Modelica.Icons.Function;
        input String cellAddress="A1" "Cell address";
        input String sheetName="" "Sheet name";
        input Types.ExternXLSFile xls "External Excel XLS file object";
        output Boolean y "Boolean value";
        output Boolean exist "= true, if cellAddress exits; = false, if it does not exist and y is set to false";
        protected
          Real aux;
        algorithm
          (aux, exist) := getReal(xls=xls, cellAddress=cellAddress, sheetName=sheetName);
          y := aux <> 0;
        annotation(Inline=false);
      end getBoolean;

      function getString "Get scalar String value from Excel XLS file"
        extends Modelica.Icons.Function;
        input String cellAddress="A1" "Cell address";
        input String sheetName="" "Sheet name";
        input Types.ExternXLSFile xls "External Excel XLS file object";
        output String str "String value";
        output Boolean exist "= true, if cellAddress exits; = false, if it does not exist and y is set to an empty string";
        external "C" str=ED_getStringFromXLS(xls, cellAddress, sheetName, exist) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSFile.h\"",
          Library = "ED_XLSFile");
      end getString;

      function getArraySize2D "Get dimensions of 2D array in Excel XLS file"
        extends Modelica.Icons.Function;
        input String sheetName="" "Sheet name";
        input Types.ExternXLSFile xls "External Excel XLS file object";
        output Integer m "Number of rows in array";
        output Integer n "Number of columns in array";
        external "C" ED_getArray2DDimensionsFromXLS(xls, sheetName, m, n) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSFile.h\"",
          Library = "ED_XLSFile");
      end getArraySize2D;

      function getArrayRows2D "Get first dimension of 2D array in Excel XLS file"
        extends Modelica.Icons.Function;
        input String sheetName="" "Sheet name";
        input Types.ExternXLSFile xls "External Excel XLS file object";
        output Integer m "Number of rows in array";
        protected
          Integer n[1] "Number of columns in array";
        external "C" ED_getArray2DDimensionsFromXLS(xls, sheetName, m, n) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSFile.h\"",
          Library = "ED_XLSFile");
      end getArrayRows2D;

      function getArrayColumns2D "Get second dimension of 2D array in Excel XLS file"
        extends Modelica.Icons.Function;
        input String sheetName="" "Sheet name";
        input Types.ExternXLSFile xls "External Excel XLS file object";
        output Integer n "Number of columns in array";
        protected
          Integer m[1] "Number of rows in array";
        external "C" ED_getArray2DDimensionsFromXLS(xls, sheetName, m, n) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSFile.h\"",
          Library = "ED_XLSFile");
      end getArrayColumns2D;
      annotation(Icon(coordinateSystem(preserveAspectRatio=false, extent={{-100,-100},{100,100}}), graphics={Text(lineColor={128,128,128},extent={{-90,-90},{90,90}},textString="f")}));
    end XLS;

    package XLSX "Excel XLSX file functions"
      extends Modelica.Icons.Package;
      function getReal "Get scalar Real value from Excel XLSX file"
        extends Modelica.Icons.Function;
        input String cellAddress="A1" "Cell address";
        input String sheetName="" "Sheet name";
        input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
        output Real y "Real value";
        output Boolean exist "= true, if cellAddress exits; = false, if it does not exist and y is set to 0.0";
        external "C" y=ED_getDoubleFromXLSX(xlsx, cellAddress, sheetName, exist) annotation(
          __iti_dll = "ITI_ED_XLSXFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSXFile.h\"",
          Library = {"ED_XLSXFile", "bsxml-json", "expat", "zlib"});
      end getReal;

      function getRealArray2D "Get 2D Real values from Excel XLSX file"
        extends Modelica.Icons.Function;
        input String cellAddress="A1" "Start cell address";
        input String sheetName="" "Sheet name";
        input Integer m=1 "Number of rows";
        input Integer n=1 "Number of columns";
        input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
        output Real y[m,n] "2D Real values";
        external "C" ED_getDoubleArray2DFromXLSX(xlsx, cellAddress, sheetName, y, size(y, 1), size(y, 2)) annotation(
          __iti_dll = "ITI_ED_XLSXFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSXFile.h\"",
          Library = {"ED_XLSXFile", "bsxml-json", "expat", "zlib"});
      end getRealArray2D;

      function getInteger "Get scalar Integer value from Excel XLSX file"
        extends Modelica.Icons.Function;
        input String cellAddress="A1" "Cell address";
        input String sheetName="" "Sheet name";
        input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
        output Integer y "Integer value";
        output Boolean exist "= true, if cellAddress exits; = false, if it does not exist and y is set 0";
        external "C" y=ED_getIntFromXLSX(xlsx, cellAddress, sheetName, true) annotation(
          __iti_dll = "ITI_ED_XLSXFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSXFile.h\"",
          Library = {"ED_XLSXFile", "bsxml-json", "expat", "zlib"});
      end getInteger;

      function getBoolean "Get scalar Boolean value from Excel XLSX file"
        extends Modelica.Icons.Function;
        input String cellAddress="A1" "Cell address";
        input String sheetName="" "Sheet name";
        input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
        output Boolean y "Boolean value";
        output Boolean exist "= true, if cellAddress exits; = false, if it does not exist and y is set to false";
        protected
          Real aux;
        algorithm
          (aux, exist) := getReal(xlsx=xlsx, cellAddress=cellAddress, sheetName=sheetName);
          y := aux <> 0;
        annotation(Inline=false);
      end getBoolean;

      function getString "Get scalar String value from Excel XLSX file"
        extends Modelica.Icons.Function;
        input String cellAddress="A1" "Cell address";
        input String sheetName="" "Sheet name";
        input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
        output String str "String value";
        output Boolean exist "= true, if cellAddress exits; = false, if it does not exist and y is set to an empty string";
        external "C" str=ED_getStringFromXLSX(xlsx, cellAddress, sheetName, exist) annotation(
          __iti_dll = "ITI_ED_XLSXFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSXFile.h\"",
          Library = {"ED_XLSXFile", "bsxml-json", "expat", "zlib"});
      end getString;

      function getArraySize2D "Get dimensions of 2D array in Excel XLSX file"
        extends Modelica.Icons.Function;
        input String sheetName="" "Sheet name";
        input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
        output Integer m "Number of rows in array";
        output Integer n "Number of columns in array";
        external "C" ED_getArray2DDimensionsFromXLSX(xlsx, sheetName, m, n) annotation(
          __iti_dll = "ITI_ED_XLSXFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSXFile.h\"",
          Library = {"ED_XLSXFile", "bsxml-json", "expat", "zlib"});
      end getArraySize2D;

      function getArrayRows2D "Get first dimension of 2D array in Excel XLSX file"
        extends Modelica.Icons.Function;
        input String sheetName="" "Sheet name";
        input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
        output Integer m "Number of rows in array";
        protected
          Integer n[1] "Number of columns in array";
        external "C" ED_getArray2DDimensionsFromXLSX(xlsx, sheetName, m, n) annotation(
          __iti_dll = "ITI_ED_XLSXFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSXFile.h\"",
          Library = {"ED_XLSXFile", "bsxml-json", "expat", "zlib"});
      end getArrayRows2D;

      function getArrayColumns2D "Get second dimension of 2D array in Excel XLSX file"
        extends Modelica.Icons.Function;
        input String sheetName="" "Sheet name";
        input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
        output Integer n "Number of columns in array";
        protected
          Integer m[1] "Number of rows in array";
        external "C" ED_getArray2DDimensionsFromXLSX(xlsx, sheetName, m, n) annotation(
          __iti_dll = "ITI_ED_XLSXFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSXFile.h\"",
          Library = {"ED_XLSXFile", "bsxml-json", "expat", "zlib"});
      end getArrayColumns2D;
      annotation(Icon(coordinateSystem(preserveAspectRatio=false, extent={{-100,-100},{100,100}}), graphics={Text(lineColor={128,128,128},extent={{-90,-90},{90,90}},textString="f")}));
    end XLSX;

    package XML "XML file functions"
      extends Modelica.Icons.Package;
      function getReal "Get scalar Real value from XML file"
        extends Interfaces.partialGetReal;
        input Types.ExternXMLFile xml "External XML file object";
        external "C" y=ED_getDoubleFromXML(xml, varName, exist) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end getReal;

      function getRealArray1D "Get 1D Real values from XML file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer n=1 "Number of elements";
        input Types.ExternXMLFile xml "External XML file object";
        output Real y[n] "1D Real values";
        external "C" ED_getDoubleArray1DFromXML(xml, varName, y, size(y, 1)) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end getRealArray1D;

      function getRealArray2D "Get 2D Real values from XML file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer m=1 "Number of rows";
        input Integer n=1 "Number of columns";
        input Types.ExternXMLFile xml "External XML file object";
        output Real y[m,n] "2D Real values";
        external "C" ED_getDoubleArray2DFromXML(xml, varName, y, size(y, 1), size(y, 2)) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end getRealArray2D;

      function getInteger "Get scalar Integer value from XML file"
        extends Interfaces.partialGetInteger;
        input Types.ExternXMLFile xml "External XML file object";
        external "C" y=ED_getIntFromXML(xml, varName, exist) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end getInteger;

      function getBoolean "Get scalar Boolean value from XML file"
        extends Interfaces.partialGetBoolean;
        input Types.ExternXMLFile xml "External XML file object";
        protected
          Real aux;
        algorithm
          (aux, exist) := getReal(xml=xml, varName=varName);
          y := aux <> 0;
        annotation(Inline=false);
      end getBoolean;

      function getString "Get scalar String value from XML file"
        extends Interfaces.partialGetString;
        input Types.ExternXMLFile xml "External XML file object";
        external "C" str=ED_getStringFromXML(xml, varName, exist) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end getString;

      function getArraySize1D "Get length of 1D array in XML file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Types.ExternXMLFile xml "External XML file object";
        output Integer n "Number of elements in array";
        external "C" ED_getArray1DDimensionFromXML(xml, varName, n) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end getArraySize1D;

      function getArraySize2D "Get dimensions of 2D array in XML file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Types.ExternXMLFile xml "External XML file object";
        output Integer m "Number of rows in array";
        output Integer n "Number of columns in array";
        external "C" ED_getArray2DDimensionsFromXML(xml, varName, m, n) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end getArraySize2D;

      function getArrayRows2D "Get first dimension of 2D array in XML file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Types.ExternXMLFile xml "External XML file object";
        output Integer m "Number of rows in array";
        protected
          Integer n[1] "Number of columns in array";
        external "C" ED_getArray2DDimensionsFromXML(xml, varName, m, n) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end getArrayRows2D;

      function getArrayColumns2D "Get second dimension of 2D array in XML file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Types.ExternXMLFile xml "External XML file object";
        output Integer n "Number of columns in array";
        protected
          Integer m[1] "Number of rows in array";
        external "C" ED_getArray2DDimensionsFromXML(xml, varName, m, n) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end getArrayColumns2D;
      annotation(Icon(coordinateSystem(preserveAspectRatio=false, extent={{-100,-100},{100,100}}), graphics={Text(lineColor={128,128,128},extent={{-90,-90},{90,90}},textString="f")}));
    end XML;
    annotation(Icon(coordinateSystem(preserveAspectRatio=false, extent={{-100,-100},{100,100}}), graphics={Text(lineColor={128,128,128},extent={{-90,-90},{90,90}},textString="f")}));
  end Functions;

  package Interfaces "Interfaces"
    extends Modelica.Icons.InterfacesPackage;
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
    class ExternCSVFile "External CSV file object"
      extends ExternalObject;
      function constructor "Parse CSV file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input String delimiter="," "Column delimiter character";
        input String quotation="\"" "Quotation character";
        input Boolean verboseRead=true "= true, if info message that file is loading is to be printed";
        output ExternCSVFile csv "External CSV file object";
        external "C" csv=ED_createCSV(fileName, delimiter, quotation, verboseRead) annotation(
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
        input Boolean verboseRead=true "= true, if info message that file is loading is to be printed";
        output ExternINIFile ini "External INI file object";
        external "C" ini=ED_createINI(fileName, verboseRead) annotation(
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
        input Boolean verboseRead=true "= true, if info message that file is loading is to be printed";
        output ExternJSONFile json "External JSON file object";
        external "C" json=ED_createJSON(fileName, verboseRead) annotation(
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

    class ExternMATFile "External MAT-file object"
      extends ExternalObject;
      function constructor "Only copy MAT-file name (File not yet opened)"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input Boolean verboseRead=true "= true, if info message that file is loading is to be printed";
        output ExternMATFile mat "External MATLAB MAT-file object";
        external "C" mat=ED_createMAT(fileName, verboseRead) annotation(
          __iti_dll = "ITI_ED_MATFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_MATFile.h\"",
          Library = {"ED_MATFile", "hdf5", "zlib", "dl"});
      end constructor;

      function destructor "Clean up"
        extends Modelica.Icons.Function;
        input ExternMATFile mat "External MATLAB MAT-file object";
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
        input String encoding="UTF-8" "Encoding";
        input Boolean verboseRead=true "= true, if info message that file is loading is to be printed";
        output ExternXLSFile xls "External Excel XLS file object";
        external "C" xls=ED_createXLS(fileName, encoding, verboseRead) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSFile.h\"",
          Library = "ED_XLSFile");
      end constructor;

      function destructor "Clean up"
        extends Modelica.Icons.Function;
        input ExternXLSFile xls "External Excel XLS file object";
        external "C" ED_destroyXLS(xls) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSFile.h\"",
          Library = "ED_XLSFile");
      end destructor;
    end ExternXLSFile;

    class ExternXLSXFile "External XLSX file object"
      extends ExternalObject;
      function constructor "Parse Excel XLSX file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input Boolean verboseRead=true "= true, if info message that file is loading is to be printed";
        output ExternXLSXFile xlsx "External Excel XLSX file object";
        external "C" xlsx=ED_createXLSX(fileName, verboseRead) annotation(
          __iti_dll = "ITI_ED_XLSXFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSXFile.h\"",
          Library = {"ED_XLSXFile", "bsxml-json", "expat", "zlib"});
      end constructor;

      function destructor "Clean up"
        extends Modelica.Icons.Function;
        input ExternXLSXFile xlsx "External Excel XLSX file object";
        external "C" ED_destroyXLSX(xlsx) annotation(
          __iti_dll = "ITI_ED_XLSXFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSXFile.h\"",
          Library = {"ED_XLSXFile", "bsxml-json", "expat", "zlib"});
      end destructor;
    end ExternXLSXFile;

    class ExternXMLFile "External XML file object"
      extends ExternalObject;
      function constructor "Parse XML file"
        extends Modelica.Icons.Function;
        input String fileName "File name";
        input Boolean verboseRead=true "= true, if info message that file is loading is to be printed";
        output ExternXMLFile xml "External XML file object";
        external "C" xml=ED_createXML(fileName, verboseRead) annotation(
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
  end Types;

  annotation(uses(Modelica(version="3.2.2")), version="2.3.4",
    Documentation(info="<html><p>Library <strong>ExternData</strong> is a Modelica utility library for data access of <a href=\"https://en.wikipedia.org/wiki/Comma-separated_values\">CSV</a>, <a href=\"https://en.wikipedia.org/wiki/INI_file\">INI</a>, <a href=\"https://en.wikipedia.org/wiki/JSON\">JSON</a>, <a href=\"https://en.wikipedia.org/wiki/MATLAB\">MATLAB</a> MAT, <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel\">Excel</a> <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel#Binary\">XLS</a>/<a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel#XML_Spreadsheet\">XLSX</a> and <a href=\"https://en.wikipedia.org/wiki/XML\">XML</a> files.</p></html>"));
end ExternData;
