decode ax25 kiss frames


Build:

gcc decodekiss.c -o decodekiss -g

Usage:

Each frame is in its own seperate file in rawdata folder

for file in rawdata/dataframe-*.bin; do  cat "$file" | ./decodekiss ; done


Example output:


GB7WOK to GB7RDG ctl I+ pid CF  (NRom: GB7WOK GB7RDG-1 I)
  
  <01>PGB7WOK-1 GB7RDG-1

GB7WOD to GB7RDG ctl I+ pid CF  (NRom: GB7WOD GB7RDG-1 I AK)

GB7WOK to GB7RDG ctl I+ pid CF  (NRom: GB7WOK GB7RDG-1 I AK)

GB7WOD to GB7RDG ctl I+ pid CF  (NRom: GB7WOD GB7RDG-1 I)

  <01>KGB7WOD-1 GB7RDG-1 6.0.24.25

GB7WOD to GB7RDG ctl I+ pid CF  (NRom: GB7WOD GB7RDG-1 I AK)

GB7RDG to ID ctl UI^ pid F0

  GB7RDG - Reading - 144.95 1200bd AX.25, 432.625 9k6 IL2P

GB7RDG to ID ctl UI^ pid F0

  GB7RDG - Reading - 144.95 1200bd AX.25, 432.625 9k6 IL2P

GB7WOK to GB7RDG ctl I+ pid CF  (NRom: GB7WOK GB7RDG-1 I)

  <01>RGB7WOK-1 GB7RDG-1

GB7WOD to NODES ctl UI^ pid F0
                   
                   Node:    GB7WOD-1:WODCHT -> GB7WOD  255

                   Node:    GB7WOK-1:JABCHT -> GB7WOK  191
                   
                   Node:    GB7RDG-1:RDGCHT -> GB7RDG  191



GB7WOD to GB7RDG ctl I+ pid CF  (NRom: GB7WOD GB7RDG-1 I)

  <01>RGB7WOD-1 GB7RDG-1

G7BCS-7 to GB7SOU-1 ctl I+ pid CF       (NRom: G7BCS-7 KEEPLI I)

G7BCS-7 to GB7SOU-1 ctl I+ pid CF       (NRom: G7BCS-7 KEEPLI I)

