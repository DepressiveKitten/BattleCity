#pragma once

const int GSCALE=3;
const int GFRAMERATE=50;
const int GBORDER = 13*16*GSCALE;
const float GTANKSPEED=35*(float)GSCALE/(float)GFRAMERATE;
const float GBULLETSPEED=90*(float)GSCALE/(float)GFRAMERATE;
const int GTANKANIM=GFRAMERATE*0.1;
const int GENEMYSHOTCOOLDOWN=2*GFRAMERATE;
const int GENEMYSPAWN=5*GFRAMERATE;
const int GPLAYERSPAWN=7.5*GFRAMERATE;
const int GSPAWNTICK=GFRAMERATE/12;