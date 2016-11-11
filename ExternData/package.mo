// CP: 65001
/* package.mo - Modelica library for data I/O of INI, JSON, MATLAB MAT, Excel XLS/XLSX or XML files
 *
 * Copyright (C) 2015-2016, tbeu
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
package ExternData "Library for data I/O of INI, JSON, MATLAB MAT, Excel XLS/XLSX or XML files"
  extends Modelica.Icons.Package;
  package UsersGuide "User's Guide"
    extends Modelica.Icons.Information;
    class References "References"
      extends Modelica.Icons.References;
      annotation(Documentation(info="<html><p>The ExternData Modelica library is based on the following third-party C projects</p><ul><li><a href=\"https://github.com/bsapundzhiev/bsxml-json\">bsxml-json</a>&nbsp;-&nbsp;Borislav Sapundzhiev&#039;s fairly simple XML DOM and JSON implementation</li><li><a href=\"http://sourceforge.net/projects/expat\">expat</a>&nbsp;-&nbsp;James Clark&#039;s Expat XML parser library</li><li><a href=\"https://support.hdfgroup.org/HDF5\">hdf5</a>&nbsp;-&nbsp;The HDF Group&#039;s data model, library and file format for storing and managing data</li><li><a href=\"http://sourceforge.net/projects/libxls\">libxls</a>&nbsp;-&nbsp;David Hoerl&#039;s C library for parsing Excel files</li><li><a href=\"http://sourceforge.net/projects/matio\">matio</a>&nbsp;-&nbsp;Christopher Hulbert&#039;s C library for reading and writing MATLAB MAT-files</li><li><a href=\"https://github.com/compuphase/minIni\">minIni</a>&nbsp;-&nbsp;Thiadmer Riemersma&#039;s small and portable INI file library with read/write support</li><li><a href=\"http://www.winimage.com/zLibDll/minizip.html\">minizip</a>&nbsp;-&nbsp;Gilles Vollant&#039;s Zip and UnZip library</li><li><a href=\"https://github.com/troydhanson/uthash\">uthash</a>&nbsp;-&nbsp;Troy D. Hanson&#039;s C macros for hash tables and more</li><li><a href=\"https://github.com/madler/zlib\">zlib</a>&nbsp;-&nbsp;Jean-loup Gailly&#039;s and Mark Adler&#039;s massively spiffy yet delicately unobtrusive compression library</li></ul></html>"));
    end References;
    class License "Simplified BSD License"
      extends Modelica.Icons.Information;
      annotation(Documentation(info="<html><p>Copyright (C) 2015-2016, tbeu<br>All rights reserved.</p><p>Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:</p><p>1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.</p><p>2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.</p><p>THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.</p></html>"));
    end License;
    class Contact "Contact"
      extends Modelica.Icons.Contact;
      annotation(Documentation(info="<html><p>The ExternData Modelica library is developed by <a href=\"https://github.com/tbeu\">tbeu</a> at <a href=\"https://github.com/tbeu/ExternData\">GitHub</a>.</p></html>"));
    end Contact;
    annotation(DocumentationClass=true,
      Documentation(info="<html><p>Library <strong>ExternData</strong> is a <a href=\"https://en.wikipedia.org/wiki/Modelica\">Modelica</a> utility library to access data stored in <a href=\"https://en.wikipedia.org/wiki/INI_file\">INI</a>, <a href=\"https://en.wikipedia.org/wiki/JSON\">JSON</a>, <a href=\"https://en.wikipedia.org/wiki/MATLAB\">MATLAB</a> MAT, <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel\">Excel</a> <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel#Binary\">XLS</a>/<a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel#XML_Spreadsheet\">XLSX</a> and <a href=\"https://en.wikipedia.org/wiki/XML\">XML</a> files.</p></html>"));
  end UsersGuide;

  model INIFile "Read data values from INI file"
    parameter String fileName="" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="INI files (*.ini);;Configuration files (*.cfg;*.conf;config.txt);;Text files (*.txt)",
        caption="Open file")));

    final function getReal = Functions.INI.getReal(ini=ini) "Get scalar Real value from INI file";
    final function getInteger = Functions.INI.getInteger(ini=ini) "Get scalar Integer value from INI file";
    final function getBoolean = Functions.INI.getBoolean(ini=ini) "Get scalar Boolean value from INI file";
    final function getString = Functions.INI.getString(ini=ini) "Get scalar String value from INI file";

    protected
      parameter Types.ExternINIFile ini=Types.ExternINIFile(fileName);

    annotation(
      Documentation(info="<html><p>Model that wraps the external object <a href=\"modelica://ExternData.Types.ExternINIFile\">ExternINIFile</a> and the <a href=\"modelica://ExternData.Functions.INI\">INI</a> read functions for data access of <a href=\"https://en.wikipedia.org/wiki/INI_file\">INI</a> files.</p><p>See <a href=\"modelica://ExternData.Examples.INITest\">Examples.INITest</a> for an example.</p></html>"),
      defaultComponentName="inifile",
      Icon(graphics={
        Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
        Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillPattern=FillPattern.Solid),
        Text(lineColor={0,0,255},extent={{-85,-10},{85,-55}},textString="[ini]"),
        Text(lineColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end INIFile;

  model JSONFile "Read data values from JSON file"
    parameter String fileName="" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="JSON files (*.json)",
        caption="Open file")));

    final function getReal = Functions.JSON.getReal(json=json) "Get scalar Real value from JSON file";
    final function getInteger = Functions.JSON.getInteger(json=json) "Get scalar Integer value from JSON file";
    final function getBoolean = Functions.JSON.getBoolean(json=json) "Get scalar Boolean value from JSON file";
    final function getString = Functions.JSON.getString(json=json) "Get scalar String value from JSON file";

    protected
      parameter Types.ExternJSONFile json=Types.ExternJSONFile(fileName);

    annotation(
      Documentation(info="<html><p>Model that wraps the external object <a href=\"modelica://ExternData.Types.ExternJSONFile\">ExternJSONFile</a> and the <a href=\"modelica://ExternData.Functions.JSON\">JSON</a> read functions for data access of <a href=\"https://en.wikipedia.org/wiki/JSON\">JSON</a> files.</p><p>See <a href=\"modelica://ExternData.Examples.JSONTest\">Examples.JSONTest</a> for an example.</p></html>"),
      defaultComponentName="jsonfile",
      Icon(graphics={
        Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
        Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillPattern=FillPattern.Solid),
        Text(lineColor={0,0,255},extent={{-85,-10},{85,-55}},textString="{\"json\"}"),
        Text(lineColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end JSONFile;

  model MATFile "Read data values from MATLAB MAT-file"
    parameter String fileName="" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="MATLAB MAT-files (*.mat)",
        caption="Open file")));

    final function getRealArray2D = Functions.MAT.getRealArray2D(mat=mat) "Get 2D Real values from MAT-file";

    protected
      parameter Types.ExternMATFile mat=Types.ExternMATFile(fileName);

    annotation(
      Documentation(info="<html><p>Model that wraps the external object <a href=\"modelica://ExternData.Types.ExternMATFile\">ExternMATFile</a> and the <a href=\"modelica://ExternData.Functions.MAT\">MAT</a> read functions for data access of <a href=\"https://en.wikipedia.org/wiki/MATLAB\">MATLAB</a> MAT-files.</p><p>See <a href=\"modelica://ExternData.Examples.MATTest\">Examples.MATTest</a> for an example.</p></html>"),
      defaultComponentName="matfile",
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

  model XLSFile "Read data values from Excel XLS file"
    parameter String fileName="" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="Excel files (*.xls)",
        caption="Open file")));
    parameter String encoding="UTF-8" "Encoding";

    final function getReal = Functions.XLS.getReal(xls=xls) "Get scalar Real value from Excel XLS file";
    final function getInteger = Functions.XLS.getInteger(xls=xls) "Get scalar Integer value from Excel XLS file";
    final function getBoolean = Functions.XLS.getBoolean(xls=xls) "Get scalar Boolean value from Excel XLS file";
    final function getString = Functions.XLS.getString(xls=xls) "Get scalar String value from Excel XLS file";

    protected
      parameter Types.ExternXLSFile xls=Types.ExternXLSFile(fileName, encoding);

    annotation(
      Documentation(info="<html><p>Model that wraps the external object <a href=\"modelica://ExternData.Types.ExternXLSFile\">ExternXLSFile</a> and the <a href=\"modelica://ExternData.Functions.XLS\">XLS</a> read functions for data access of <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel\">Excel</a> <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel#Binary\">XLS</a> files.</p><p>See <a href=\"modelica://ExternData.Examples.XLSTest\">Examples.XLSTest</a> for an example.</p></html>"),
      defaultComponentName="xlsfile",
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

  model XLSXFile "Read data values from Excel XLSX file"
    parameter String fileName="" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="Excel files (*.xlsx)",
        caption="Open file")));

    final function getReal = Functions.XLSX.getReal(xlsx=xlsx) "Get scalar Real value from Excel XLSX file";
    final function getInteger = Functions.XLSX.getInteger(xlsx=xlsx) "Get scalar Integer value from Excel XLSX file";
    final function getBoolean = Functions.XLSX.getBoolean(xlsx=xlsx) "Get scalar Boolean value from Excel XLSX file";
    final function getString = Functions.XLSX.getString(xlsx=xlsx) "Get scalar String value from Excel XLSX file";

    protected
      parameter Types.ExternXLSXFile xlsx=Types.ExternXLSXFile(fileName);

    annotation(
      Documentation(info="<html><p>Model that wraps the external object <a href=\"modelica://ExternData.Types.ExternXLSXFile\">ExternXLSXFile</a> and the <a href=\"modelica://ExternData.Functions.XLSX\">XLSX</a> read functions for data access of <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel\">Excel</a> <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel#XML_Spreadsheet\">XLSX</a> files.</p><p>See <a href=\"modelica://ExternData.Examples.XLSXTest\">Examples.XLSXTest</a> for an example.</p></html>"),
      defaultComponentName="xlsxfile",
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

  model XMLFile "Read data values from XML file"
    parameter String fileName="" "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="XML files (*.xml)",
        caption="Open file")));

    final function getReal = Functions.XML.getReal(xml=xml) "Get scalar Real value from XML file";
    final function getRealArray1D = Functions.XML.getRealArray1D(xml=xml) "Get 1D Real values from XML file";
    final function getRealArray2D = Functions.XML.getRealArray2D(xml=xml) "Get 2D Real values from XML file";
    final function getInteger = Functions.XML.getInteger(xml=xml) "Get scalar Integer value from XML file";
    final function getBoolean = Functions.XML.getBoolean(xml=xml) "Get scalar Boolean value from XML file";
    final function getString = Functions.XML.getString(xml=xml) "Get scalar String value from XML file";

    protected
      parameter Types.ExternXMLFile xml=Types.ExternXMLFile(fileName);

    annotation(
      Documentation(info="<html><p>Model that wraps the external object <a href=\"modelica://ExternData.Types.ExternXMLFile\">ExternXMLFile</a> and the <a href=\"modelica://ExternData.Functions.XML\">XML</a> read functions for data access of <a href=\"https://en.wikipedia.org/wiki/XML\">XML</a> files.</p><p>See <a href=\"modelica://ExternData.Examples.XMLTest\">Examples.XMLTest</a> for an example.</p></html>"),
      defaultComponentName="xmlfile",
      Icon(graphics={
        Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
        Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillColor={255,128,0},fillPattern=FillPattern.Solid),
        Text(lineColor={255,128,0},extent={{-85,-10},{85,-55}},textString="<?xml?>"),
        Text(lineColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end XMLFile;

  package Examples "Examples"
    extends Modelica.Icons.ExamplesPackage;
/* TODO: Revise examples
    model XMLTest1 "XML Real read test with initial equation"
      extends Modelica.Icons.Example;
      XMLFile xmlfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xml")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-20,60},{0,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      initial equation
        gain.k = xmlfile.getReal("set1.gain.k");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-20,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the XML file <a href=\"modelica://ExternData/Resources/Examples/test.xml\">test.xml</a> and assigns its Real value in an initial equation to the gain block.</p></html>"));
    end XMLTest1;

    model XMLTest2 "XML Real read test with parameter binding"
      extends Modelica.Icons.Example;
      XMLFile xmlfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xml")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Math.Gain gain(k=xmlfile.getReal("set2.gain.k")) annotation(Placement(transformation(extent={{-20,60},{0,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-20,70}}));
      annotation(experiment(StopTime=1),
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the XML file <a href=\"modelica://ExternData/Resources/Examples/test.xml\">test.xml</a> and assigns its Real value to the gain block by a parameter binding.</p><p>This probably is non-standard Modelica but works in Dymola where external functions are generally assumed to be pure.</p></html>"));
    end XMLTest2;

    model XMLTest3 "XML Real read test with parameter binding and fixed=false"
      extends Modelica.Icons.Example;
      XMLFile xmlfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xml")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)=xmlfile.getReal("set2.gain.k")) annotation(Placement(transformation(extent={{-20,60},{0,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-20,70}}));
      annotation(experiment(StopTime=1),
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the XML file <a href=\"modelica://ExternData/Resources/Examples/test.xml\">test.xml</a> and assigns its Real value to the gain block by a parameter binding. In comparison with <a href=\"modelica://ExternData.Examples.XMLTest2\">XMLTest2</a> <code>gain.k.fixed</code> is set to <code>false</code>.</p><p>This is non-standard Modelica but works in SimulationX though.</p></html>"));
    end XMLTest3;

    model XMLTest4 "XML String read test with initial equation"
      extends Modelica.Icons.Example;
      XMLFile xmlfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xml")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-20,60},{0,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      initial equation
        gain.k = Modelica.Utilities.Strings.scanReal(xmlfile.getString("set1.gain.k"));
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-20,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the XML file <a href=\"modelica://ExternData/Resources/Examples/test.xml\">test.xml</a>, retrieves its String value and assigns the scanned Real value (using <a href=\"modelica://Modelica.Utilities.Strings.scanReal\">Modelica.Utilities.Strings.scanReal</a>) in an initial equation to the gain block.</p></html>"));
    end XMLTest4;

    model XMLTest5 "XML Integer read test with initial equation"
      extends Modelica.Icons.Example;
      XMLFile xmlfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xml")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-20,60},{0,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      initial equation
        gain.k = xmlfile.getInteger("set1.gain.k");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-20,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the XML file <a href=\"modelica://ExternData/Resources/Examples/test.xml\">test.xml</a> and assigns its Integer value in an initial equation to the gain block.</p></html>"));
    end XMLTest5;

    model XMLTest6 "XML Real 2D array read test with parameter binding for table array of CombiTimeTable"
      extends Modelica.Icons.Example;
      XMLFile xmlfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xml")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Sources.CombiTimeTable table(table=xmlfile.getRealArray2D("table1", 3, 2)) annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      annotation(experiment(StopTime=1),
        Documentation(info="<html><p>Reads the table array parameter <code>table1</code> from the XML file <a href=\"modelica://ExternData/Resources/Examples/test.xml\">test.xml</a> and assigns its Real values to the table matrix of the table block by a parameter binding.</p><p>This probably is non-standard Modelica but works in Dymola where external functions are generally assumed to be pure.</p></html>"));
    end XMLTest6;

    model XMLTest7 "XML Real 2D array read test with with initial equation and parameter binding for table array of CombiTimeTable"
      extends Modelica.Icons.Example;
      final parameter Integer m = 3 "Number of rows";
      final parameter Integer n = 2 "Number of columns";
      XMLFile xmlfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xml")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Sources.CombiTimeTable table(table(fixed=fill(false, m, n))=fill(0., m, n)) annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      initial equation
        table.table = xmlfile.getRealArray2D("table3", m, n);
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the table array parameter <code>table3</code> from the XML file <a href=\"modelica://ExternData/Resources/Examples/test.xml\">test.xml</a> and assigns its Real values to the table matrix of the table block in an initial equation.</p><p>This works in Dymola with an unavoidable translation warning: The following parameters with fixed = false also have a binding: table.table = fill(0.0, m, n)</p></html>"));
    end XMLTest7;

    model INITest1 "INI Real read test with initial equation"
      extends Modelica.Icons.Example;
      INIFile inifile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.ini")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-20,60},{0,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      initial equation
        gain.k = inifile.getReal("gain.k");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-20,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the plain section of the INI file <a href=\"modelica://ExternData/Resources/Examples/test.ini\">test.ini</a> and assigns its Real value in an initial equation to the gain block.</p></html>"));
    end INITest1;

    model INITest2 "INI Real read test from section with initial equation"
      extends Modelica.Icons.Example;
      INIFile inifile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.ini")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-20,60},{0,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      initial equation
        gain.k = inifile.getReal("gain.k", "set1");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-20,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the section [set1] of the INI file <a href=\"modelica://ExternData/Resources/Examples/test.ini\">test.ini</a> and assigns its Real value in an initial equation to the gain block.</p></html>"));
    end INITest2;

    model INITest3 "INI String read test from section with initial equation"
      extends Modelica.Icons.Example;
      INIFile inifile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.ini")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-20,60},{0,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      initial equation
        gain.k = Modelica.Utilities.Strings.scanReal(inifile.getString("gain.k", "set1"));
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-20,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the INI file <a href=\"modelica://ExternData/Resources/Examples/test.ini\">test.ini</a>, retrieves its String value and assigns the scanned Real value (using <a href=\"modelica://Modelica.Utilities.Strings.scanReal\">Modelica.Utilities.Strings.scanReal</a>) in an initial equation to the gain block.</p></html>"));
    end INITest3;

    model INITest4 "INI Integer read test from section with initial equation"
      extends Modelica.Icons.Example;
      INIFile inifile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.ini")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-20,60},{0,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      initial equation
        gain.k = inifile.getInteger("gain.k", "set1");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-20,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the INI file <a href=\"modelica://ExternData/Resources/Examples/test.ini\">test.ini</a> and assigns its Integer value in an initial equation to the gain block.</p></html>"));
    end INITest4;

    model JSONTest1 "JSON Real read test with initial equation"
      extends Modelica.Icons.Example;
      JSONFile jsonfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.json")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-20,60},{0,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      initial equation
        gain.k = jsonfile.getReal("set1.gain.k");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-20,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the JSON file <a href=\"modelica://ExternData/Resources/Examples/test.json\">test.json</a> and assigns its Real value in an initial equation to the gain block.</p></html>"));
    end JSONTest1;

    model JSONTest2 "JSON String read test with initial equation"
      extends Modelica.Icons.Example;
      JSONFile jsonfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.json")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-20,60},{0,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      initial equation
        gain.k = Modelica.Utilities.Strings.scanReal(jsonfile.getString("set1.gain.k"));
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-20,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the JSON file <a href=\"modelica://ExternData/Resources/Examples/test.json\">test.json</a>, retrieves its String value and assigns the scanned Real value (using <a href=\"modelica://Modelica.Utilities.Strings.scanReal\">Modelica.Utilities.Strings.scanReal</a>) in an initial equation to the gain block.</p></html>"));
    end JSONTest2;

    model JSONTest3 "JSON Integer read test with initial equation"
      extends Modelica.Icons.Example;
      JSONFile jsonfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.json")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-20,60},{0,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      initial equation
        gain.k = jsonfile.getInteger("set2.gain.k");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-20,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the JSON file <a href=\"modelica://ExternData/Resources/Examples/test.json\">test.json</a> and assigns its Integer value in an initial equation to the gain block.</p></html>"));
    end JSONTest3;

    model XLSTest1 "XLS Real read test from default sheet with initial equation"
      extends Modelica.Icons.Example;
      XLSFile xlsfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xls")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-20,60},{0,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      initial equation
        gain.k = xlsfile.getReal("B2");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-20,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the first sheet of the Excel file <a href=\"modelica://ExternData/Resources/Examples/test.xls\">test.xls</a> and assigns its Real value in an initial equation to the gain block.</p></html>"));
    end XLSTest1;

    model XLSTest2 "XLS Real read test from sheet with initial equation"
      extends Modelica.Icons.Example;
      XLSFile xlsfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xls")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-20,60},{0,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      initial equation
        gain.k = xlsfile.getReal("B2", "set2");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-20,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from sheet set2 of the Excel file <a href=\"modelica://ExternData/Resources/Examples/test.xls\">test.xls</a> and assigns its Real value in an initial equation to the gain block.</p></html>"));
    end XLSTest2;

    model XLSTest3 "XLS String read test from sheet with initial equation"
      extends Modelica.Icons.Example;
      XLSFile xlsfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xls")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-20,60},{0,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      initial equation
        gain.k = Modelica.Utilities.Strings.scanReal(xlsfile.getString("B2", "set2"));
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-20,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from sheet set2 of the Excel file <a href=\"modelica://ExternData/Resources/Examples/test.xls\">test.xls</a>, retrieves its String value and assigns the scanned Real value (using <a href=\"modelica://Modelica.Utilities.Strings.scanReal\">Modelica.Utilities.Strings.scanReal</a>) in an initial equation to the gain block.</p></html>"));
    end XLSTest3;

    model XLSTest4 "XLS Integer read test from sheet with initial equation"
      extends Modelica.Icons.Example;
      XLSFile xlsfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xls")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-20,60},{0,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      initial equation
        gain.k = xlsfile.getInteger("B2", "set1");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-20,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from sheet set1 of the Excel file <a href=\"modelica://ExternData/Resources/Examples/test.xls\">test.xls</a> and assigns its Integer value in an initial equation to the gain block.</p></html>"));
    end XLSTest4;

    model XLSXTest1 "XLSX Real read test from default sheet with initial equation"
      extends Modelica.Icons.Example;
      XLSXFile xlsxfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xlsx")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-20,60},{0,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      initial equation
        gain.k = xlsxfile.getReal("B2");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-20,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the first sheet of the Excel file <a href=\"modelica://ExternData/Resources/Examples/test.xlsx\">test.xlsx</a> and assigns its Real value in an initial equation to the gain block.</p></html>"));
    end XLSXTest1;

    model XLSXTest2 "XLSX Real read test from sheet with initial equation"
      extends Modelica.Icons.Example;
      XLSXFile xlsxfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xlsx")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-20,60},{0,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      initial equation
        gain.k = xlsxfile.getReal("B2", "set2");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-20,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from sheet set2 of the Excel file <a href=\"modelica://ExternData/Resources/Examples/test.xlsx\">test.xlsx</a> and assigns its Real value in an initial equation to the gain block.</p></html>"));
    end XLSXTest2;

    model XLSXTest3 "XLSX String read test from sheet with initial equation"
      extends Modelica.Icons.Example;
      XLSXFile xlsxfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xlsx")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-20,60},{0,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      initial equation
        gain.k = Modelica.Utilities.Strings.scanReal(xlsxfile.getString("B2", "set2"));
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-20,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from sheet set2 of the Excel file <a href=\"modelica://ExternData/Resources/Examples/test.xlsx\">test.xlsx</a>, retrieves its String value and assigns the scanned Real value (using <a href=\"modelica://Modelica.Utilities.Strings.scanReal\">Modelica.Utilities.Strings.scanReal</a>) in an initial equation to the gain block.</p></html>"));
    end XLSXTest3;

    model XLSXTest4 "XLSX Integer read test from sheet with initial equation"
      extends Modelica.Icons.Example;
      XLSXFile xlsxfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xlsx")) annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-20,60},{0,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
      initial equation
        gain.k = xlsxfile.getInteger("B2", "set1");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-20,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from sheet set1 of the Excel file <a href=\"modelica://ExternData/Resources/Examples/test.xlsx\">test.xlsx</a> and assigns its Integer value in an initial equation to the gain block.</p></html>"));
    end XLSXTest4;
*/
  end Examples;

  package Functions "Functions"
    extends Modelica.Icons.Package;

    package INI "INI file functions"
      extends Modelica.Icons.Package;
      function getReal "Get scalar Real value from INI file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input String section="" "Section";
        input Types.ExternINIFile ini "External INI file object";
        output Real y "Real value";
        algorithm
          y := Internal.getReal(ini=ini, varName=varName, section=section);
        annotation(Inline=true);
      end getReal;

      function getInteger "Get scalar Integer value from INI file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input String section="" "Section";
        input Types.ExternINIFile ini "External INI file object";
        output Integer y "Integer value";
        algorithm
          y := Internal.getInteger(ini=ini, varName=varName, section=section);
        annotation(Inline=true);
      end getInteger;

      function getBoolean "Get scalar Boolean value from INI file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input String section="" "Section";
        input Types.ExternINIFile ini "External INI file object";
        output Boolean y "Boolean value";
        algorithm
          y := Internal.getReal(ini=ini, varName=varName, section=section) <> 0;
        annotation(Inline=true);
      end getBoolean;

      function getString "Get scalar String value from INI file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input String section="" "Section";
        input Types.ExternINIFile ini "External INI file object";
        output String str "String value";
        algorithm
          str := Internal.getString(ini=ini, varName=varName, section=section);
        annotation(Inline=true);
      end getString;

      package Internal "Internal INI file functions"
        extends Modelica.Icons.InternalPackage;
        function getReal
          extends Interfaces.partialGetReal;
          input Types.ExternINIFile ini "External INI file object";
          input String section="" "Section";
          external "C" y=ED_getDoubleFromINI(ini, varName, section) annotation(
            __iti_dll = "ITI_ED_INIFile.dll",
            __iti_dllNoExport = true,
            Include = "#include \"ED_INIFile.h\"",
            Library = {"ED_INIFile", "bsxml-json"});
        end getReal;

        function getInteger
          extends Interfaces.partialGetInteger;
          input Types.ExternINIFile ini "External INI file object";
          input String section="" "Section";
          external "C" y=ED_getIntFromINI(ini, varName, section) annotation(
            __iti_dll = "ITI_ED_INIFile.dll",
            __iti_dllNoExport = true,
            Include = "#include \"ED_INIFile.h\"",
            Library = {"ED_INIFile", "bsxml-json"});
        end getInteger;

        function getString
          extends Interfaces.partialGetString;
          input Types.ExternINIFile ini "External INI file object";
          input String section="" "Section";
          external "C" str=ED_getStringFromINI(ini, varName, section) annotation(
            __iti_dll = "ITI_ED_INIFile.dll",
            __iti_dllNoExport = true,
            Include = "#include \"ED_INIFile.h\"",
            Library = {"ED_INIFile", "bsxml-json"});
        end getString;
      end Internal;
    end INI;

    package JSON "JSON file functions"
      extends Modelica.Icons.Package;
      function getReal "Get scalar Real value from JSON file"
        extends Interfaces.partialGetReal;
        input Types.ExternJSONFile json "External JSON file object";
        algorithm
          y := Internal.getReal(json=json, varName=varName);
        annotation(Inline=true);
      end getReal;

      function getInteger "Get scalar Integer value from JSON file"
        extends Interfaces.partialGetInteger;
        input Types.ExternJSONFile json "External JSON file object";
        algorithm
          y := Internal.getInteger(json=json, varName=varName);
        annotation(Inline=true);
      end getInteger;

      function getBoolean "Get scalar Boolean value from JSON file"
        extends Interfaces.partialGetBoolean;
        input Types.ExternJSONFile json "External JSON file object";
        algorithm
          y := Internal.getReal(json=json, varName=varName) <> 0;
        annotation(Inline=true);
      end getBoolean;

      function getString "Get scalar String value from JSON file"
        extends Interfaces.partialGetString;
        input Types.ExternJSONFile json "External JSON file object";
        algorithm
          str := Internal.getString(json=json, varName=varName);
        annotation(Inline=true);
      end getString;

      package Internal "Internal JSON file functions"
        extends Modelica.Icons.InternalPackage;
        function getReal
          extends Interfaces.partialGetReal;
          input Types.ExternJSONFile json "External JSON file object";
          external "C" y=ED_getDoubleFromJSON(json, varName) annotation(
            __iti_dll = "ITI_ED_JSONFile.dll",
            __iti_dllNoExport = true,
            Include = "#include \"ED_JSONFile.h\"",
            Library = {"ED_JSONFile", "bsxml-json"});
        end getReal;

        function getInteger
          extends Interfaces.partialGetInteger;
          input Types.ExternJSONFile json "External JSON file object";
          external "C" y=ED_getIntFromJSON(json, varName) annotation(
            __iti_dll = "ITI_ED_JSONFile.dll",
            __iti_dllNoExport = true,
            Include = "#include \"ED_JSONFile.h\"",
            Library = {"ED_JSONFile", "bsxml-json"});
        end getInteger;

        function getString
          extends Interfaces.partialGetString;
          input Types.ExternJSONFile json "External JSON file object";
          external "C" str=ED_getStringFromJSON(json, varName) annotation(
            __iti_dll = "ITI_ED_JSONFile.dll",
            __iti_dllNoExport = true,
            Include = "#include \"ED_JSONFile.h\"",
            Library = {"ED_JSONFile", "bsxml-json"});
        end getString;
      end Internal;
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
        algorithm
          y := Internal.getRealArray2D(mat=mat, varName=varName, m=m, n=n);
        annotation(Inline=true);
      end getRealArray2D;

      package Internal "Internal MAT-file functions"
        extends Modelica.Icons.InternalPackage;
        function getRealArray2D
          input String varName "Variable name";
          input Integer m=1 "Number of rows";
          input Integer n=1 "Number of columns";
          input Types.ExternMATFile mat "External MATLAB MAT-file object";
          output Real y[m,n] "2D Real values";
          external "C" ED_getDoubleArray2DFromMAT(mat, varName, y, size(y, 1), size(y, 2)) annotation(
            __iti_dll = "ITI_ED_MATFile.dll",
            __iti_dllNoExport = true,
            Include = "#include \"ED_MATFile.h\"",
            Library = {"ED_MATFile", "hdf5", "zlib"});
        end getRealArray2D;
      end Internal;
    end MAT;

    package XLS "Excel XLS file functions"
      extends Modelica.Icons.Package;
      function getReal "Get scalar Real value from Excel XLS file"
        extends Modelica.Icons.Function;
        input String cellAddress="A1" "Cell address";
        input String sheetName="" "Sheet name";
        input Types.ExternXLSFile xls "External Excel XLS file object";
        output Real y "Real value";
        algorithm
          y := Internal.getReal(xls=xls, cellAddress=cellAddress, sheetName=sheetName);
        annotation(Inline=true);
      end getReal;

      function getInteger "Get scalar Integer value from Excel XLS file"
        extends Modelica.Icons.Function;
        input String cellAddress="A1" "Cell address";
        input String sheetName="" "Sheet name";
        input Types.ExternXLSFile xls "External Excel XLS file object";
        output Integer y "Integer value";
        algorithm
          y := Internal.getInteger(xls=xls, cellAddress=cellAddress, sheetName=sheetName);
        annotation(Inline=true);
      end getInteger;

      function getBoolean "Get scalar Boolean value from Excel XLS file"
        extends Modelica.Icons.Function;
        input String cellAddress="A1" "Cell address";
        input String sheetName="" "Sheet name";
        input Types.ExternXLSFile xls "External Excel XLS file object";
        output Boolean y "Boolean value";
        algorithm
          y := Internal.getReal(xls=xls, cellAddress=cellAddress, sheetName=sheetName) <> 0;
        annotation(Inline=true);
      end getBoolean;

      function getString "Get scalar String value from Excel XLS file"
        extends Modelica.Icons.Function;
        input String cellAddress="A1" "Cell address";
        input String sheetName="" "Sheet name";
        input Types.ExternXLSFile xls "External Excel XLS file object";
        output String str "String value";
        algorithm
          str := Internal.getString(xls=xls, cellAddress=cellAddress, sheetName=sheetName);
        annotation(Inline=true);
      end getString;

      package Internal "Internal Excel XLS file functions"
        extends Modelica.Icons.InternalPackage;
        function getReal
          extends Modelica.Icons.Function;
          input String cellAddress="A1" "Cell address";
          input Types.ExternXLSFile xls "External Excel XLS file object";
          input String sheetName="" "Sheet name";
          output Real y "Real value";
          external "C" y=ED_getDoubleFromXLS(xls, cellAddress, sheetName) annotation(
            __iti_dll = "ITI_ED_XLSFile.dll",
            __iti_dllNoExport = true,
            Include = "#include \"ED_XLSFile.h\"",
            Library = "ED_XLSFile");
        end getReal;

        function getInteger
          extends Modelica.Icons.Function;
          input String cellAddress="A1" "Cell address";
          input Types.ExternXLSFile xls "External Excel XLS file object";
          input String sheetName="" "Sheet name";
          output Integer y "Integer value";
          external "C" y=ED_getIntFromXLS(xls, cellAddress, sheetName) annotation(
            __iti_dll = "ITI_ED_XLSFile.dll",
            __iti_dllNoExport = true,
            Include = "#include \"ED_XLSFile.h\"",
            Library = "ED_XLSFile");
        end getInteger;

        function getString
          extends Modelica.Icons.Function;
          input String cellAddress="A1" "Cell address";
          input Types.ExternXLSFile xls "External Excel XLS file object";
          input String sheetName="" "Sheet name";
          output String str "String value";
          external "C" str=ED_getStringFromXLS(xls, cellAddress, sheetName) annotation(
            __iti_dll = "ITI_ED_XLSFile.dll",
            __iti_dllNoExport = true,
            Include = "#include \"ED_XLSFile.h\"",
            Library = "ED_XLSFile");
        end getString;
      end Internal;
    end XLS;

    package XLSX "Excel XLSX file functions"
      extends Modelica.Icons.Package;
      function getReal "Get scalar Real value from Excel XLSX file"
        extends Modelica.Icons.Function;
        input String cellAddress="A1" "Cell address";
        input String sheetName="" "Sheet name";
        input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
        output Real y "Real value";
        algorithm
          y := Internal.getReal(xlsx=xlsx, cellAddress=cellAddress, sheetName=sheetName);
        annotation(Inline=true);
      end getReal;

      function getInteger "Get scalar Integer value from Excel XLSX file"
        extends Modelica.Icons.Function;
        input String cellAddress="A1" "Cell address";
        input String sheetName="" "Sheet name";
        input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
        output Integer y "Integer value";
        algorithm
          y := Internal.getInteger(xlsx=xlsx, cellAddress=cellAddress, sheetName=sheetName);
        annotation(Inline=true);
      end getInteger;

      function getBoolean "Get scalar Boolean value from Excel XLSX file"
        extends Modelica.Icons.Function;
        input String cellAddress="A1" "Cell address";
        input String sheetName="" "Sheet name";
        input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
        output Boolean y "Boolean value";
        algorithm
          y := Internal.getReal(xlsx=xlsx, cellAddress=cellAddress, sheetName=sheetName) <> 0;
        annotation(Inline=true);
      end getBoolean;

      function getString "Get scalar String value from Excel XLSX file"
        extends Modelica.Icons.Function;
        input String cellAddress="A1" "Cell address";
        input String sheetName="" "Sheet name";
        input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
        output String str "String value";
        algorithm
          str := Internal.getString(xlsx=xlsx, cellAddress=cellAddress, sheetName=sheetName);
        annotation(Inline=true);
      end getString;

      package Internal "Internal Excel XLSX file functions"
        extends Modelica.Icons.InternalPackage;
        function getReal
          extends Modelica.Icons.Function;
          input String cellAddress="A1" "Cell address";
          input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
          input String sheetName="" "Sheet name";
          output Real y "Real value";
          external "C" y=ED_getDoubleFromXLSX(xlsx, cellAddress, sheetName) annotation(
            __iti_dll = "ITI_ED_XLSXFile.dll",
            __iti_dllNoExport = true,
            Include = "#include \"ED_XLSXFile.h\"",
            Library = {"ED_XLSXFile", "bsxml-json", "expat", "zlib"});
        end getReal;

        function getInteger
          extends Modelica.Icons.Function;
          input String cellAddress="A1" "Cell address";
          input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
          input String sheetName="" "Sheet name";
          output Integer y "Integer value";
          external "C" y=ED_getIntFromXLSX(xlsx, cellAddress, sheetName) annotation(
            __iti_dll = "ITI_ED_XLSXFile.dll",
            __iti_dllNoExport = true,
            Include = "#include \"ED_XLSXFile.h\"",
            Library = {"ED_XLSXFile", "bsxml-json", "expat", "zlib"});
        end getInteger;

        function getString
          extends Modelica.Icons.Function;
          input String cellAddress="A1" "Cell address";
          input Types.ExternXLSXFile xlsx "External Excel XLSX file object";
          input String sheetName="" "Sheet name";
          output String str "String value";
          external "C" str=ED_getStringFromXLSX(xlsx, cellAddress, sheetName) annotation(
            __iti_dll = "ITI_ED_XLSXFile.dll",
            __iti_dllNoExport = true,
            Include = "#include \"ED_XLSXFile.h\"",
            Library = {"ED_XLSXFile", "bsxml-json", "expat", "zlib"});
        end getString;
      end Internal;
    end XLSX;

    package XML "XML file functions"
      extends Modelica.Icons.Package;
      function getReal "Get scalar Real value from XML file"
        extends Interfaces.partialGetReal;
        input Types.ExternXMLFile xml "External XML file object";
        algorithm
          y := Internal.getReal(xml=xml, varName=varName);
        annotation(Inline=true);
      end getReal;

      function getRealArray1D "Get 1D Real values from XML file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer n=1 "Number of values";
        input Types.ExternXMLFile xml "External XML file object";
        output Real y[n] "1D Real values";
        algorithm
          y := Internal.getRealArray1D(xml=xml, varName=varName, n=n);
        annotation(Inline=true);
      end getRealArray1D;

      function getRealArray2D "Get 2D Real values from XML file"
        extends Modelica.Icons.Function;
        input String varName "Key";
        input Integer m=1 "Number of rows";
        input Integer n=1 "Number of columns";
        input Types.ExternXMLFile xml "External XML file object";
        output Real y[m,n] "2D Real values";
        algorithm
          y := Internal.getRealArray2D(xml=xml, varName=varName, m=m, n=n);
        annotation(Inline=true);
      end getRealArray2D;

      function getInteger "Get scalar Integer value from XML file"
        extends Interfaces.partialGetInteger;
        input Types.ExternXMLFile xml "External XML file object";
        algorithm
          y := Internal.getInteger(xml=xml, varName=varName);
        annotation(Inline=true);
      end getInteger;

      function getBoolean "Get scalar Boolean value from XML file"
        extends Interfaces.partialGetBoolean;
        input Types.ExternXMLFile xml "External XML file object";
        algorithm
          y := Internal.getReal(xml=xml, varName=varName) <> 0;
        annotation(Inline=true);
      end getBoolean;

      function getString "Get scalar String value from XML file"
        extends Interfaces.partialGetString;
        input Types.ExternXMLFile xml "External XML file object";
        algorithm
          str := Internal.getString(xml=xml, varName=varName);
        annotation(Inline=true);
      end getString;

      package Internal "Internal XML file functions"
        extends Modelica.Icons.InternalPackage;
        function getReal
          extends Interfaces.partialGetReal;
          input Types.ExternXMLFile xml "External XML file object";
          external "C" y=ED_getDoubleFromXML(xml, varName) annotation(
            __iti_dll = "ITI_ED_XMLFile.dll",
            __iti_dllNoExport = true,
            Include = "#include \"ED_XMLFile.h\"",
            Library = {"ED_XMLFile", "bsxml-json", "expat"});
        end getReal;

        function getRealArray2D
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

        function getRealArray1D
          input String varName "Key";
          input Integer n=1 "Number of values";
          input Types.ExternXMLFile xml "External XML file object";
          output Real y[n] "1D Real values";
          external "C" ED_getDoubleArray1DFromXML(xml, varName, y, size(y, 1)) annotation(
            __iti_dll = "ITI_ED_XMLFile.dll",
            __iti_dllNoExport = true,
            Include = "#include \"ED_XMLFile.h\"",
            Library = {"ED_XMLFile", "bsxml-json", "expat"});
        end getRealArray1D;

        function getInteger
          extends Interfaces.partialGetInteger;
          input Types.ExternXMLFile xml "External XML file object";
          external "C" y=ED_getIntFromXML(xml, varName) annotation(
            __iti_dll = "ITI_ED_XMLFile.dll",
            __iti_dllNoExport = true,
            Include = "#include \"ED_XMLFile.h\"",
            Library = {"ED_XMLFile", "bsxml-json", "expat"});
        end getInteger;

        function getString
          extends Interfaces.partialGetString;
          input Types.ExternXMLFile xml "External XML file object";
          external "C" str=ED_getStringFromXML(xml, varName) annotation(
            __iti_dll = "ITI_ED_XMLFile.dll",
            __iti_dllNoExport = true,
            Include = "#include \"ED_XMLFile.h\"",
            Library = {"ED_XMLFile", "bsxml-json", "expat"});
        end getString;
      end Internal;
    end XML;
  end Functions;

  package Interfaces "Interfaces"
    extends Modelica.Icons.InterfacesPackage;
    partial function partialGetReal
      extends Modelica.Icons.Function;
      input String varName "Key";
      output Real y "Real value";
    end partialGetReal;

    partial function partialGetInteger
      extends Modelica.Icons.Function;
      input String varName "Key";
      output Integer y "Integer value";
    end partialGetInteger;

    partial function partialGetBoolean
      extends Modelica.Icons.Function;
      input String varName "Key";
      output Boolean y "Boolean value";
    end partialGetBoolean;

    partial function partialGetString
      extends Modelica.Icons.Function;
      input String varName "Key";
      output String str "String value";
    end partialGetString;
  end Interfaces;

  package Types "Types"
    extends Modelica.Icons.TypesPackage;
    class ExternINIFile "External INI file object"
      extends ExternalObject;
      function constructor "Parse INI file"
        input String fileName "File name";
        output ExternINIFile ini "External INI file object";
        external "C" ini=ED_createINI(fileName) annotation(
          __iti_dll = "ITI_ED_INIFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_INIFile.h\"",
          Library = {"ED_INIFile", "bsxml-json"});
      end constructor;

      function destructor "Clean up"
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
        input String fileName "File name";
        output ExternJSONFile json "External JSON file object";
        external "C" json=ED_createJSON(fileName) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "bsxml-json"});
      end constructor;

      function destructor "Clean up"
        input ExternJSONFile json "External JSON file object";
        external "C" ED_destroyJSON(json) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_JSONFile.h\"",
          Library = {"ED_JSONFile", "bsxml-json"});
      end destructor;
    end ExternJSONFile;

    class ExternMATFile "External MAT-file object"
      extends ExternalObject;
      function constructor "Only copy MAT-file name (File not yet opened)"
        input String fileName "File name";
        output ExternMATFile mat "External MATLAB MAT-file object";
        external "C" mat=ED_createMAT(fileName) annotation(
          __iti_dll = "ITI_ED_MATFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_MATFile.h\"",
          Library = {"ED_MATFile", "hdf5", "zlib"});
      end constructor;

      function destructor "Clean up"
        input ExternMATFile mat "External MATLAB MAT-file object";
        external "C" ED_destroyMAT(mat) annotation(
          __iti_dll = "ITI_ED_MATFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_MATFile.h\"",
          Library = {"ED_MATFile", "hdf5", "zlib"});
      end destructor;
    end ExternMATFile;

    class ExternXLSFile "External XLS file object"
      extends ExternalObject;
      function constructor "Open Excel XLS file"
        input String fileName "File name";
        input String encoding="UTF-8" "Encoding";
        output ExternXLSFile xls "External Excel XLS file object";
        external "C" xls=ED_createXLS(fileName, encoding) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSFile.h\"",
          Library = "ED_XLSFile");
      end constructor;

      function destructor "Clean up"
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
        input String fileName "File name";
        output ExternXLSXFile xlsx "External Excel XLSX file object";
        external "C" xlsx=ED_createXLSX(fileName) annotation(
          __iti_dll = "ITI_ED_XLSXFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XLSXFile.h\"",
          Library = {"ED_XLSXFile", "bsxml-json", "expat", "zlib"});
      end constructor;

      function destructor "Clean up"
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
        input String fileName "File name";
        output ExternXMLFile xml "External XML file object";
        external "C" xml=ED_createXML(fileName) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end constructor;

      function destructor "Clean up"
        input ExternXMLFile xml "External XML file object";
        external "C" ED_destroyXML(xml) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          __iti_dllNoExport = true,
          Include = "#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "bsxml-json", "expat"});
      end destructor;
    end ExternXMLFile;
  end Types;

  annotation(uses(Modelica(version="3.2.1")), version="2.0.0-dev",
    Documentation(info="<html><p>Library <strong>ExternData</strong> is a Modelica utility library for data access of <a href=\"https://en.wikipedia.org/wiki/INI_file\">INI</a>, <a href=\"https://en.wikipedia.org/wiki/JSON\">JSON</a>, <a href=\"https://en.wikipedia.org/wiki/MATLAB\">MATLAB</a> MAT, <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel\">Excel</a> <a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel#Binary\">XLS</a>/<a href=\"https://en.wikipedia.org/wiki/Microsoft_Excel#XML_Spreadsheet\">XLSX</a> and <a href=\"https://en.wikipedia.org/wiki/XML\">XML</a> files.</p></html>"));
end ExternData;
