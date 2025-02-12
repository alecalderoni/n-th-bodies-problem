set term gif animate delay 1 size 1200,1200 font "Arial,20" background rgb "black"
set output "pianeti.gif"
set view 80,50 
set title "Solar system" textcolor rgb "white"
set hidden3d
set xrange [-10:10]
set yrange [-10:10]
set zrange [-10:10]
set key textcolor rgb "white"



do for[i=0:100000] {
splot "dati.csv" u 2:3:4 every ::i::i pt 7 ps 10 lc rgb "orange" notitle,\
      "dati.csv" u 2:3:4 every ::i::i+8 w l lt 2 lw 10 lc rgb "orange" title "1",\
      "dati.csv" u 5:6:7 every ::i::i+8 w l lt 2 lw 1 lc rgb "grey" title "2",\
      "dati.csv" u 5:6:7 every ::i+8::i+8 pt 7 ps 1 lc rgb "grey" notitle,\
      "dati.csv" u 8:9:10 every ::i::i+8 w l lt 2 lw 4 lc rgb "yellow" title "3",\
      "dati.csv" u 8:9:10 every ::i+8::i+8 pt 7 ps 4 lc rgb "yellow" notitle,\
      "dati.csv" u 11:12:13 every ::i::i+8 w l lt 2 lw 4 lc rgb "blue" title "4",\
      "dati.csv" u 11:12:13 every ::i+8::i+8 pt 7 ps 4 lc rgb "blue" notitle
}
