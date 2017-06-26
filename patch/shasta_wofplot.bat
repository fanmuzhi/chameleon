echo "enter boot loader mode"
dactyl -o bigep0,ep1crc spi reset
dactyl -o bigep0,ep1crc spi writeep3 nomission

echo "load wofplot patch"
dactyl -o bigep0,ep1crc send -s 2 -b 125 shasta_wofplot_square.blpbin

echo "excute wofplot patch"
dactyl -o bigep0,ep1crc,timeout=99999 send -s 999999 -b 254 

