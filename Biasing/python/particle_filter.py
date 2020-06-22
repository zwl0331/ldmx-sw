""" Example configurations for filtering out PN events by the properties of 
    of the reaction products.

"""

from LDMX.SimApplication import simcfg

def kaon_filter():
    """ Configuration for filtering photo-nuclear events whose products don't contain a kaon.


    Returns
    -------
    Instance of configured PhotoNuclearProductsFilter

    """
    particle_filter = simcfg.UserAction("particle_filter", "ldmx::PhotoNuclearProductsFilter")
    particle_filter.parameters['pdg_ids'] = [
            130, # K_L^0
            310, # K_S^0
            311, # K^0
            321  # K^+
    ]
    return particle_filter