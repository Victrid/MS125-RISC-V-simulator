# Memory

Memory is implemented in `memory.cpp`

The memory structure is described below:

```
76543210
76543*** parts stored in a map
*****210 stored in continuous space
```

**Choosing a map** 

~~Map has convenient STL functions.~~  We can spend less memory to simulate this memory.

**Continuous space**

```
Store sequence:		00 01 02 03
					AB CD EF FF
```

Datum is stored in char as-is in the memory (little-endian).

Though commands are forcedly aligned to 4-byte, the data can be accessed everywhere. This feature had consumed me a lot of time to locate.