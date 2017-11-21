<?xml version="1.0" encoding="UTF-8"?>
<tileset name="boat_01" tilewidth="128" tileheight="128" tilecount="24" columns="8">
 <tileoffset x="-14" y="11"/>
 <grid orientation="orthogonal" width="8" height="8"/>
 <image source="../img/original/t_boat_01.png" width="1024" height="384"/>
 <tile id="0" type="boatStill_SE"/>
 <tile id="1" type="boatStill_EE"/>
 <tile id="2" type="boatStill_NE"/>
 <tile id="3" type="boatStill_NN"/>
 <tile id="4" type="boatStill_NW"/>
 <tile id="5" type="boatStill_WW"/>
 <tile id="6" type="boatStill_SW"/>
 <tile id="7" type="boatStill_SS"/>
 <tile id="8" type="boatMoving_SE">
  <animation>
   <frame tileid="8" duration="100"/>
   <frame tileid="16" duration="100"/>
  </animation>
 </tile>
 <tile id="9" type="boatMoving_EE">
  <animation>
   <frame tileid="9" duration="100"/>
   <frame tileid="17" duration="100"/>
  </animation>
 </tile>
 <tile id="10" type="boatMoving_NE">
  <animation>
   <frame tileid="10" duration="100"/>
   <frame tileid="18" duration="100"/>
  </animation>
 </tile>
 <tile id="11" type="boatMoving_NN">
  <animation>
   <frame tileid="11" duration="100"/>
   <frame tileid="19" duration="100"/>
  </animation>
 </tile>
 <tile id="12" type="boatMoving_NW">
  <animation>
   <frame tileid="12" duration="100"/>
   <frame tileid="20" duration="100"/>
  </animation>
 </tile>
 <tile id="13" type="boatMoving_WW">
  <animation>
   <frame tileid="13" duration="100"/>
   <frame tileid="21" duration="100"/>
  </animation>
 </tile>
 <tile id="14" type="boatMoving_SW">
  <animation>
   <frame tileid="14" duration="100"/>
   <frame tileid="22" duration="100"/>
  </animation>
 </tile>
 <tile id="15" type="boatMoving_SS">
  <animation>
   <frame tileid="15" duration="100"/>
   <frame tileid="23" duration="100"/>
  </animation>
 </tile>
</tileset>
