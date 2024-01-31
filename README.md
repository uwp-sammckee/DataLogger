# DataLogger

PACKET BREAKDOWN:\
0. Time
1. Acc X
2. Acc Y
3. Acc Z
4. Gyro X
5. Gyro Y
6. Gyro Z
7. Mag X
8. Mag Y
9. Mag Z
10. Compass Heading
11. Angle X
12. Angle Y
13. Angle Z
14. Velocity X
15. Velocity Y
16. Velocity Z
17. Temperature
18. Pressure
19. Altitude
20. State
21. GPS Latitude
22. GPS Longitude
23. GPS Satellites
24. GPS Altitude
25. GPS Speed
26. GPS HDOP (Horizontal Dilution of Precision)

## Record.py
You need python, and the pyserial package\
`pip install pyserial`

Usage `python record.py <device>`\
ex. `python record.py com3`

## Coding Conventions
### Naming Conventions
  -Constants should be capitalized and in snake case\
  -Variables should be in camel case\
  -Functions/methods should be in snake case\
  -Classes should be in camel case\

Running at 30hz we get about 51 min of data recording time.\
Running at 60hz we get about 26 min of data recording time.