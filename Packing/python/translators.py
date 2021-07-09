"""Python module for configuring translators"""

from LDMX.Framework import ldmxcfg

class Translator() :
    """Base class for all translator configuration classes

    Does the bare-minimum of including the library the translators
    are compiled into and defining the class name that is necessary
    to determine which C++ translator to configure.
    """

    def __init__(self, class_name, module_name = 'Packing::Translators') :
        ldmxcfg.Process.addModule(module_name)
        self.class_name = class_name

class Hgcroc(Translator) :
    """Configuration for Hgcroc Translator configuration

    The Hgcroc translator is not configured with any parameters.
    """

    def __init__(self, pass_regex, object_regex) :
        super().__init__('packing::translators::Hgcroc','Packing::Translators')
