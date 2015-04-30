within;
package ExternData "Library to read data from INI, JSON or XML files"
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

    annotation(defaultComponentName="inifile");
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

    annotation(defaultComponentName="jsonfile");
  end JSONFile;

  model XMLFile "Read data values from XML file"
    parameter String fileName "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="XML files (*.xml)",
        caption="Open file")));

    final function getReal = Functions.XML.getReal(xml=xml) "Get scalar Real value from XML file";
    final function getInteger = Functions.XML.getInteger(xml=xml) "Get scalar Integer value from XML file";
    final function getBoolean = Functions.XML.getBoolean(xml=xml) "Get scalar Boolean value from XML file";
    final function getString = Functions.XML.getString(xml=xml) "Get scalar String value from XML file";

    protected
      inner parameter Types.ExternXMLFile xml=Types.ExternXMLFile(fileName);

    annotation(defaultComponentName="xmlfile");
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
      annotation(experiment(StopTime=1));
    end XMLTest1;

    model XMLTest2 "XML Real read test with parameter binding"
      extends Modelica.Icons.Example;
      XMLFile xmlfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xml")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k=xmlfile.getReal("set2.gain.k")) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1));
    end XMLTest2;

    model XMLTest3 "XML Real read test with parameter binding and fixed=false"
      extends Modelica.Icons.Example;
      XMLFile xmlfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xml")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)=xmlfile.getReal("set2.gain.k")) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1));
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
      annotation(experiment(StopTime=1));
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
      annotation(experiment(StopTime=1));
    end XMLTest5;

    model INITest1 "INI Real read test with initial equation"
      extends Modelica.Icons.Example;
      INIFile inifile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.ini")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      initial equation
        gain.k = inifile.getReal("gain.k");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1));
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
      annotation(experiment(StopTime=1));
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
      annotation(experiment(StopTime=1));
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
      annotation(experiment(StopTime=1));
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
      annotation(experiment(StopTime=1));
    end JSONTest1;

    model JSONTest2 "JSON String read test from section with initial equation"
      extends Modelica.Icons.Example;
      JSONFile jsonfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.json")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      initial equation
        gain.k = Modelica.Utilities.Strings.scanReal(jsonfile.getString("set1.gain.k"));
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1));
    end JSONTest2;

    model JSONTest3 "JSON Integer read test from section with initial equation"
      extends Modelica.Icons.Example;
      JSONFile jsonfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.json")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      initial equation
        gain.k = jsonfile.getInteger("set2.gain.k");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1));
    end JSONTest3;
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
            Include="#include \"ED_INIFile.h\"",
            Library = "ED_INIFile");
        end getReal;

        function getInteger
          extends Interfaces.partialGetInteger;
          input Types.ExternINIFile ini;
          input String section="";
          external "C" y=ED_getIntFromINI(ini, varName, section) annotation(
            Include="#include \"ED_INIFile.h\"",
            Library = "ED_INIFile");
        end getInteger;

        function getString
          extends Interfaces.partialGetString;
          input Types.ExternINIFile ini;
          input String section="";
          external "C" str=ED_getStringFromINI(ini, varName, section) annotation(
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
            Include="#include \"ED_JSONFile.h\"",
             Library = {"ED_XMLFile", "expat"});
        end getReal;

        function getInteger
          extends Interfaces.partialGetInteger;
          input Types.ExternJSONFile json;
          external "C" y=ED_getIntFromJSON(json, varName) annotation(
            Include="#include \"ED_JSONFile.h\"",
             Library = {"ED_XMLFile", "expat"});
        end getInteger;

        function getString
          extends Interfaces.partialGetString;
          input Types.ExternJSONFile json;
          external "C" str=ED_getStringFromJSON(json, varName) annotation(
            Include="#include \"ED_JSONFile.h\"",
             Library = {"ED_XMLFile", "expat"});
        end getString;
      end Internal;
    end JSON;

    package XML
      extends Modelica.Icons.Package;
      function getReal
        extends Interfaces.partialGetReal;
        input Types.ExternXMLFile xml;
        algorithm
          y := Internal.getReal(xml=xml, varName=varName);
        annotation(Inline=true);
      end getReal;

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
            Include="#include \"ED_XMLFile.h\"",
            Library = {"ED_XMLFile", "expat"});
        end getReal;

        function getInteger
          extends Interfaces.partialGetInteger;
          input Types.ExternXMLFile xml;
          external "C" y=ED_getIntFromXML(xml, varName) annotation(
            Include="#include \"ED_XMLFile.h\"",
            Library = {"ED_XMLFile", "expat"});
        end getInteger;

        function getString
          extends Interfaces.partialGetString;
          input Types.ExternXMLFile xml;
          external "C" str=ED_getStringFromXML(xml, varName) annotation(
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
          Include="#include \"ED_INIFile.h\"",
          Library = "ED_INIFile");
      end constructor;

      function destructor
        input ExternINIFile ini;
        external "C" ED_destroyINI(ini) annotation(
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
          Include="#include \"ED_JSONFile.h\"",
          Library = {"ED_XMLFile", "expat"});
      end constructor;

      function destructor
        input ExternJSONFile json;
        external "C" ED_destroyJSON(json) annotation(
          Include="#include \"ED_JSONFile.h\"",
          Library = {"ED_XMLFile", "expat"});
      end destructor;
    end ExternJSONFile;

    class ExternXMLFile
      extends ExternalObject;
      function constructor
        input String fileName;
        output ExternXMLFile xml;
        external "C" xml=ED_createXML(fileName) annotation(
          Include="#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "expat"});
      end constructor;

      function destructor
        input ExternXMLFile xml;
        external "C" ED_destroyXML(xml) annotation(
          Include="#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "expat"});
      end destructor;
    end ExternXMLFile;
  end Types;

  annotation(uses(Modelica(version="3.2.1")));
end ExternData;
