within;
package ExternData
  extends Modelica.Icons.Package;
  model XMLFile
    parameter String fileName "File where external data is stored"
      annotation(Dialog(
        loadSelector(filter="XML files (*.xml)",
        caption="Open file")));

    replaceable function getReal = Functions.getReal(
      xml=xml
    ) constrainedby Interfaces.partialGetReal;

    parameter Types.ExternXMLFile xml=Types.ExternXMLFile(fileName);

    annotation(
      defaultComponentName="xmlfile",
      defaultComponentPrefixes="inner");
  end XMLFile;

  package Types
    extends Modelica.Icons.TypesPackage;
    class ExternXMLFile
      extends ExternalObject;
      function constructor
        input String fileName;
        output ExternXMLFile xml;
        external "C" xml=ED_createXML(fileName) annotation(Include="#include \"ED_XMLFile.c\"");
      end constructor;
  
      function destructor
        input ExternXMLFile xml;
        external "C" ED_destroyXML(xml) annotation(Include="#include \"ED_XMLFile.c\"");
      end destructor;
    end ExternXMLFile;
  end Types;
  
  package Interfaces
    extends Modelica.Icons.InterfacesPackage;
    partial function partialGetReal
      extends Modelica.Icons.Function;
      input String varName;
      output Real y;
    end partialGetReal;
  end Interfaces;

  package Functions
    extends Modelica.Icons.Package;
    function getReal
      extends Interfaces.partialGetReal;
      input Types.ExternXMLFile xml;
      external "C" y=ED_getDoubleFromXML(xml, varName) annotation(Include="#include \"ED_XMLFile.c\"");
    end getReal;
  end Functions;

  package Examples
    extends Modelica.Icons.ExamplesPackage;
    model XMLTest "XML Test"
      extends Modelica.Icons.Example;
      inner XMLFile xmlfile(fileName="dummy.xml") annotation(Placement(transformation(extent={{-81,60},{-61,80}})));
      Modelica.Blocks.Math.Gain gain(k=xmlfile.getReal("k")) annotation(Placement(transformation(extent={{-16,60},{4,80}})));
      Modelica.Blocks.Sources.Clock clock annotation(Placement(transformation(extent={{-51,60},{-31,80}})));
      equation
        connect(clock.y,gain.u) annotation(Line(points={{-30,70},{-18,70}}));
     annotation(experiment(StopTime=1));
    end XMLTest;
  end Examples;
end ExternData;
