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
position startpos
go movetime 10000
info string allocated 10000ms
info depth 2 seldepth 2 score cp 0 time 1 nodes 421 nps 421000 pv a2a3
info depth 3 seldepth 3 score cp 0 time 29 nodes 9323 nps 321482 pv a2a3
info depth 4 seldepth 4 score cp 0 time 204 nodes 206600 nps 1012745 pv a2a3
info depth 5 seldepth 5 score cp 100 time 4044 nodes 5072136 nps 1254237 pv b2b3
bestmove b2b3
```

```
Rank Name                          Elo     +/-   Games    Wins  Losses   Draws   Points   Score    Draw   White   Black
   0 Moinsen-0.0.2                -154      17    1600     382    1049     169    466.5   29.2%   10.6%   31.4%   26.9%
   1 FracTal                       778      86     800     782       0      18    791.0   98.9%    2.3%   99.0%   98.8%
   2 Moinsen-0.0.1                 -50      22     800     267     382     151    342.5   42.8%   18.9%   47.3%   38.4%
```

### 0.0.3
Added piece square tables

```
position startpos
go movetime 10000
info string allocated 10000ms
info depth 1 seldepth 1 score cp 50 time 0 nodes 21 nps 21000 pv b1c3
info depth 2 seldepth 2 score cp 0 time 2 nodes 421 nps 210500 pv b1c3
info depth 3 seldepth 3 score cp 50 time 44 nodes 9323 nps 211886 pv b1c3
info depth 4 seldepth 4 score cp 0 time 447 nodes 206604 nps 462201 pv b1c3
info depth 5 seldepth 5 score cp 70 time 10246 nodes 5072213 nps 495043 pv e2e4
bestmove e2e4
```

```
Rank Name                          Elo     +/-   Games    Wins  Losses   Draws   Points   Score    Draw   White   Black
   0 Moinsen                      -119      18    1600     502    1029      69    536.5   33.5%    4.3%   33.6%   33.4%
   1 FracTal                       660      63     800     767       2      31    782.5   97.8%    3.9%   98.3%   97.4%
   2 Moinsen-main                 -107      25     800     262     500      38    281.0   35.1%    4.8%   34.9%   35.4%
```

### 0.0.4
Add small bonus for having castling rights, add bonus to eval for attacked pieces, small bonus for bishop pair

```
bench
info depth 1 seldepth 1 score cp 49 time 0 nodes 21 nps 21000 pv g1f3
info depth 2 seldepth 2 score cp -2 time 0 nodes 421 nps 421000 pv g1f3
info depth 3 seldepth 3 score cp 49 time 8 nodes 9323 nps 1165375 pv b1c3
info depth 4 seldepth 4 score cp -2 time 198 nodes 206604 nps 1043454 pv g1f3
info depth 5 seldepth 5 score cp 151 time 3174 nodes 5072213 nps 1598050 pv g1f3
bestmove g1f3
```

```
Rank Name                          Elo     +/-   Games    Wins  Losses   Draws   Points   Score    Draw   White   Black 
   0 Moinsen                      -106      16    2000     666    1257      77    704.5   35.2%    3.9%   35.9%   34.5% 
   1 FracTal                       647      52    1000     954       1      45    976.5   97.7%    4.5%   97.5%   97.8% 
   2 Moinsen-main                 -132      23    1000     303     665      32    319.0   31.9%    3.2%   33.5%   30.3%
```

### 0.0.5
Add tapered eval, PST still simplified, e.g. only king has differing endgame PST, slightly adjusted piece values

```
bench
info depth 1 seldepth 1 score cp 50 time 0 nodes 21 nps 21000 pv b1c3
info depth 2 seldepth 2 score cp 0 time 0 nodes 421 nps 421000 pv b1c3 b8c6
info depth 3 seldepth 3 score cp 50 time 17 nodes 9323 nps 548411 pv b1c3 b8c6 g1f3
info depth 4 seldepth 4 score cp 0 time 121 nodes 206604 nps 1707471 pv b1c3 b8c6 g1f3 g8f6
info depth 5 seldepth 5 score cp 154 time 2057 nodes 5072213 nps 2465830 pv g1f3 b8c6 f3g5 f7f5 g5e6
info string 0 0 0 0
bestmove g1f3
```

```
Rank Name                          Elo     +/-   Games    Wins  Losses   Draws   Points   Score    Draw   White   Black
   0 Moinsen                      -114      18    1600     516    1024      60    546.0   34.1%    3.8%   34.4%   33.9%
   1 FracTal                       660      61     800     766       1      33    782.5   97.8%    4.1%   97.6%   98.0%
   2 Moinsen-main                 -116      25     800     258     515      27    271.5   33.9%    3.4%   34.6%   33.3%
```