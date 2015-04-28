within;
package ExternData
  extends Modelica.Icons.Package;
  model XMLFile
    parameter String fileName "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="XML files (*.xml)",
        caption="Open file")));

  protected
    replaceable function getReal = Functions.getReal(
      xml=xml
    ) constrainedby Interfaces.partialGetReal;

    inner parameter Types.ExternXMLFile xml=Types.ExternXMLFile(fileName);

    annotation(defaultComponentName="xmlfile");
  end XMLFile;

  package Examples
    extends Modelica.Icons.ExamplesPackage;
    model XMLTest1 "XML read test with initial equation"
      extends Modelica.Icons.Example;
      XMLFile xmlfile(fileName="dummy.xml") annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k(fixed=false)) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      initial equation
        gain.k = xmlfile.getReal("k");
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
     annotation(experiment(StopTime=1));
    end XMLTest1;
  end Examples;

  package Functions
    extends Modelica.Icons.Package;
    function getReal
      extends Interfaces.partialGetReal;
      input Types.ExternXMLFile xml;
      algorithm
        y := Internal.getReal(xml=xml, varName=varName);
    end getReal;

    package Internal
      extends Modelica.Icons.InternalPackage;
      function getReal
        extends Interfaces.partialGetReal;
        input Types.ExternXMLFile xml;
        external "C" y=ED_getDoubleFromXML(xml, varName) annotation(
          Include="#include \"ED_XMLFile.h\"",
          Library = {"ED_XMLFile", "expat"});
      end getReal;
    end Internal;
  end Functions;

  package Interfaces
    extends Modelica.Icons.InterfacesPackage;
    partial function partialGetReal
      extends Modelica.Icons.Function;
      input String varName;
      output Real y;
    end partialGetReal;
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
