// CP: 65001
/* package.mo - Modelica Examples library for data I/O of CSV, INI, JSON, MATLAB MAT, TIR, Excel XLS/XLSX or XML files
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

within ExternData;
package Examples "Test examples"
  extends Modelica.Icons.ExamplesPackage;
  model CSVTest "CSV file read test"
    extends Modelica.Icons.Example;
    inner parameter CSVFile csvfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.csv")) "CSV file" annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
    final parameter Integer m = csvfile.getArrayRows2D() "Number of rows in 2D array";
    Modelica.Blocks.Sources.TimeTable timeTable(table=csvfile.getRealArray2D(3, 2)) annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
    annotation(experiment(StopTime=1),
      Documentation(info="<html><p>This example model reads the table parameter from the CSV file <a href=\"modelica://ExternData/Resources/Examples/test.csv\">test.csv</a>. The table parameter is read as Real array of dimension 3x2 by function <a href=\"modelica://ExternData.CSVFile.getRealArray2D\">ExternData.CSVFile.getRealArray2D</a>. The read parameter is assigned by a parameter binding to the appropriate model parameter.</p></html>"));
  end CSVTest;

  model INITest "INI file read test"
    extends Modelica.Icons.Example;
    parameter String setName="set1" "Parameter set name" annotation(choices(choice="set1" "First parameter set", choice="set2" "Second parameter set"));
    inner parameter INIFile inifile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.ini")) "INI file" annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
    Modelica.Blocks.Math.Gain gain1(k=inifile.getReal("gain.k", setName)) annotation(Placement(transformation(extent={{-15,60},{5,80}})));
    Modelica.Blocks.Math.Gain gain2(k=Modelica.Utilities.Strings.scanReal(inifile.getString("gain.k", setName))) annotation(Placement(transformation(extent={{-15,30},{5,50}})));
    Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
    equation
      connect(clock.y,gain1.u) annotation(Line(points={{-29,70},{-17,70}}, color={0,0,127}));
      connect(clock.y,gain2.u) annotation(Line(points={{-29,70},{-22,70},{-22,40},{-17,40}}, color={0,0,127}));
    annotation(experiment(StopTime=1),
      Documentation(info="<html><p>This example model reads the gain parameters from different sections of the INI file <a href=\"modelica://ExternData/Resources/Examples/test.ini\">test.ini</a>. For gain1 the gain parameter is read as Real value using the function <a href=\"modelica://ExternData.INIFile.getReal\">ExternData.INIFile.getReal</a>. For gain2 the String value is retrieved by function <a href=\"modelica://ExternData.INIFile.getString\">ExternData.INIFile.getString</a> and converted to a Real value (using the utility function <a href=\"modelica://Modelica.Utilities.Strings.scanReal\">Modelica.Utilities.Strings.scanReal</a>). The read parameters are assigned by parameter bindings to the appropriate model parameters.</p></html>"));
  end INITest;

  model JSONTest "JSON file read test"
    extends Modelica.Icons.Example;
    parameter String setName="set1" "Parameter set name" annotation(choices(choice="set1" "First parameter set", choice="set2" "Second parameter set"));
    inner parameter JSONFile jsonfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.json")) "JSON file" annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
    Modelica.Blocks.Math.Gain gain1(k=jsonfile.getReal(setName + ".gain.k")) annotation(Placement(transformation(extent={{-15,60},{5,80}})));
    Modelica.Blocks.Math.Gain gain2(k=Modelica.Utilities.Strings.scanReal(jsonfile.getString(setName + ".gain.k"))) annotation(Placement(transformation(extent={{-15,30},{5,50}})));
    Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
    final parameter Integer m = jsonfile.getArrayRows2D("table1") "Number of rows in 2D array";
    Modelica.Blocks.Sources.TimeTable timeTable(table=jsonfile.getRealArray2D("table1", 3, 2)) annotation(Placement(transformation(extent={{-50,30},{-30,50}})));
    equation
      connect(clock.y,gain1.u) annotation(Line(points={{-29,70},{-17,70}}, color={0,0,127}));
      connect(clock.y,gain2.u) annotation(Line(points={{-29,70},{-22,70},{-22,40},{-17,40}}, color={0,0,127}));
    annotation(experiment(StopTime=1),
      Documentation(info="<html><p>This example model reads the gain parameters from different nodes of the JSON file <a href=\"modelica://ExternData/Resources/Examples/test.json\">test.json</a>. For gain1 the gain parameter is read as Real value using the function <a href=\"modelica://ExternData.JSONFile.getReal\">ExternData.JSONFile.getReal</a>. For gain2 the String value is retrieved by function <a href=\"modelica://ExternData.JSONFile.getString\">ExternData.JSONFile.getString</a> and converted to a Real value (using the utility function <a href=\"modelica://Modelica.Utilities.Strings.scanReal\">Modelica.Utilities.Strings.scanReal</a>). For timeTable the table parameter is read as Real array of dimension 3x2 by function <a href=\"modelica://ExternData.JSONFile.getRealArray2D\">ExternData.JSONFile.getRealArray2D</a>. The read parameters are assigned by parameter bindings to the appropriate model parameters.</p></html>"));
  end JSONTest;

  model MATTest "MAT-file read test"
    extends Modelica.Icons.Example;
    inner parameter MATFile matfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test_v7.3.mat")) "MAT file" annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
    final parameter Integer m = matfile.getArrayRows2D("table1") "Number of rows in 2D array";
    Modelica.Blocks.Sources.TimeTable timeTable(table=matfile.getRealArray2D("table1", 3, 2)) annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
    annotation(experiment(StopTime=1),
      Documentation(info="<html><p>This example model reads the table parameter from variable table1 of the HDF5-based MAT-file <a href=\"modelica://ExternData/Resources/Examples/test_v7.3.mat\">test_v7.3.mat</a>. The table parameter is read as Real array of dimension 3x2 by function <a href=\"modelica://ExternData.MATFile.getRealArray2D\">ExternData.MATFile.getRealArray2D</a>. The read parameter is assigned by a parameter binding to the appropriate model parameter.</p></html>"));
  end MATTest;

  model XLSTest "Excel XLS file read test"
    extends Modelica.Icons.Example;
    parameter String sheetName="set1" "Excel sheet name" annotation(choices(choice="set1" "First Excel sheet", choice="set2" "Second Excel sheet"));
    inner parameter XLSFile xlsfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xls")) "XLS file" annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
    Modelica.Blocks.Math.Gain gain1(k=xlsfile.getReal("B2", sheetName)) annotation(Placement(transformation(extent={{-15,60},{5,80}})));
    Modelica.Blocks.Math.Gain gain2(k=Modelica.Utilities.Strings.scanReal(xlsfile.getString("B2", sheetName))) annotation(Placement(transformation(extent={{-15,30},{5,50}})));
    Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
    final parameter Integer m = xlsfile.getArrayRows2D("table1") "Number of rows in 2D array";
    Modelica.Blocks.Sources.TimeTable timeTable(table=xlsfile.getRealArray2D("A1", "table1", 3, 2)) annotation(Placement(transformation(extent={{-50,30},{-30,50}})));
    final parameter Real sumB = computeColSum(xlsfile, "B") "Sum of column B";
    function computeColSum "Compute column sum"
      extends Modelica.Icons.Function;
      input XLSFile xlsfile "Excel XLS file record";
      input String col = "A" "Column";
      input String sheetName="" "Sheet name";
      input Integer startRow = 2 "Start row";
      input Integer endRow = Functions.XLS.getArrayRows2D(sheetName=sheetName, xls=xlsfile.xls) "End row";
      output Real colSum "Column sum";
      protected
        Real val;
        Boolean exist;
      algorithm
        colSum := 0.0;
        for row in startRow:endRow loop
          (val, exist) := Functions.XLS.getReal(cellAddress=col + String(row), sheetName=sheetName, xls=xlsfile.xls);
          if exist then
            colSum := colSum + val;
          end if;
        end for;
      annotation(Documentation(info="<html><p>This function computes the column sum of a given column and sheet of an Excel XLS file.</p></html>"));
    end computeColSum;
    equation
      connect(clock.y,gain1.u) annotation(Line(points={{-29,70},{-17,70}}, color={0,0,127}));
      connect(clock.y,gain2.u) annotation(Line(points={{-29,70},{-22,70},{-22,40},{-17,40}}, color={0,0,127}));
    annotation(experiment(StopTime=1),
      Documentation(info="<html><p>This example model reads the gain parameters from different cells and sheets of the Excel XLS file <a href=\"modelica://ExternData/Resources/Examples/test.xls\">test.xls</a>. For gain1 the gain parameter is read as Real value using the function <a href=\"modelica://ExternData.XLSFile.getReal\">ExternData.XLSFile.getReal</a>. For gain2 the String value is retrieved by function <a href=\"modelica://ExternData.XLSFile.getString\">ExternData.XLSFile.getString</a> and converted to a Real value (using the utility function <a href=\"modelica://Modelica.Utilities.Strings.scanReal\">Modelica.Utilities.Strings.scanReal</a>). For timeTable the table parameter is read as Real array of dimension 3x2 by function <a href=\"modelica://ExternData.XLSFile.getRealArray2D\">ExternData.XLSFile.getRealArray2D</a>. The read parameters are assigned by parameter bindings to the appropriate model parameters.</p></html>"));
  end XLSTest;

  model XLSXTest "Excel XLSX file read test"
    extends Modelica.Icons.Example;
    parameter String sheetName="set1" "Excel sheet name" annotation(choices(choice="set1" "First Excel sheet", choice="set2" "Second Excel sheet"));
    inner parameter XLSXFile xlsxfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xlsx")) "XLSX file" annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
    Modelica.Blocks.Math.Gain gain1(k=xlsxfile.getReal("B2", sheetName)) annotation(Placement(transformation(extent={{-15,60},{5,80}})));
    Modelica.Blocks.Math.Gain gain2(k=Modelica.Utilities.Strings.scanReal(xlsxfile.getString("B2", sheetName))) annotation(Placement(transformation(extent={{-15,30},{5,50}})));
    Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
    final parameter Integer m = xlsxfile.getArrayRows2D("table1") "Number of rows in 2D array";
    Modelica.Blocks.Sources.TimeTable timeTable(table=xlsxfile.getRealArray2D("A1", "table1", 3, 2)) annotation(Placement(transformation(extent={{-50,30},{-30,50}})));
    final parameter Real sumB = computeColSum(xlsxfile, "B") "Sum of column B";
    function computeColSum "Compute column sum"
      extends Modelica.Icons.Function;
      input XLSXFile xlsxfile "Excel XLSX file record";
      input String col = "A" "Column";
      input String sheetName="" "Sheet name";
      input Integer startRow = 2 "Start row";
      input Integer endRow = Functions.XLSX.getArrayRows2D(sheetName=sheetName, xlsx=xlsxfile.xlsx) "End row";
      output Real colSum "Column sum";
      protected
        Real val;
        Boolean exist;
      algorithm
        colSum := 0.0;
        for row in startRow:endRow loop
          (val, exist) := Functions.XLSX.getReal(cellAddress=col + String(row), sheetName=sheetName, xlsx=xlsxfile.xlsx);
          if exist then
            colSum := colSum + val;
          end if;
        end for;
      annotation(Documentation(info="<html><p>This function computes the column sum of a given column and sheet of an Excel XLSX file.</p></html>"));
    end computeColSum;
    equation
      connect(clock.y,gain1.u) annotation(Line(points={{-29,70},{-17,70}}, color={0,0,127}));
      connect(clock.y,gain2.u) annotation(Line(points={{-29,70},{-22,70},{-22,40},{-17,40}}, color={0,0,127}));
    annotation(experiment(StopTime=1),
      Documentation(info="<html><p>This example model reads the gain parameters from different cells and sheets of the Excel XLSX file <a href=\"modelica://ExternData/Resources/Examples/test.xlsx\">test.xlsx</a>. For gain1 the gain parameter is read as Real value using the function <a href=\"modelica://ExternData.XLSXFile.getReal\">ExternData.XLSXFile.getReal</a>. For gain2 the String value is retrieved by function <a href=\"modelica://ExternData.XLSXFile.getString\">ExternData.XLSXFile.getString</a> and converted to a Real value (using the utility function <a href=\"modelica://Modelica.Utilities.Strings.scanReal\">Modelica.Utilities.Strings.scanReal</a>). For timeTable the table parameter is read as Real array of dimension 3x2 by function <a href=\"modelica://ExternData.XLSXFile.getRealArray2D\">ExternData.XLSXFile.getRealArray2D</a>. The read parameters are assigned by parameter bindings to the appropriate model parameters.</p></html>"));
  end XLSXTest;

  model XMLTest "XML file read test"
    extends Modelica.Icons.Example;
    parameter String setName="set1" "Parameter set name" annotation(choices(choice="set1" "First parameter set", choice="set2" "Second parameter set"));
    inner parameter XMLFile xmlfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xml")) "XML file" annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
    Modelica.Blocks.Math.Gain gain1(k=xmlfile.getReal(setName + ".gain.k")) annotation(Placement(transformation(extent={{-15,60},{5,80}})));
    Modelica.Blocks.Math.Gain gain2(k=Modelica.Utilities.Strings.scanReal(xmlfile.getString(setName + ".gain.k"))) annotation(Placement(transformation(extent={{-15,30},{5,50}})));
    Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
    final parameter Integer m = xmlfile.getArrayRows2D("table1") "Number of rows in 2D array";
    Modelica.Blocks.Sources.TimeTable timeTable(table=xmlfile.getRealArray2D("table1", 3, 2)) annotation(Placement(transformation(extent={{-50,30},{-30,50}})));
    equation
      connect(clock.y,gain1.u) annotation(Line(points={{-29,70},{-17,70}}, color={0,0,127}));
      connect(clock.y,gain2.u) annotation(Line(points={{-29,70},{-22,70},{-22,40},{-17,40}}, color={0,0,127}));
    annotation(experiment(StopTime=1),
      Documentation(info="<html><p>This example model reads the gain and table parameters from different nodes of the XML file <a href=\"modelica://ExternData/Resources/Examples/test.xml\">test.xml</a>. For gain1 the gain parameter is read as Real value using the function <a href=\"modelica://ExternData.XMLFile.getReal\">ExternData.XMLFile.getReal</a>. For gain2 the String value is retrieved by function <a href=\"modelica://ExternData.XMLFile.getString\">ExternData.XMLFile.getString</a> and converted to a Real value (using the utility function <a href=\"modelica://Modelica.Utilities.Strings.scanReal\">Modelica.Utilities.Strings.scanReal</a>). For timeTable the table parameter is read as Real array of dimension 3x2 by function <a href=\"modelica://ExternData.XMLFile.getRealArray2D\">ExternData.XMLFile.getRealArray2D</a>. The read parameters are assigned by parameter bindings to the appropriate model parameters.</p></html>"));
  end XMLTest;

  model TIRTest "TIR file read test"
    extends Modelica.Icons.Example;
    inner parameter TIRFile tirfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/sample.tir")) "TIR file" annotation(Placement(transformation(extent={{-80,60},{-60,80}})));
    Modelica.Blocks.Sources.Constant pcx1(k=tirfile.getReal("PCX1", "LONGITUDINAL_COEFFICIENTS")) annotation(Placement(transformation(extent={{-50,60},{-30,80}})));
    Modelica.Blocks.Sources.Constant pcy1(k=tirfile.getReal("PCY1", "LATERAL_COEFFICIENTS")) annotation(Placement(transformation(extent={{-50,30},{-30,50}})));
    annotation(experiment(StopTime=1),
      Documentation(info="<html><p>This example model reads the tire coefficients from different sections of the TIR file <a href=\"modelica://ExternData/Resources/Examples/sample.tir\">sample.tir</a>. The coefficients are read as Real values using the function <a href=\"modelica://ExternData.TIRFile.getReal\">ExternData.TIRFile.getReal</a>. The read parameters are assigned by parameter bindings to the appropriate model parameters.</p></html>"));
  end TIRTest;
end Examples;
