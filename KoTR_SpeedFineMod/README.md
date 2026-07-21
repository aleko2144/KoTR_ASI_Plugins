## KoTR_SpeedFineMod (21.07.2026)
Version 0.01

A modification that makes the police / radar speeding tolerance configurable.

The game issues a fine as soon as the player exceeds the signposted limit by more
than a hard-coded 5 kph. This plugin lets you raise (or lower) that tolerance via
`ToleranceKph` in `KoTR_SpeedFineMod.ini`; a fine then starts only above
`signposted + ToleranceKph`. Both enforcement triggers are covered — police cars
and roadside radar posts. The fine-degree brackets and the limit/speed shown on
the ticket are left untouched.
