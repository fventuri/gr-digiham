# gr-digiham: GNU Radio OOT module for digital voice modes

This OOT GNU Radio module supports decoding several digital voice modes, including DMR, D-Star, and YSF

This module leverages the digiham library (https://github.com/jketterl/digiham) for the actual processing.


## Dependencies

This module depends on digiham (https://github.com/jketterl/digiham).

digiham depends on csdr (https://github.com/jketterl/csdr), and codecserver (https://github.com/jketterl/codecserver).

Please build and install csdr, codecserver, and digiham before building this module.


## Build and installation

```
cd gr-digiham
mkdir build
cd build
cmake ..
make
sudo make install
```

# Examples

Examples of GNU Radio Companion flowgraphs for decoding DMR, D-Star, and YSF are provided in the [examples](examples) directory.


## Credits

- Many thanks to Jakob Ketterl, DD5JFK for all his hard work on developing and supporting Csdr, Digiham and Codec Server; without them the gr-digiham OOT module wouldn't be possible
- Franco Spinelli, IW2DHW for his continuous support and testing of ths module


## Copyright

(C) 2021 Franco Venturi - Licensed under the GNU GPL V3 (see [LICENSE](LICENSE))
