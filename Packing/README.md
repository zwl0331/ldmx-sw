# Packing

This ldmx-sw module is focused on interfacing between the raw, binary data coming out of LDMX's different subsystems 
and the centralized, hierachical data that is used for the rest of our processing chain.

The hierarchical objects that are most similar to the raw binary data are the "digi" objects.
These objects (especially the HgcrocDigiCollection) directly reference the encoded words that are a part of the raw binary files.

This raw-digi conversion is done in two steps.

### 1. Merge
Merge the various raw files coming off simultaneouslly from the different subsystems into a single file. 
At this time, we also partially unpack the binary files into a ROOT TTree that can be fed into ldmx-sw.
By "partially" I mean we only unpack the binary files in a way that is uniform across all subsystems.
This way, we can isolate any subsystem/chip-specific decoding to the second step.

This step is meant to be isolated from ldmx-sw so hardware close to the detector that does
this merging does not need a full build of ldmx-sw to function.

### 2. Unpack
Finish unpacking the encoded data by utilising chip-specific decoding routines.
We can then create the "digi" objects that can be utilized by further reconstruction steps.

This step is meant to be _the_ translation step from "online" data to "offline" data (ldmx-sw).

## Band Aids
While we are waiting to nail down the specifics of the readout of the various subsystems,
we will need some extra code to insert the expected header information that will surround
the raw files currectly being read off a stand-a-lone chip.