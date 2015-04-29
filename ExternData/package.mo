within;
package ExternData
  extends Modelica.Icons.Package;
  model XMLFile
    parameter String fileName "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="XML files (*.xml)",
        caption="Open file")));

    final function getReal = Functions.XML.getReal(xml=xml);
    final function getString = Functions.XML.getString(xml=xml);

  protected
    inner parameter Types.ExternXMLFile xml=Types.ExternXMLFile(fileName);

    annotation(defaultComponentName="xmlfile");
  end XMLFile;

  package Examples
    extends Modelica.Icons.ExamplesPackage;
    model XMLTest1 "XML double read test with initial equation"
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

    model XMLTest2 "XML double read test with parameter binding"
      extends Modelica.Icons.Example;
      XMLFile xmlfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xml")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k=xmlfile.getReal("set2.gain.k")) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1));
    end XMLTest2;

    model XMLTest3 "XML double read test with parameter binding and fixed=false"
      extends Modelica.Icons.Example;
      XMLFile xmlfile(fileName=Modelica.Utilities.Files.loadResource("modelica://ExternData/Resources/Examples/test.xml")) annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)=xmlfile.getReal("set2.gain.k")) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
      annotation(experiment(StopTime=1));
    end XMLTest3;

    model XMLTest4 "XML string read test with initial equation"
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
  end Examples;

  package Functions
    extends Modelica.Icons.Package;
    package XML
      extends Modelica.Icons.Package;
      function getReal
        extends Interfaces.partialGetReal;
        input Types.ExternXMLFile xml;
        algorithm
          y := Internal.getReal(xml=xml, varName=varName);
        annotation(Inline=true);
      end getReal;

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

    partial function partialGetString
      extends Modelica.Icons.Function;
      input String varName;
      output String str;
    end partialGetString;
  end Interfaces;

  package Types
    extends Modelica.Icons.TypesPackage;
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
end ExternData;
