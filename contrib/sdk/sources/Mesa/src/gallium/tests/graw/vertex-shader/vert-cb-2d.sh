VERT

DCL IN[0]
DCL IN[1]
DCL OUT[0], POSITION
DCL OUT[1], COLOR
DCL CONST[1][1..2]

MOV OUT[0], IN[0]
MAD OUT[1], IN[1], CONST[1][2], CONST[1][1]

END