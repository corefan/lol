
//SCENE SETUP END

//addlight 0.0 position (1 0 1) color #0ff
//addlight 0.0 position (-0.2 -1 -0.5) color (.0 .2 .5 1)

//custom setmesh "sc#f00 ab 2 ty 1.5 ab 2"

custom setmesh "

//tqw
//CMD TEST
//sc#0f0 asph 1 10
//sc#0f0 ab 10
//sc#0f0 afcb 1 -.2
//sc#0f0 acg 8 1 1 1 2 2 1.5 1.5 -.4
//sc#0f0 as 6 1 2
//sc#0f0 scb#f00 aes 6 1 2 2
//sc#0f0 ac 6 5 2 2 false true true
//sc#0f0 aq 1
//splt 2 smth 4 0 1

//[sc#0f0 ac 3 .5 .4 0 ty .25 [ad 3 .4 sy -1] ty .5 ac 3 1 .1 ty .5 dup [ rz 90 ry 90 scv#00f dup [ ry 90 scv#f00 ] ] ]
//[sc#fff ab .1]

//[sc#0f0 ac 3 .5 .4 0 ty .25 [ad 3 .4 sy -1] ty .5 ac 3 1 .1 ty .5 dup[rz 90 ry 90 scv#00f dup[ry 90 scv#f00]]][sc#fff ab .1]

//[sc#fff ab 1 tx .5 ty .5]
//[sc#fff ab 1 tx -.5 ty -.5]


//[sc#fff ac 3 .5 .4 0 ty .25 [ad 3 .4 sy -1] ty .5 ac 3 1 .1 ty .5 [ad 3 .1 sy -1] ty 1]

//smth 3 1 1 smth 8 0 1
//[sc#fff loop 4 [ [loop 4 [ab 1 tx 2]] tz 2]]


//[sc#f00 scb#f00 ac 3 .5 .4 0 ty .25 [ad 3 .4 sy -1] ty .5 ac 3 1 .1 .1 ty .5 rz -90]
//[sc#00f scb#00f ac 3 .5 .4 0 ty .25 [ad 3 .4 sy -1] ty .5 ac 3 1 .1 .1 ty .5 rx 90]
//[sc#f00 ab .2 tx 1]
//[sc#0f0 ab .2 ty 1]
//[sc#00f ab .2 tz 1]
//sc#0f0 ab 2 ty 1.5 ab 2
//sc#00f ab 2 ty 1.5 ab 2

//sc#00f tqw lp 5[tz 11 [lp 6 [tx 5 ty 5 afcb 10 -1 ]]] tz -22

// tqw
sc#f00 ab 10 
//tz 5
//vm
// splt 1
// rj .2
// splt 1
// rj .2
// smth 1 0 1

//[sc#66f afcb(10) .25tx0]csgs[sc#fff afcb(10).25t(2)][[sc#6f6 afcb(7).25]csgs[sc#fff afcb(7).25t(1.5)]][[sc#f44 asph4 12t(-2.5)]csga[sc#fff afcb(7).25t(1.4)]csgs[sc#fff afcb(7).25t(2.5)]][[sc#ff0 ato6 2.5 1.2rx90tz-1.25tx-.9][sc#ff0 ab.5 2.1 .5dup[rz90sx.5ty-.8tx-.28]ty.55tx.25tz-.4taz1 1sy.8sx.8ty-.55tx-.25tz.4tz-1.2tx1.2]tz1.25tx1.25dup[sx-1ry90]tz-1.25tx-1.25]

"

//splt 0 
//test
//[sc#f8f ab 1 splt 4 twy 90]


// splt 5 twy 90 0 
//[sc#f8f afcb 10 10 10 .25 tx 0 splt 4 twy 1.6 0 ]
//sc#fff
//[afcb 1 1 1 -.1 tx 1]
//[afcb 1 1 1 -.1 tx 0]
//[afcb 1 1 1 -.1 tx -1]
//[[afcb 1 1 1 -.1 tx 1]
//[afcb 1 1 1 -.1 tx 0]
//[afcb 1 1 1 -.1 tx -1]tz 1]
//[[afcb 1 1 1 -.1 tx 1]
//[afcb 1 1 1 -.1 tx 0]
//[afcb 1 1 1 -.1 tx -1]tz -1]
//[afcb 3 .2 3 -.05 ty -.6]


//[sc#fff scb#fff ato 20 8 12 rx0]
//[sc#fff scb#fff acg 8 2 5 1 6 8 4 2 .1 1]

//[sc#fff ab 4 4 4 ]//twy 45 0 bdxy 90 0 splt 5 tz 2 
//[sc#88f ab 4 4 4 tx 4 ab 4 4 4 tx -2 tax .4 .4 0]
//[sc#88f ab .25 1 1 tx .25 ab .25 1 1 tx .25 ab .25 1 1 tx .25 ab .25 1 1 tx .125 stx 10 0 0]
 // tax -2 0 0 0
//[sc#88f ab 4 4 4 tx 4 ab 4 4 4 tx -2 shz 1 0 0 0]
//[sc#fff asph 4 4]
//[sc#ff2 asph 4 4 tax .4 .4 0]
//[sc#ff2 asph 2 4]
//[sc#fff acap 1 4 4]
//[sc#fff scb#fff ac 10 4 4 4 0 0 1]
//[sc#ff2 scb#ff2 ad 10 4 0]
//[sc#fff scb#fff at 4 1]
//[sc#fff scb#000 aq 4 1]
//[sc#fff scb#2ff aes 5 3 6 2]
//[sc#fff scb#2ff as 4 2 4 0 0]
//[sc#fff scb#fff acg 2 10 .1 .1 .4 .4 .1 .1 0 1]

//[sc#ff2 asph 2 10 10 10]
//[sc#400 asph 2 10 10 10 t 2 2 2 csgs]
