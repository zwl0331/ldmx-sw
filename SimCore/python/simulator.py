"""Package to help configure the simulation

Defines a derived class from ldmxcfg.Producer
with several helpful member functions.
"""

from LDMX.Framework.ldmxcfg import Producer

class _EventToReSim:
    """A class to hold the information identifying a specific event we wish to re-simulate

    This is an internal class used by simulator.resimulate in order to pass the event
    identification to the ReSimulator class

    Attributes
    ----------
    run: int
        run number of the event to re-sim, -1 if we don't care about the run
    event: int
        event number to re-sim, required
    """

    def __init__(self, event, run = -1):
        self.event = event
        self.run = run


class simulator(Producer):
    """A instance of the simulation configuration

    This class is derived from ldmxcfg.Producer and is mainly
    focused on providing helper functions that can be used instead
    of accessing the parameters member directly.

    The parameters that are lists ('preInitCommands', 'postInitCommands', 'actions', and 'generators')
    are initialized as empty lists so that we can append to them later.

    The ECal hit conbtibutions are enabled and compressed by default.

    Parameters
    ----------
    instance_name : str
        Name of this instance of the Simulator

    Attributes
    ----------
    generators : list of PrimaryGenerator
        Generators to use to make primaries
    detector : str
        Full path to detector description gdml (suggested to use setDetector)
    validate_detector : bool, optional
        Should we have Geant4 validate that the gdml is correctly formatted?
    sensitive_detectors : list[SensitiveDetector]
        List of sensitive detectors to load
    description : str
        Describe this run in a human-readable way
    scoringPlanes : str, optional
        Full path to the scoring planes gdml (suggested to use setDetector)
    beamSpotSmear : list of float, optional
        2 (x,y) or 3 (x,y,z) widths to smear ALL primary vertices by [mm]
    time_shift_primaries : bool
        Should we shift the times of primaries so that z=0mm corresponds to t=0ns? 
    preInitCommands : list of str, optional
        Geant4 commands to run before the run is initialized
    postInitCommands : list of str, optional
        Geant4 commands to run after run is initialized (but before run starts)
    actions : list of UserAction, optional
        Special User-defined actions to take during the simulation
    biasing_operators : list of XsecBiasingOperators, optional
        Operators for biasing specific particles to undergo specific processes
    dark_brem : DarkBrem
        Configuration options for dark brem process
    logging_prefix : str, optional
        Prefix to prepend any Geant4 logging files
    rootPrimaryGenUseSeed : bool, optional
        Use the seed stored in the EventHeader for random generation
    verbosity : int, optional
        Verbosity level to print
    """

    def __init__(self, instance_name ) :
        super().__init__( instance_name , "simcore::Simulator" , "SimCore" )

        #######################################################################
        # Required Parameters
        self.generators = [ ]
        self.detector = ''
        self.sensitive_detectors = [ ]
        self.description = ''

        #######################################################################
        # Optional Parameters (with helpful defaults)
        self.scoringPlanes = ''
        self.beamSpotSmear = [ ]
        self.time_shift_primaries = True
        self.preInitCommands = [ ]
        self.postInitCommands = [ ]
        self.actions = [ ]
        self.biasing_operators = [ ]
        self.logging_prefix = ''
        self.rootPrimaryGenUseSeed = False
        self.validate_detector = False
        self.verbosity = 0


        #Dark Brem stuff
        from LDMX.SimCore import dark_brem
        self.dark_brem = dark_brem.DarkBrem()

        # Default photonuclear model
        from LDMX.SimCore import photonuclear_models
        self.photonuclear_model = photonuclear_models.BertiniModel()

        from LDMX.SimCore import kaon_physics
        self.kaon_parameters = kaon_physics.KaonPhysics()

    def setDetector(self, det_name , include_scoring_planes = False ) :
        """Set the detector description with the option to include the scoring planes

        Parameters
        ----------
        det_name : str
            name of a detector in the Detectors module
        include_scoring_planes : bool
            True if you want to import and use scoring planes

        See Also
        --------
        LDMX.Detectors.makePath for definitions of the path making functions.
        sensitive_detectors for configuring the SDs
        """

        from LDMX.Detectors import makePath as mP
        from . import sensitive_detectors as sds
        self.detector = mP.makeDetectorPath( det_name )
        if 'v12' in det_name :
            trigscint = [ sds.TrigScintSD.up(), sds.TrigScintSD.tag(), sds.TrigScintSD.down() ]
        elif 'hcal-prototype' in det_name :
            trigscint = [ sds.TrigScintSD.testbeam() ]
        else :
            trigscint = [ sds.TrigScintSD.pad1(), sds.TrigScintSD.pad2(), sds.TrigScintSD.pad3() ]
        self.sensitive_detectors = [
                sds.TrackerSD.tagger(),
                sds.TrackerSD.recoil(),
                sds.HcalSD(),
                sds.EcalSD(),
                sds.TrigScintSD.target()
                ] + trigscint
        if include_scoring_planes :
            self.scoringPlanes = mP.makeScoringPlanesPath( det_name )
            self.sensitive_detectors.extend([
                sds.ScoringPlaneSD.ecal(),
                sds.ScoringPlaneSD.hcal(),
                sds.ScoringPlaneSD.target(),
                sds.ScoringPlaneSD.trigscint(),
                sds.ScoringPlaneSD.tracker(),
                sds.ScoringPlaneSD.magnet()
                ])

    def resimulate(self, which_events = None, which_runs = None):
        """Create a resimulator based on the simulator configuration.

        This is intended to ensure that a resimulator has the same configuration
        as the simulation that was used to generate the input file. If you
        require any changes to the simulation configuration, such as loading a
        modified geometry, you can make those changes after creating the
        resiulator.


        Parameters
        ----------
        which_events : list of event numbers, optional
            Which events from the input files to resimulate. If None,
            resimulate all events.

            Events that are not present in any of the input files will be
            ignored.

            For multiple input files, if an event number is present within more
            than one input file all versions will be resimulated unless the which_runs
            parameters is used to distinguish them.

        which_runs : list of run numbers, optional
            Which runs from the input files to resimulate, ignored if no
            events are listed. Runs not present in the input files will be
            ignored.

            If not provided, all runs will be resimulated (i.e. the run number
            check is ignored). If only one value is provided, all events requested
            are also required to have that value for their run number to be resimulated.
            If more than one value is provided, it must be the same length as the
            number of events requested so that the event/run number pair can be required.

        """
        resimulator = self
        resimulator.className = 'simcore::ReSimulator'
        if which_events is None:
            resimulator.resimulate_all_events = True
            resimulator.care_about_run = False
            resimulator.events_to_resimulate = [ ]
        elif isinstance(which_events, list):
            resimulator.resimulate_all_events = False
            if len(which_events) == 0:
                raise ValueError('which_events must contain at least one element if provided')
            if which_runs is None:
                resimulator.care_about_run = False
                resimulator.events_to_resimulate = [ _EventToReSim(event) for event in which_events ]
            elif isinstance(which_runs, int):
                resimulator.care_about_run = True
                resimulator.events_to_resimulate = [ _EventToReSim(event, which_runs) for event in which_events ]
            elif isinstance(which_runs, list):
                if len(which_runs) == 0:
                    raise ValueError('which_runs must have at least one value if provided as a list')
                if len(which_runs) != len(which_events):
                    raise ValueError('which_runs must have the same number of entries as which_events if more than one run is provided')
                resimulator.care_about_run = True
                resimulator.runs_to_resimulate = [ _EventToReSim(event, run) for event, run in zip(which_events, which_runs) ]
            else:
                raise ValueError('which_runs must be an int or a list of ints if provided')
        else:
            raise ValueError('which_events must be a list if provided')
        return resimulator
