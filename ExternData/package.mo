within;
package ExternData "Library to read data from INI, JSON, Excel XLS or XML files"
  extends Modelica.Icons.Package;
  model INIFile "Read data values from INI file"
    parameter String fileName "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="INI files (*.ini)",
        caption="Open file")));

    final function getReal = Functions.INI.getReal(ini=ini) "Get scalar Real value from INI file";
    final function getInteger = Functions.INI.getInteger(ini=ini) "Get scalar Integer value from INI file";
    final function getBoolean = Functions.INI.getBoolean(ini=ini) "Get scalar Boolean value from INI file";
    final function getString = Functions.INI.getString(ini=ini) "Get scalar String value from INI file";

    protected
      inner parameter Types.ExternINIFile ini=Types.ExternINIFile(fileName);

    annotation(
      defaultComponentName="inifile",
      Icon(graphics={
        Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
        Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillPattern=FillPattern.Solid),
        Text(lineColor={0,0,255},extent={{-85,-10},{85,-55}},textString="[ini]"),
        Text(lineColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end INIFile;

  model JSONFile "Read data values from JSON file"
    parameter String fileName "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="JSON files (*.json)",
        caption="Open file")));

    final function getReal = Functions.JSON.getReal(json=json) "Get scalar Real value from JSON file";
    final function getInteger = Functions.JSON.getInteger(json=json) "Get scalar Integer value from JSON file";
    final function getBoolean = Functions.JSON.getBoolean(json=json) "Get scalar Boolean value from JSON file";
    final function getString = Functions.JSON.getString(json=json) "Get scalar String value from JSON file";

    protected
      inner parameter Types.ExternJSONFile json=Types.ExternJSONFile(fileName);

    annotation(
      defaultComponentName="jsonfile",
      Icon(graphics={
        Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
        Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillPattern=FillPattern.Solid),
        Text(lineColor={0,0,255},extent={{-85,-10},{85,-55}},textString="{\"json\"}"),
        Text(lineColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end JSONFile;

  model XLSFile "Read data values from XLS file"
    parameter String fileName "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="Excel files (*.xls)",
        caption="Open file")));
    parameter String encoding="UTF-8" "Encoding";

    final function getReal = Functions.XLS.getReal(xls=xls) "Get scalar Real value from XLS file";
    final function getInteger = Functions.XLS.getInteger(xls=xls) "Get scalar Integer value from XLS file";
    final function getBoolean = Functions.XLS.getBoolean(xls=xls) "Get scalar Boolean value from XLS file";
    final function getString = Functions.XLS.getString(xls=xls) "Get scalar String value from XLS file";

    protected
      inner parameter Types.ExternXLSFile xls=Types.ExternXLSFile(fileName, encoding);

    annotation(
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

  model XMLFile "Read data values from XML file"
    parameter String fileName "File where external data is stored"
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
      inner parameter Types.ExternXMLFile xml=Types.ExternXMLFile(fileName);

    annotation(
      defaultComponentName="xmlfile",
      Icon(graphics={
        Line(points={{-40,90},{-90,40},{-90,-90},{90,-90},{90,90},{-40,90}}),
        Polygon(points={{-40,90},{-40,40},{-90,40},{-40,90}},fillColor={255,128,0},fillPattern=FillPattern.Solid),
        Text(lineColor={255,128,0},extent={{-85,-10},{85,-55}},textString="<?xml?>"),
        Text(lineColor={0,0,255},extent={{-150,150},{150,110}},textString="%name")}));
  end XMLFile;

  package Examples
    extends Modelica.Icons.ExamplesPackage;
    model XMLTest1 "XML Real read test with initial equation"
      extends Modelica.Icons.Example;
      XMLFile xmlfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xml")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      initial equation
        gain.k = xmlfile.getReal("set1.gain.k");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the XML file <a href=\"modelica://ExternData/Resources/Examples/test.xml\">test.xml</a> and assigns its Real value in an initial equation to the gain block.</p></html>"));
    end XMLTest1;

    model XMLTest2 "XML Real read test with parameter binding"
      extends Modelica.Icons.Example;
      XMLFile xmlfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xml")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k=xmlfile.getReal("set2.gain.k")) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1),
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the XML file <a href=\"modelica://ExternData/Resources/Examples/test.xml\">test.xml</a> and assigns its Real value to the gain block by a parameter binding.</p><p>This probably is non-standard Modelica but works in Dymola though.</p></html>"));
    end XMLTest2;

    model XMLTest3 "XML Real read test with parameter binding and fixed=false"
      extends Modelica.Icons.Example;
      XMLFile xmlfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xml")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)=xmlfile.getReal("set2.gain.k")) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1),
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the XML file <a href=\"modelica://ExternData/Resources/Examples/test.xml\">test.xml</a> and assigns its Real value to the gain block by a parameter binding. In comparison with <a href=\"modelica://ExternData.Examples.XMLTest2\">XMLTest2</a> <code>gain.k.fixed</code> is set to <code>false</code>.</p><p>This is non-standard Modelica but works in SimulationX though.</p></html>"));
    end XMLTest3;

    model XMLTest4 "XML String read test with initial equation"
      extends Modelica.Icons.Example;
      XMLFile xmlfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xml")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      initial equation
        gain.k = Modelica.Utilities.Strings.scanReal(xmlfile.getString("set1.gain.k"));
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the XML file <a href=\"modelica://ExternData/Resources/Examples/test.xml\">test.xml</a>, retrieves its String value and assigns the scanned Real value (using <a href=\"modelica://Modelica.Utilities.Strings.scanReal\">Modelica.Utilities.Strings.scanReal</a>) in an initial equation to the gain block.</p></html>"));
    end XMLTest4;

    model XMLTest5 "XML Integer read test with initial equation"
      extends Modelica.Icons.Example;
      XMLFile xmlfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xml")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      initial equation
        gain.k = xmlfile.getInteger("set1.gain.k");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the XML file <a href=\"modelica://ExternData/Resources/Examples/test.xml\">test.xml</a> and assigns its Integer value in an initial equation to the gain block.</p></html>"));
    end XMLTest5;

    model XMLTest6 "XML Real 2D array read test with parameter binding"
      extends Modelica.Icons.Example;
      XMLFile xmlfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xml")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Sources.CombiTimeTable table(table=xmlfile.getRealArray2D("table", 3, 2)) annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      annotation(experiment(StopTime=1),
        Documentation(info="<html><p>Reads the table array parameter <code>table</code> from the XML file <a href=\"modelica://ExternData/Resources/Examples/test.xml\">test.xml</a> and assigns its Real values to the table matrix of the table block by a parameter binding.</p><p>This probably is non-standard Modelica but works in Dymola though.</p></html>"));
    end XMLTest6;

    model INITest1 "INI Real read test with initial equation"
      extends Modelica.Icons.Example;
      INIFile inifile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.ini")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      initial equation
        gain.k = inifile.getReal("gain.k");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the plain section of the INI file <a href=\"modelica://ExternData/Resources/Examples/test.ini\">test.ini</a> and assigns its Real value in an initial equation to the gain block.</p></html>"));
    end INITest1;

    model INITest2 "INI Real read test from section with initial equation"
      extends Modelica.Icons.Example;
      INIFile inifile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.ini")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      initial equation
        gain.k = inifile.getReal("gain.k", "set1");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the section [set1] of the INI file <a href=\"modelica://ExternData/Resources/Examples/test.ini\">test.ini</a> and assigns its Real value in an initial equation to the gain block.</p></html>"));
    end INITest2;

    model INITest3 "INI String read test from section with initial equation"
      extends Modelica.Icons.Example;
      INIFile inifile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.ini")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      initial equation
        gain.k = Modelica.Utilities.Strings.scanReal(inifile.getString("gain.k", "set1"));
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the INI file <a href=\"modelica://ExternData/Resources/Examples/test.ini\">test.ini</a>, retrieves its String value and assigns the scanned Real value (using <a href=\"modelica://Modelica.Utilities.Strings.scanReal\">Modelica.Utilities.Strings.scanReal</a>) in an initial equation to the gain block.</p></html>"));
    end INITest3;

    model INITest4 "INI Integer read test from section with initial equation"
      extends Modelica.Icons.Example;
      INIFile inifile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.ini")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      initial equation
        gain.k = inifile.getInteger("gain.k", "set1");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the INI file <a href=\"modelica://ExternData/Resources/Examples/test.ini\">test.ini</a> and assigns its Integer value in an initial equation to the gain block.</p></html>"));
    end INITest4;

    model JSONTest1 "JSON Real read test with initial equation"
      extends Modelica.Icons.Example;
      JSONFile jsonfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.json")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      initial equation
        gain.k = jsonfile.getReal("set1.gain.k");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the JSON file <a href=\"modelica://ExternData/Resources/Examples/test.json\">test.json</a> and assigns its Real value in an initial equation to the gain block.</p></html>"));
    end JSONTest1;

    model JSONTest2 "JSON String read test with initial equation"
      extends Modelica.Icons.Example;
      JSONFile jsonfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.json")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      initial equation
        gain.k = Modelica.Utilities.Strings.scanReal(jsonfile.getString("set1.gain.k"));
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the JSON file <a href=\"modelica://ExternData/Resources/Examples/test.json\">test.json</a>, retrieves its String value and assigns the scanned Real value (using <a href=\"modelica://Modelica.Utilities.Strings.scanReal\">Modelica.Utilities.Strings.scanReal</a>) in an initial equation to the gain block.</p></html>"));
    end JSONTest2;

    model JSONTest3 "JSON Integer read test with initial equation"
      extends Modelica.Icons.Example;
      JSONFile jsonfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.json")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      initial equation
        gain.k = jsonfile.getInteger("set2.gain.k");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the JSON file <a href=\"modelica://ExternData/Resources/Examples/test.json\">test.json</a> and assigns its Integer value in an initial equation to the gain block.</p></html>"));
    end JSONTest3;

    model XLSTest1 "XLS Real read test from default sheet with initial equation"
      extends Modelica.Icons.Example;
      XLSFile xlsfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xls")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      initial equation
        gain.k = xlsfile.getReal("B2");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from the first sheet of the Excel file <a href=\"modelica://ExternData/Resources/Examples/test.xls\">test.xls</a> and assigns its Real value in an initial equation to the gain block.</p></html>"));
    end XLSTest1;

    model XLSTest2 "XLS Real read test from sheet with initial equation"
      extends Modelica.Icons.Example;
      XLSFile xlsfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xls")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      initial equation
        gain.k = xlsfile.getReal("B2", "set2");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from sheet set2 of the Excel file <a href=\"modelica://ExternData/Resources/Examples/test.xls\">test.xls</a> and assigns its Real value in an initial equation to the gain block.</p></html>"));
    end XLSTest2;

    model XLSTest3 "XLS String read test from sheet with initial equation"
      extends Modelica.Icons.Example;
      XLSFile xlsfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xls")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      initial equation
        gain.k = Modelica.Utilities.Strings.scanReal(xlsfile.getString("B2", "set2"));
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from sheet set2 of the Excel file <a href=\"modelica://ExternData/Resources/Examples/test.xls\">test.xls</a>, retrieves its String value and assigns the scanned Real value (using <a href=\"modelica://Modelica.Utilities.Strings.scanReal\">Modelica.Utilities.Strings.scanReal</a>) in an initial equation to the gain block.</p></html>"));
    end XLSTest3;

    model XLSTest4 "XLS Integer read test from sheet with initial equation"
      extends Modelica.Icons.Example;
      XLSFile xlsfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xls")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      initial equation
        gain.k = xlsfile.getInteger("B2", "set1");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1), preferredView="text",
        Documentation(info="<html><p>Reads the gain parameter <code>k</code> from sheet set1 of the the Excel file <a href=\"modelica://ExternData/Resources/Examples/test.xls\">test.xls</a> and assigns its Integer value in an initial equation to the gain block.</p></html>"));
    end XLSTest4;
  end Examples;

  package Functions
    extends Modelica.Icons.Package;
    package INI
      extends Modelica.Icons.Package;
      function getReal
        extends Modelica.Icons.Function;
        input String varName;
        input String section="";
        input Types.ExternINIFile ini;
        output Real y;
        algorithm
          y := Internal.getReal(ini=ini, varName=varName, section=section);
        annotation(Inline=true);
      end getReal;

      function getInteger
        extends Modelica.Icons.Function;
        input String varName;
        input String section="";
        input Types.ExternINIFile ini;
        output Integer y;
        algorithm
          y := Internal.getInteger(ini=ini, varName=varName, section=section);
        annotation(Inline=true);
      end getInteger;

      function getBoolean
        extends Modelica.Icons.Function;
        input String varName;
        input String section="";
        input Types.ExternINIFile ini;
        output Boolean y;
        algorithm
          y := Internal.getReal(ini=ini, varName=varName, section=section) <> 0;
        annotation(Inline=true);
      end getBoolean;

      function getString
        extends Modelica.Icons.Function;
        input String varName;
        input String section="";
        input Types.ExternINIFile ini;
        output String str;
        algorithm
          str := Internal.getString(ini=ini, varName=varName, section=section);
        annotation(Inline=true);
      end getString;

      package Internal
        extends Modelica.Icons.InternalPackage;
        function getReal
          extends Interfaces.partialGetReal;
          input Types.ExternINIFile ini;
          input String section="";
          external "C" y=ED_getDoubleFromINI(ini, varName, section) annotation(
            __iti_dll = "ITI_ED_INIFile.dll",
            Include="#include \"ED_INIFile.h\"",
            Library = "ED_INIFile");
        end getReal;

        function getInteger
          extends Interfaces.partialGetInteger;
          input Types.ExternINIFile ini;
          input String section="";
          external "C" y=ED_getIntFromINI(ini, varName, section) annotation(
            __iti_dll = "ITI_ED_INIFile.dll",
            Include="#include \"ED_INIFile.h\"",
            Library = "ED_INIFile");
        end getInteger;

        function getString
          extends Interfaces.partialGetString;
          input Types.ExternINIFile ini;
          input String section="";
          external "C" str=ED_getStringFromINI(ini, varName, section) annotation(
            __iti_dll = "ITI_ED_INIFile.dll",
            Include="#include \"ED_INIFile.h\"",
            Library = "ED_INIFile");
        end getString;
      end Internal;
    end INI;

    package JSON
      extends Modelica.Icons.Package;
      function getReal
        extends Interfaces.partialGetReal;
        input Types.ExternJSONFile json;
        algorithm
          y := Internal.getReal(json=json, varName=varName);
        annotation(Inline=true);
      end getReal;

      function getInteger
        extends Interfaces.partialGetInteger;
        input Types.ExternJSONFile json;
        algorithm
          y := Internal.getInteger(json=json, varName=varName);
        annotation(Inline=true);
      end getInteger;

      function getBoolean
        extends Interfaces.partialGetBoolean;
        input Types.ExternJSONFile json;
        algorithm
          y := Internal.getReal(json=json, varName=varName) <> 0;
        annotation(Inline=true);
      end getBoolean;

      function getString
        extends Interfaces.partialGetString;
        input Types.ExternJSONFile json;
        algorithm
          str := Internal.getString(json=json, varName=varName);
        annotation(Inline=true);
      end getString;

      package Internal
        extends Modelica.Icons.InternalPackage;
        function getReal
          extends Interfaces.partialGetReal;
          input Types.ExternJSONFile json;
          external "C" y=ED_getDoubleFromJSON(json, varName) annotation(
            __iti_dll = "ITI_ED_JSONFile.dll",
            Include="#include \"ED_JSONFile.h\"",
            Library = "ED_JSONFile");
        end getReal;

        function getInteger
          extends Interfaces.partialGetInteger;
          input Types.ExternJSONFile json;
          external "C" y=ED_getIntFromJSON(json, varName) annotation(
            __iti_dll = "ITI_ED_JSONFile.dll",
            Include="#include \"ED_JSONFile.h\"",
            Library = "ED_JSONFile");
        end getInteger;

        function getString
          extends Interfaces.partialGetString;
          input Types.ExternJSONFile json;
          external "C" str=ED_getStringFromJSON(json, varName) annotation(
            __iti_dll = "ITI_ED_JSONFile.dll",
            Include="#include \"ED_JSONFile.h\"",
            Library = "ED_JSONFile");
        end getString;
      end Internal;
    end JSON;

    package XLS
      extends Modelica.Icons.Package;
      function getReal
        extends Modelica.Icons.Function;
        input String cellAddress="A1";
        input String sheetName="";
        input Types.ExternXLSFile xls;
        output Real y;
        algorithm
          y := Internal.getReal(xls=xls, cellAddress=cellAddress, sheetName=sheetName);
        annotation(Inline=true);
      end getReal;

      function getInteger
        extends Modelica.Icons.Function;
        input String cellAddress="A1";
        input String sheetName="";
        input Types.ExternXLSFile xls;
        output Integer y;
        algorithm
          y := Internal.getInteger(xls=xls, cellAddress=cellAddress, sheetName=sheetName);
        annotation(Inline=true);
      end getInteger;

      function getBoolean
        extends Modelica.Icons.Function;
        input String cellAddress="A1";
        input String sheetName="";
        input Types.ExternXLSFile xls;
        output Boolean y;
        algorithm
          y := Internal.getReal(xls=xls, cellAddress=cellAddress, sheetName=sheetName) <> 0;
        annotation(Inline=true);
      end getBoolean;

      function getString
        extends Modelica.Icons.Function;
        input String cellAddress="A1";
        input String sheetName="";
        input Types.ExternXLSFile xls;
        output String str;
        algorithm
          str := Internal.getString(xls=xls, cellAddress=cellAddress, sheetName=sheetName);
        annotation(Inline=true);
      end getString;

      package Internal
        extends Modelica.Icons.InternalPackage;
        function getReal
          extends Modelica.Icons.Function;
          input String cellAddress="A1";
          input Types.ExternXLSFile xls;
          input String sheetName="";
          output Real y;
          external "C" y=ED_getDoubleFromXLS(xls, cellAddress, sheetName) annotation(
            __iti_dll = "ITI_ED_XLSFile.dll",
            Include="#include \"ED_XLSFile.h\"",
            Library = "ED_XLSFile");
        end getReal;

        function getInteger
          extends Modelica.Icons.Function;
          input String cellAddress="A1";
          input Types.ExternXLSFile xls;
          input String sheetName="";
          output Integer y;
          external "C" y=ED_getIntFromXLS(xls, cellAddress, sheetName) annotation(
            __iti_dll = "ITI_ED_XLSFile.dll",
            Include="#include \"ED_XLSFile.h\"",
            Library = "ED_XLSFile");
        end getInteger;

        function getString
          extends Modelica.Icons.Function;
          input String cellAddress="A1";
          input Types.ExternXLSFile xls;
          input String sheetName="";
          output String str;
          external "C" str=ED_getStringFromXLS(xls, cellAddress, sheetName) annotation(
            __iti_dll = "ITI_ED_XLSFile.dll",
            Include="#include \"ED_XLSFile.h\"",
            Library = "ED_XLSFile");
        end getString;
      end Internal;
    end XLS;

    package XML
      extends Modelica.Icons.Package;
      function getReal
        extends Interfaces.partialGetReal;
        input Types.ExternXMLFile xml;
        algorithm
          y := Internal.getReal(xml=xml, varName=varName);
        annotation(Inline=true);
      end getReal;

      function getRealArray1D
        extends Modelica.Icons.Function;
        input String varName;
        input Integer n=1;
        input Types.ExternXMLFile xml;
        output Real y[n];
        algorithm
          y := Internal.getRealArray1D(xml=xml, varName=varName, n=n);
        annotation(Inline=true);
      end getRealArray1D;

      function getRealArray2D
        extends Modelica.Icons.Function;
        input String varName;
        input Integer m=1;
        input Integer n=1;
        input Types.ExternXMLFile xml;
        output Real y[m,n];
        algorithm
          y := Internal.getRealArray2D(xml=xml, varName=varName, m=m, n=n);
        annotation(Inline=true);
      end getRealArray2D;

      function getInteger
        extends Interfaces.partialGetInteger;
        input Types.ExternXMLFile xml;
        algorithm
          y := Internal.getInteger(xml=xml, varName=varName);
        annotation(Inline=true);
      end getInteger;

      function getBoolean
        extends Interfaces.partialGetBoolean;
        input Types.ExternXMLFile xml;
        algorithm
          y := Internal.getReal(xml=xml, varName=varName) <> 0;
        annotation(Inline=true);
      end getBoolean;

      function getString
        extends Interfaces.partialGetString;
        input Types.ExternXMLFile xml;
        algorithm
          str := Internal.getString(xml=xml, varName=varName);
        annotation(Inline=true);
      end getString;

      package Internal
        extends Modelica.Icons.InternalPackage;
        function getReal
          extends Interfaces.partialGetReal;
          input Types.ExternXMLFile xml;
          external "C" y=ED_getDoubleFromXML(xml, varName) annotation(
            __iti_dll = "ITI_ED_XMLFile.dll",
            Include="#include \"ED_XMLFile.h\"",
            Library = {"ED_XMLFile", "expat"});
        end getReal;

        function getRealArray2D
          input String varName;
          input Integer m=1;
          input Integer n=1;
          input Types.ExternXMLFile xml;
          output Real y[m,n];
          external "C" ED_getDoubleArray2DFromXML(xml, varName, y, size(y, 1), size(y, 2)) annotation(
            __iti_dll = "ITI_ED_XMLFile.dll",
            Include="#include \"ED_XMLFile.h\"",
            Library = {"ED_XMLFile", "expat"});
        end getRealArray2D;

        function getRealArray1D
          input String varName;
          input Integer n=1;
          input Types.ExternXMLFile xml;
          output Real y[n];
          external "C" ED_getDoubleArray1DFromXML(xml, varName, y, size(y, 1)) annotation(
            __iti_dll = "ITI_ED_XMLFile.dll",
            Include="#include \"ED_XMLFile.h\"",
            Library = {"ED_XMLFile", "expat"});
        end getRealArray1D;

        function getInteger
          extends Interfaces.partialGetInteger;
          input Types.ExternXMLFile xml;
          external "C" y=ED_getIntFromXML(xml, varName) annotation(
            __iti_dll = "ITI_ED_XMLFile.dll",
            Include="#include \"ED_XMLFile.h\"",
            Library = {"ED_XMLFile", "expat"});
        end getInteger;

        function getString
          extends Interfaces.partialGetString;
          input Types.ExternXMLFile xml;
          external "C" str=ED_getStringFromXML(xml, varName) annotation(
            __iti_dll = "ITI_ED_XMLFile.dll",
            Include="#include \"ED_XMLFile.h\"",
            Library = {"ED_XMLFile", "expat"});
        end getString;
      end Internal;
    end XML;
  end Functions;

  package Interfaces
    extends Modelica.Icons.InterfacesPackage;
    partial function partialGetReal
      extends Modelica.Icons.Function;
      input String varName;
      output Real y;
    end partialGetReal;

    partial function partialGetInteger
      extends Modelica.Icons.Function;
      input String varName;
      output Integer y;
    end partialGetInteger;

    partial function partialGetBoolean
      extends Modelica.Icons.Function;
      input String varName;
      output Boolean y;
    end partialGetBoolean;

    partial function partialGetString
      extends Modelica.Icons.Function;
      input String varName;
      output String str;
    end partialGetString;
  end Interfaces;

  package Types
    extends Modelica.Icons.TypesPackage;
    class ExternINIFile
      extends ExternalObject;
      function constructor
        input String fileName;
        output ExternINIFile ini;
        external "C" ini=ED_createINI(fileName) annotation(
          __iti_dll = "ITI_ED_INIFile.dll",
          Include="#include \"ED_INIFile.h\"",
          Library = "ED_INIFile");
      end constructor;

      function destructor
        input ExternINIFile ini;
        external "C" ED_destroyINI(ini) annotation(
          __iti_dll = "ITI_ED_INIFile.dll",
          Include="#include \"ED_INIFile.h\"",
          Library = "ED_INIFile");
      end destructor;
    end ExternINIFile;

    class ExternJSONFile
      extends ExternalObject;
      function constructor
        input String fileName;
        output ExternJSONFile json;
        external "C" json=ED_createJSON(fileName) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          Include="#include \"ED_JSONFile.h\"",
          Library = "ED_JSONFile");
      end constructor;

      function destructor
        input ExternJSONFile json;
        external "C" ED_destroyJSON(json) annotation(
          __iti_dll = "ITI_ED_JSONFile.dll",
          Include="#include \"ED_JSONFile.h\"",
          Library = "ED_JSONFile");
      end destructor;
    end ExternJSONFile;

    class ExternXLSFile
      extends ExternalObject;
      function constructor
        input String fileName;
        input String encoding="UTF-8";
        output ExternXLSFile xls;
        external "C" xls=ED_createXLS(fileName, encoding) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          Include="#include \"ED_XLSFile.h\"",
          Library = "ED_XLSFile");
      end constructor;

      function destructor
        input ExternXLSFile xls;
        external "C" ED_destroyXLS(xls) annotation(
          __iti_dll = "ITI_ED_XLSFile.dll",
          Include="#include \"ED_XLSFile.h\"",
          Library = "ED_XLSFile");
      end destructor;
    end ExternXLSFile;

    class ExternXMLFile
      extends ExternalObject;
      function constructor
        input String fileName;
        output ExternXMLFile xml;
        external "C" xml=ED_createXML(fileName) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          Include="#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "expat"});
      end constructor;

      function destructor
        input ExternXMLFile xml;
        external "C" ED_destroyXML(xml) annotation(
          __iti_dll = "ITI_ED_XMLFile.dll",
          Include="#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "expat"});
      end destructor;
    end ExternXMLFile;
  end Types;

  annotation(uses(Modelica(version="3.2.1")), version="1.0.4-dev",
    Documentation(info="<html><p>Library <b>ExternData</b> is a Modelica utility library for reading data from INI, JSON, Excel XLS and XML files.</p></html>"));
end ExternData;
