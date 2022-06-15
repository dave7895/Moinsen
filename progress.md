Engines compared against

| name | version | elo |
| --- | --- | --- |
| FracTal | 1.0 | 1651 |

### 0.0.1
Initial version

```
position startpos
go movetime 10000
info string allocated 10000ms
info depth 2 score cp 0 pv a2a3
info depth 3 score cp 0 pv a2a3
info depth 4 score cp 0 pv a2a3
info depth 5 score cp 100 pv b2b3
info depth 6 score cp 0 pv b2b3
bestmove b2b3
```

```
Score of Moinsen-0.0.1 vs FracTal: 1 - 781 - 18  [0.013] 800
...      Moinsen-0.0.1 playing White: 0 - 391 - 9  [0.011] 400
...      Moinsen-0.0.1 playing Black: 1 - 390 - 9  [0.014] 400
...      White vs Black: 390 - 392 - 18  [0.499] 800
Elo difference: -759.1 +/- 86.0, LOS: 0.0 %, DrawRatio: 2.3 %
```

### 0.0.2
Added added a nodecounter, improved time management

```
Rank Name                          Elo     +/-   Games    Wins  Losses   Draws   Points   Score    Draw   White   Black
   0 Moinsen-0.0.2                -154      17    1600     382    1049     169    466.5   29.2%   10.6%   31.4%   26.9%
   1 FracTal                       778      86     800     782       0      18    791.0   98.9%    2.3%   99.0%   98.8%
   2 Moinsen-0.0.1                 -50      22     800     267     382     151    342.5   42.8%   18.9%   47.3%   38.4%
```