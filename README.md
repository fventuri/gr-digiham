# gr-digiham: GNU Radio OOT module for digital voice modes

This OOT GNU Radio module supports decoding several digital voice modes, including DMR, D-Star, and YSF

This module leverages the digiham library (https://github.com/jketterl/digiham) for the actual processing.

For the MBE synthesis of the digital voice there are three options:
- none (only the text messages are decoded)
- codecserver (to use specialized hardware for the MBE synthesis via OpenWebRX Codec Server)
- mbelib (to perform the digital voice synthesis in software via the MBE library)
In order to select the digital voice MBE synthesizer, you have to define the 'MBE_SYNTHESIZER' cmake variable as follows:
- none: `cmake -DMBE_SYNTHESIZER=none ..` (default option)
- codecserver: `cmake -DMBE_SYNTHESIZER=codecserver ..`
- mbelib: `cmake -DMBE_SYNTHESIZER=mbelib ..`


## Dependencies

This module depends on digiham (https://github.com/jketterl/digiham).

digiham depends on csdr (https://github.com/jketterl/csdr), and codecserver (https://github.com/jketterl/codecserver).

For the 'mbelib' MBE synthesizer option, this module also requires the mbelib library (https://github.com/szechyjs/mbelib)

Please build and install csdr, codecserver, digiham (and possibly mbelib) before building this module.


## Build and installation

```
cd gr-digiham
mkdir build
cd build
cmake [-DMBE_SYNTHESIZER=none|codecserver|mbelib] ..
make
sudo make install
```

# Examples

Examples of GNU Radio Companion flowgraphs for decoding DMR, D-Star, and YSF are provided in the [examples](examples) directory.


## Credits

- Many thanks to Jakob Ketterl, DD5JFK for all his hard work on developing and supporting Csdr, Digiham and Codec Server; without them the gr-digiham OOT module wouldn't be possible
- Franco Spinelli, IW2DHW for his continuous support and testing of ths module
- Also many thanks to the anonymous author(s) of mbelib for all their work researching, implementing, and testing the codecs used in these Digital Voice protocols.


## Copyright

(C) 2022 Franco Venturi - Licensed under the GNU GPL V3 (see [LICENSE](LICENSE))
