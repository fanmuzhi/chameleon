dactyl.exe spi writeep3 nomission
dactyl.exe spi reset
dactyl.exe  -o bigep0,ep1crc getver
dactyl.exe  -o bigep0,ep1crc,vio=1800,vdd=1800,vled=0,vtx=3450 send -b 125 ict/obj/sc24-rom/test_ram_ict.blpbin;
dactyl.exe  -o bigep0,ep1crc,vio=1800,vdd=1800,vled=0,vtx=3450 send -b 125 icd/obj/sc24-rom/test_ram_icd.blpbin; 
dactyl.exe  -o bigep0,ep1crc,vio=1800,vdd=1800,vled=0,vtx=3450 send -b 125 dct/obj/sc24-rom/test_ram_dct.blpbin; 
dactyl.exe  -o bigep0,ep1crc,vio=1800,vdd=1800,vled=0,vtx=3450 send -b 125 dcd/obj/sc24-rom/test_ram_dcd.blpbin; 
dactyl.exe  -o bigep0,ep1crc,vio=1800,vdd=1800,vled=0,vtx=3450 send -b 125 sys/obj/sc24-rom/test_ram_sys.blpbin;
dactyl.exe  -o bigep0,ep1crc,vio=1800,vdd=1800,vled=0,vtx=3450 send -b 125 dev/obj/sc24-rom/test_ram_dev.blpbin;
