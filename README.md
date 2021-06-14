# AndroidArcoreFacesStreaming

From any Android phone ArCore compatible, using this app will send over TCP 5680 bytes messages:
The first 5616 bytes is a vertex buffer of 468 points mapping the user face in local face space (468 x 3 floats).
Following 64 bytes are the coefficient of a 4x4 Matrix (16floats) representing the transform of the Head bone in world space.

![alt text](https://i.imgur.com/T9EV1fr.png)

![alt text](https://i.imgur.com/w20bTLD.png)
