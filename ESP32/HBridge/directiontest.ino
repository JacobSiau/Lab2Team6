

// at this point we will have a Vl and Vr

// find l: the axle length 

float l; 
float vl, vr;
float vdif = vr - vl;
float R = (l/2) * (vl + vr) / (vdif);
float w = vdif / l;
float ICC = 